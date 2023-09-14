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
  hyped::debug::Repl repl(logger, terminal);
  if (argc > 1) {
    repl.run();
  } else {
    logger.log(hyped::core::LogLevel::kFatal, "Usage: %s [config_file]", argv[0]);
    return 1;
  }
}