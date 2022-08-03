#include "catch2/catch_test_macros.hpp"
#include "jobsystem/workers.h"

TEST_CASE("Job System", "[JobSystem]") {
  using namespace WhiteDeer::Engine;
  int sum = 0;
  int *psum = &sum;
  auto job = WorkerGroup::GetJobWorkers()->ScheduleJob(
      [](void *p, int index) { *(int *)p += index; }, psum, 101, nullptr);

  // spinning wait until job finishes
  while (job->status != JobStatus::Completed) {
  }
  REQUIRE(*psum == 5050);
}