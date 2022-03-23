#include "Application/core.h"
#include "Application/files.h"
#include <CLI11/CLI11.hpp>
#include <fmt/core.h>
#include <filesystem>
#include <string>
#include <fstream>
#include <vector>

namespace Application {
  enum FormatFlags {
    LineNumbers = 0b00000001,
    Headers = 0b00000010,
    Colours = 0b00000100,
    AllOn = 0b11111111,
    AllOff = 0b0000000,
  };

  struct Config {
    uint8_t format_flag = 0b11111111;

    void update_config(const uint8_t& flag_sum) {
      format_flag = format_flag ^ flag_sum;
    }

    auto base_colour() const { 
      return format_flag & FormatFlags::Colours ? "\e[0m" : ""; 
    }

    auto header_colour() const {
      return format_flag & FormatFlags::Colours ? "\e[0;100m" : "";
    }

    auto line_number_colour() const {
      return format_flag & FormatFlags::Colours ? "\e[32;40m" : "";
    }

    auto line_pillar_colour() const {
      return format_flag & FormatFlags::Colours ? "\e[0;40m" : "";
    }
  };

  const std::string format_line_number(const size_t& line_count, const Config& config) {
    return config.format_flag & FormatFlags::LineNumbers 
      ? fmt::format(
          "{1}{0:>4}{2}|{3} ", 
          line_count, 
          config.line_number_colour(),
          config.line_pillar_colour(),
          config.base_colour()
        ) 
      : "";
  }


  int handle_file_list(const Config& config, const FileList& list) {
    if (!list.all_exist) {
      std::string invalid_files = list.invalids_to_string();
      if (list.size() > 1)
        fmt::print("Files [{}] don't exist\n", invalid_files);
      else
        fmt::print("File [{}] doesn't exist\n", invalid_files);

      return EXIT_FAILURE;
    }

    for (const auto& file : list) {

      if (config.format_flag & FormatFlags::Headers)
        fmt::print("{1}--[{0}]--{2}\n", file.path, config.header_colour(), config.base_colour());

      std::ifstream fs(file.path);
      if (fs.is_open()) {
        std::string line;
        size_t line_count = 0; 
        while (std::getline(fs, line)) {
          fmt::print("{}{}\n", format_line_number(++line_count, config), line);
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

	int run(int& argc, char**& argv) {
    CLI::App app("cat clone written in C++", "catpp");
    Config config = Config { .format_flag = FormatFlags::AllOn };

    uint8_t line_flag;
    app
      .add_flag("--line-numbers, -l", line_flag, "disable line numbers from output")
      ->group("Formatting flags");

    uint8_t colour_flag;
    app
      .add_flag("--colours, -c", colour_flag, "disable colours from output")
      ->group("Formatting flags");

    uint8_t headers_flag;
    app
      .add_flag("--headers, -t", headers_flag, "disable headers from output")
      ->group("Formatting flags");

    std::vector<std::string> files;
    app
      .add_option("file, -f, --file", files, "path(s) to file(s)")
      ->expected(-1)
      ->delimiter(' ')
      ->required()
      ->group("REQUIRED");

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

    uint8_t flag_sum = 0;
    if (line_flag) flag_sum += FormatFlags::LineNumbers;
    if (colour_flag) flag_sum += FormatFlags::Colours;
    if (headers_flag) flag_sum += FormatFlags::Headers;

    config.update_config(flag_sum);

    return handle_file_list(config, list);
	}
}
