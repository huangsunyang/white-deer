#include <plog/Log.h>
#include <plog/Initializers/RollingFileInitializer.h>
#include <plog/Appenders/ColorConsoleAppender.h>
#include "filesystem/filesystemmanager.h"

#include <ctime>

using namespace WhiteDeer::Engine;
struct LogInitializer {
  LogInitializer() {
    // log to console
    static plog::ColorConsoleAppender<plog::TxtFormatter> consoleAppender;
    plog::init(plog::debug, &consoleAppender);

    // log to file
    auto localfs = FileSystemManager::GetLocalFileSystem();
    auto logFolder = localfs->ToAbsolute("log");
    fs::create_directories(logFolder);

    // use current time as log name postfix
    auto timestamp = std::time(nullptr);
    struct tm timestruct;
    localtime_s(&timestruct, &timestamp);
    std::ostringstream oss;
    oss << std::put_time(&timestruct, "%Y-%m-%d-%H-%M-%S.log");
    auto logPath = logFolder / fs::path(oss.str());

    plog::init(plog::debug, logPath.string().c_str());
  }
};

static LogInitializer __initFileLogger;