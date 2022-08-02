/*
 * Worker Group, just as its name says, is a group of workers (threads) that do
 * same kind of jobs. Worker Group accepts a number of jobs and assign the jobs
 * among all the workers.
 *
 * Worker is just another name for a thread.
 *
 * Job is an abstraction of function and data.
 */

#include <thread>
#include <vector>

namespace WhiteDeer {
namespace Engine {
class WorkerGroup {};
} // namespace Engine
} // namespace WhiteDeer
