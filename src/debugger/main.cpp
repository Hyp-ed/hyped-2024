#include <core/logger.hpp>
#include <core/wall_clock.hpp>
#include <debug/repl.hpp>

int main(const int argc, char **argv)
{
  hyped::core::WallClock time;
  hyped::core::Logger logger("Debugger", hyped::core::LogLevel::kDebug, time);
  hyped::debug::Repl repl(logger);
  if (argc > 1) {
    auto repl_optional = repl.fromFile(argv[1]);
    if (!repl_optional) {
      logger.log(hyped::core::LogLevel::kFatal, "Failed to create debugger from file %s", argv[1]);
      return 1;
    }
    auto repl = std::move(*repl_optional);
    repl->run();
  } else {
    logger.log(hyped::core::LogLevel::kFatal, "Usage: %s [config_file]", argv[0]);
    return 1;
  }
}