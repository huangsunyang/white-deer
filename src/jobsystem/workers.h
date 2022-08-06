/*
 * Worker Group, just as its name says, is a group of workers (threads) that do
 * same kind of jobs. Worker Group accepts a number of jobs and assign the jobs
 * among all the workers.
 *
 * Worker is just another name for a thread.
 *
 * Job is an abstraction of function and data.
 */
#pragma once

#include <condition_variable>
#include <future>
#include <map>
#include <memory>
#include <mutex>
#include <queue>
#include <string>
#include <thread>
#include <vector>
#include "log/log.h"

using std::condition_variable;
using std::future;
using std::map;
using std::mutex;
using std::queue;
using std::shared_ptr;
using std::string;
using std::thread;
using std::vector;

namespace WhiteDeer {
namespace Engine {

typedef void (*JobFunc)(void *);
typedef void (*JobForEachFunc)(void *, int);
typedef void (*JobCallback)(void *);

typedef unsigned int JID;

enum class JobStatus {
  Idle,
  Running,
  Completed,
};

struct Job {
  Job() : id(GenJobID()) {}
  ~Job(){
    LOGD.printf("Job Deleting %d", id);
  }
  inline static JID GenJobID() {
    static int ID = 0;
    return ID++;
  }

  JobFunc func = nullptr;
  JobForEachFunc ffunc = nullptr;
  void *data = nullptr; // custom data, managed by user
  int count = 1;
  JobCallback callback = nullptr; // callback in thread, only for count > 1

  // used internal
  JID id;
  int left = 1;
  mutex mtx;
  JobStatus status = JobStatus::Idle;
};

class WorkerGroup {
public:
  explicit WorkerGroup(const string &name, int worker_count = 1);
  ~WorkerGroup();

  shared_ptr<Job> ScheduleJob(JobFunc func, void *jobData);
  shared_ptr<Job> ScheduleJob(JobForEachFunc func, void *jobData, int n,
                              JobCallback callback);

  static WorkerGroup *GetJobWorkers() {
    static WorkerGroup *instance = new WorkerGroup("JobThread", 4);
    return instance;
  }

  static WorkerGroup *GetFileAsyncWorkers() {
    static WorkerGroup *instance = new WorkerGroup("FileAsyncThread", 1);
    return instance;
  }

  static WorkerGroup *GetRenderWorkers() {
    static WorkerGroup *instance = new WorkerGroup("RenderThread", 1);
    return instance;
  }

protected:
  shared_ptr<Job> CreateJob(JobFunc func, void *jobData);
  shared_ptr<Job> CreateJob(JobForEachFunc func, void *jobData, int n,
                            JobCallback callback);
  shared_ptr<Job> ScheduleJob(shared_ptr<Job> job);
  void StopAllWorkers();
  void WorkerLoop();

  static bool m_quit;
  string m_name;
  vector<thread> m_workers;
  queue<shared_ptr<Job>> m_jobQueue;

  mutex m_mutex;
  condition_variable m_cv;
};
} // namespace Engine
} // namespace WhiteDeer
