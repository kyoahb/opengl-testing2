#pragma once

#pragma warning(push)
#pragma warning(disable : 26437) // Disable warning C26437 for spdlog
#pragma warning(disable : 26498) // Disable warning C26498 for fmt
#pragma warning(disable : 4005) // Disable APIENTRY warning

#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

#pragma warning(pop) // Restore warnings

class Log {
public:
    static void initialize();
       

private:
    static std::string generate_log_filename();
};