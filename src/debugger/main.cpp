#include <core/logger.hpp>
#include <core/wall_clock.hpp>
#include <debug/repl.hpp>
#include <debug/repl_logger.hpp>
#include <debug/terminal.hpp>

int main(const int argc, char **argv)
{
  hyped::core::WallClock time;
  hyped::debug::Terminal terminal;
  terminal.initialize_window();
  hyped::debug::ReplLogger logger("REPL", hyped::core::LogLevel::kDebug, time, terminal);
  if (argc > 1) {
    auto optional_repl = hyped::debug::Repl::create(logger, terminal, argv[1]);
    if (!optional_repl) {
      logger.log(
        hyped::core::LogLevel::kFatal, "Failed to create debugger using file: %s", argv[1]);
      return 1;
    }
    auto repl = std::move(*optional_repl);
    repl->run();
  } else {
    logger.log(hyped::core::LogLevel::kFatal, "Usage: %s [config_file]", argv[0]);
    return 1;
  }
}
