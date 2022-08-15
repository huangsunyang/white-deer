#include <catch2/catch_test_macros.hpp>
#include <fstream>
#include "log/log.h"
#include "filesystem/localfilesystem.h"

using namespace WhiteDeer::Engine;

TEST_CASE("load model", "[model]") {
    auto localfs = GetLocalFileSystem();
    auto path = localfs->ToAbsolute("package/models/bun_zipper.ply");
    std::fstream f(path.string());
    if (!f.is_open()) {
        LOGE << "open failed";
    }
    string temp;
    LOGE << "test ply";
    while (f >> temp) {
        LOGE << temp;
        if (temp == "end_header") {
            break;
        }
    }
}