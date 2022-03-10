#include "Application/core.h"
#include "Application/files.h"
#include <CLI11/CLI11.hpp>
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

  bool read_cin(std::vector<std::string>& files) {
    bool got_input = false;
    std::string line;
    while (std::cin >> line) {
      files.emplace_back(line);
      got_input = true;
    }
    return got_input;
  }

	int run(int argc, char** argv) {
    CLI::App app("cat clone written in C++", "catpp");

    std::vector<std::string> files;
    auto file = app.add_option("file, -f, --file", files, "path(s) to file(s)");
    file->expected(-1)->delimiter(' ')->required();
    app.alias("cat++");

    try {
      app.parse(argc, argv);
    } catch(const CLI::ParseError &e) { 
      if (e.get_name() == "RequiredError" && std::string(e.what()).ends_with("file is required")) {
        if (!read_cin(files))
          return app.exit(e);
      } else {
        return app.exit(e); 
      }
    }

    FileList list(files);

    return handle_file_list(list);
	}
}
