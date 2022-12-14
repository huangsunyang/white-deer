#include "catch2/catch_test_macros.hpp"
#include "log/log.h"
#include "plog/Initializers/RollingFileInitializer.h"

TEST_CASE("Test plog", "plog") {
  plog::init(plog::debug, "Hello.txt");  // Step2: initialize the logger

  // Step3: write log messages using a special macro
  // There are several log macros, use the macro you liked the most

  PLOGD << "Hello log!";              // short macro
  PLOG_DEBUG << "Hello log!";         // long macro
  PLOG(plog::debug) << "Hello log!";  // function-style macro

  // Also you can use LOG_XXX macro but it may clash with other logging
  // libraries
  LOGD << "Hello log!";              // short macro
  LOG_DEBUG << "Hello log!";         // long macro
  LOG(plog::debug) << "Hello log!";  // function-style macro
}