#include "Log.h"

void Log::initialize() {
    // Generate a unique log file name
    std::string log_filename = generate_log_filename();

    // Create console and file sinks
    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(log_filename, true);

    // Create a logger with both sinks
    spdlog::sinks_init_list sink_list = { console_sink, file_sink };
    auto logger = std::make_shared<spdlog::logger>("multi_sink", sink_list.begin(), sink_list.end());
    spdlog::set_default_logger(logger);
    spdlog::set_level(spdlog::level::trace); // Set global log level to info
    spdlog::flush_on(spdlog::level::trace);  // Flush log on each info level message

    spdlog::info("Finished loading logger.");
}

std::string Log::generate_log_filename(){
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    std::tm buf;
    localtime_s(&buf, &in_time_t);
    std::stringstream ss;
    ss << "logs/log_" << std::put_time(&buf, "%Y-%m-%d_%H-%M-%S") << ".txt";
    return ss.str();
}