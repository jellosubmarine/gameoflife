#include "options.h"

static constexpr auto USAGE =
    R"(Happy project.
  Usage:
          sfml_cuda [options]

  Options:
          -h --help         Show this screen.
          -v --verbose      Verbose output.
          --width=WIDTH     Screen width in pixels [default: 320].
          --height=HEIGHT   Screen height in pixels [default: 320].
)";

Options::Options(std::vector<std::string> const &argv) {
  args = docopt::docopt(USAGE, argv,
                        true,     // show help if requested
                        "0.0.0"); // version string

  if (args["--verbose"].asBool())
    spdlog::set_level(spdlog::level::debug);

  width = args["--width"].asLong();
  height = args["--height"].asLong();
}

void Options::checkOptions() {
  if (width < 8 || height < 8) {
    spdlog::error("Command line options are out of reasonable range.");
    for (auto const &arg : args) {
      if (arg.second.isString()) {
        spdlog::info("Parameter set: {}='{}'", arg.first,
                     arg.second.asString());
      }
    }
    abort();
  }
}