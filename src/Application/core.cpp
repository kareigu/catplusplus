#include "core.h"
#include <fmt/core.h>
#include <args-parser/all.hpp>
#include <filesystem>
#include <vector>
#include <string>
#include <fstream>

namespace Application {
  struct File {
    Args::String path;
    bool exists;
  };

  struct FileList {
    bool all_exist;
    std::vector<File> list;

    size_t size() { return list.size(); }
  };

  FileList files_exist(const Args::StringList& paths) {
    std::vector<File> list;
    bool exists = true;

    for (const auto& path : paths) {

      bool file_exists = std::filesystem::exists(path);
      exists = !file_exists ? false : exists;
      list.emplace_back(File { .path = path, .exists = file_exists });

      #if 0
        fmt::print("{} - {}\n", path, file_exists);
      #endif
    }

    return FileList { .all_exist = exists, .list = list };
  }

	int run(int argc, char** argv) {
		try {
      Args::CmdLine cli(argc, argv);

      cli
        .addArgWithFlagAndName('t', "test", true, false, "Test argument", "Testing argument")
        .addMultiArg('f', "file", true, true, "Path to file", "Path of file to read")
        //.addArgWithFlagAndName('f', "file", true, true, "Path to file", "Path of file to read")
        .addHelp(true, argv[0], "Basic cat clone written in C++");

      cli.parse();
      std::string test = cli.isDefined("-t") ? cli.value("-t") : "Not specified";


      FileList list = files_exist(cli.values("-f"));

      if (list.all_exist) {
        for (const auto& file : list.list) {
          fmt::print("--[{}]--\n", file.path);
          std::ifstream fs(file.path);
          if (fs.is_open()) {
            std::string line;
            size_t line_count = 0; 
            while (std::getline(fs, line)) {
              fmt::print("{:>4}| {}\n", ++line_count, line);
            }

            fs.close();
            fmt::print("\n");
          } else {
            fmt::print("Couldn't open file [{}]", file.path);
            return EXIT_FAILURE;
          }
        }

        return EXIT_SUCCESS;
      } else {
        std::string invalid_files = "";
        for (const auto& file : list.list) {
          if (!file.exists) {
            std::string start = invalid_files.empty() ? "" : ", ";
            invalid_files.append(fmt::format("{}{}", start, file.path));
          }
        }

        if (list.size() > 1)
          fmt::print("Files [{}] don't exist\n", invalid_files);
        else
          fmt::print("File [{}] doesn't exist\n", invalid_files);
        return EXIT_FAILURE;
      }
      
      //if (std::filesystem::exists(cli.value()))

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
