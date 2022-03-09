#include "Application/core.h"
#include "Application/files.h"
#include <args-parser/all.hpp>
#include <fmt/core.h>
#include <filesystem>
#include <string>
#include <fstream>
#include <vector>

namespace Application {
  int handle_file_list(const FileList& list) {
    if (!list.all_exist) {
      std::string invalid_files = list.invalids_to_string();
      if (list.size() > 1)
        fmt::print("Files [{}] don't exist\n", invalid_files);
      else
        fmt::print("File [{}] doesn't exist\n", invalid_files);

      return EXIT_FAILURE;
    }

    for (const auto& file : list) {
      fmt::print("\e[0;100m--[{}]--\e[0m\n", file.path);
      std::ifstream fs(file.path);
      if (fs.is_open()) {
        std::string line;
        size_t line_count = 0; 
        while (std::getline(fs, line)) {
          fmt::print("\e[32;40m{:>4}\e[0;40m|\e[0m {}\n", ++line_count, line);
        }

        fs.close();
        fmt::print("\n");
      } else {
        fmt::print("Couldn't open file [{}]", file.path);
        return EXIT_FAILURE;
      }
    }

    return EXIT_SUCCESS;
  }

	int run(int argc, char** argv) {
		try {
      Args::CmdLine cli(argc, argv);

      cli
        .addMultiArg('f', "file", true, true, "Path to file", "Path of file to read")
        .addHelp(true, argv[0], "Basic cat clone written in C++");

      cli.parse();

      FileList list(cli.values("-f"));

      return handle_file_list(list);
    } 
    catch (const Args::HelpHasBeenPrintedException &) {
      return EXIT_SUCCESS;
    } 
    catch (const Args::BaseException & e) {
      fmt::print("{}\n", e.desc());

      return EXIT_FAILURE;
    }

		return EXIT_SUCCESS;
	}
}
