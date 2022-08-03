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

struct Job {
  Job() : id(GenJobID()) {}
  inline static JID GenJobID() {
    static int ID = 0;
    return ID++;
  }

  JobFunc func = nullptr;
  JobForEachFunc ffunc = nullptr;
  void *data = nullptr;  // custom data, managed by user
  int count = 1;
  JobCallback callback = nullptr;  // callback in thread, only for count > 1

  // used internal
  JID id;
  int left = 1;
  mutex mtx;
};

class WorkerGroup {
 public:
  explicit WorkerGroup(const string &name, int worker_count = 1);
  ~WorkerGroup();

  void StopAllWorkers();
  JID ScheduleJob(JobFunc func, void *jobData);
  JID ScheduleJob(JobForEachFunc func, void *jobData, int n,
                  JobCallback callback);
  // todo: make it clear
  bool HasJob(JID id) { return m_livingJobs.find(id) != m_livingJobs.end(); }

 protected:
  shared_ptr<Job> CreateJob(JobFunc func, void *jobData);
  shared_ptr<Job> CreateJob(JobForEachFunc func, void *jobData, int n,
                            JobCallback callback);
  JID ScheduleJob(shared_ptr<Job> job);
  void WorkerLoop();

  static bool m_quit;
  string m_name;
  vector<thread> m_workers;
  queue<shared_ptr<Job>> m_jobQueue;
  map<JID, shared_ptr<Job>> m_livingJobs;

  mutex m_mutex;
  condition_variable m_cv;
};
}  // namespace Engine
}  // namespace WhiteDeer
