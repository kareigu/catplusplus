#include "Application/files.h"
#include <filesystem>


FileList::FileList(const std::vector<std::string>& paths) {
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

  all_exist = exists; 
  m_list = list;
}


const std::string FileList::to_string() const {
  std::string files = "";
  for (const auto& file : m_list) {
    std::string start = files.empty() ? "" : ", ";
    files.append(fmt::format("{}{}", start, file.to_string()));
  }
  return files;
}

const std::string FileList::invalids_to_string() const {
  std::string invalid_files = "";
  for (const auto& file : m_list) {
    if (!file.exists) {
      std::string start = invalid_files.empty() ? "" : ", ";
      invalid_files.append(fmt::format("{}{}", start, file.to_string()));
    }
  }
  return invalid_files;
}