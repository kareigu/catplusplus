#include "core.h"
#include <args-parser/all.hpp>
#include <fmt/core.h>
#include <filesystem>
#include <string>
#include <fstream>

#include <vector>

namespace Application {

  struct File {
    Args::String path;
    bool exists;
  };

  struct FileList {
    bool all_exist;

    size_t size() { return m_list.size(); }
    auto begin() { return m_list.begin(); }
    auto end() { return m_list.end(); }

    std::vector<File> m_list;
  };

  FileList files_exist(const Args::StringList& paths) {
    std::vector<File> m_list;
    bool m_exists = true;

    for (const auto& path : paths) {

      bool file_exists = std::filesystem::exists(path);
      m_exists = !file_exists ? false : m_exists;
      m_list.emplace_back(File { .path = path, .exists = file_exists });

      #if 0
        fmt::print("{} - {}\n", path, file_exists);
      #endif
    }

    return FileList { .all_exist = m_exists, .m_list = m_list };
  }

	int run(int argc, char** argv) {
		try {
      Args::CmdLine cli(argc, argv);

      cli
        .addMultiArg('f', "file", true, true, "Path to file", "Path of file to read")
        .addHelp(true, argv[0], "Basic cat clone written in C++");

      cli.parse();

      FileList list = files_exist(cli.values("-f"));

      if (list.all_exist) {
        for (const auto& file : list) {
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
        for (const auto& file : list) {
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
