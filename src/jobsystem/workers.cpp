#define NOMINMAX
#ifdef _WIN32
#include "Windows.h"
#endif

#include <string>

#include "Tracy.hpp"
#include "jobsystem/workers.h"

namespace WhiteDeer {
namespace Engine {

// init static values
bool WorkerGroup::m_quit = false;

WorkerGroup::WorkerGroup(const string &name, int worker_count) {
  m_name = name;
  for (int i = 0; i < worker_count; i++) {
    m_workers.emplace_back(&WorkerGroup::WorkerLoop, this);

    // set name for threads...
#ifdef _WIN32
    auto thread_handle = static_cast<HANDLE>(m_workers.back().native_handle());
    auto temp_name = name + "_" + std::to_string(i);
    size_t convertedChars = 0;
    wchar_t wcstring[255];
    mbstowcs_s(&convertedChars, wcstring, 255, temp_name.c_str(), _TRUNCATE);
    SetThreadDescription(thread_handle, wcstring);
#endif
  }
}

WorkerGroup::~WorkerGroup() {
  StopAllWorkers();
  for (auto &thread : m_workers) {
    thread.join();
  }
}

void WorkerGroup::StopAllWorkers() {
  m_quit = true;
  m_cv.notify_all();
}

shared_ptr<Job> WorkerGroup::CreateJob(JobFunc func, void *jobData) {
  auto job = std::make_shared<Job>();
  job->func = func;
  job->data = jobData;
  return job;
}

shared_ptr<Job> WorkerGroup::CreateJob(JobForEachFunc ffunc, void *jobData,
                                       int count, JobCallback callback) {
  auto job = std::make_shared<Job>();
  job->ffunc = ffunc;
  job->data = jobData;
  job->count = job->left = count;
  job->callback = callback;
  return job;
}

shared_ptr<Job> WorkerGroup::ScheduleJob(JobFunc func, void *jobData) {
  auto job = CreateJob(func, jobData);
  return ScheduleJob(job);
}

shared_ptr<Job> WorkerGroup::ScheduleJob(JobForEachFunc ffunc, void *jobData,
                                         int count, JobCallback callback) {
  auto job = CreateJob(ffunc, jobData, count, callback);
  return ScheduleJob(job);
}

shared_ptr<Job> WorkerGroup::ScheduleJob(shared_ptr<Job> job) {
  {
    std::lock_guard lg(m_mutex);
    m_jobQueue.push(job);
  }

  // conditional_variable and mutex are waiting on their own:
  // unlock before notify: notify(1)->awake(2)->acquire lock success(2)
  // unlock after notify : notify(1)->awake(2)->acquire lock failed(2)
  //                       ->release lock(1)->acquire lock success(2)
  // https://web.stanford.edu/~ouster/cgi-bin/cs140-spring14/lecture.php?topic=locks
  if (job->count > 1) {
    m_cv.notify_all();
  } else {
    m_cv.notify_one();
  }
  return job;
}

void WorkerGroup::WorkerLoop() {
  LOGD.printf("worker started %zd", std::this_thread::get_id());
  while (!m_quit) {
    shared_ptr<Job> job;
    int job_index = 0;
    {
      // get one job
      ZoneScopedN("JobQueue Pop");
      std::unique_lock lk(m_mutex);
      m_cv.wait(lk, [this] { return m_quit || !m_jobQueue.empty(); });
      if (m_quit) {
        return;
      }
      job = m_jobQueue.front();
      if (--job->count <= 0) {
        m_jobQueue.pop();
      }
      job_index = job->count;
      job->status = JobStatus::Running;
    }

    ZoneScopedN("Run Job");

    // run job
    bool all_complete = false;
    if (job->func) {
      job->func(job->data);
      all_complete = true;
    } else if (job->ffunc) {
      job->ffunc(job->data, job_index);

      all_complete = false;
      {
        std::lock_guard lg(job->mtx);
        all_complete = --job->left == 0;
      }
      if (all_complete && job->callback) {
        job->callback(job->data);
      }
    }

    if (all_complete) {
      job->status = JobStatus::Completed;
    }
  }
}

}  // namespace Engine
}  // namespace WhiteDeer