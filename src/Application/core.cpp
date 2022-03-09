#include "core.h"
#include <fmt/core.h>
#include <args-parser/all.hpp>

namespace Application {
	int run(int argc, char** argv) {
		try {
      Args::CmdLine cli(argc, argv);

      cli
        .addArgWithFlagAndName('t', "test", true, true, "Test argument", "Testing argument")
        .addHelp(true, argv[0], "Basic cat clone written in C++");

      cli.parse();
      std::string test = cli.isDefined("-t") ? cli.value("-t") : "Not specified";

      fmt::print("{}\n", test);

    } catch (const Args::HelpHasBeenPrintedException &) {
      return 0;
    } catch (const Args::BaseException & e) {
      fmt::print("{}\n", e.desc());
      return 1;
    }

		return 0;
	}
}
