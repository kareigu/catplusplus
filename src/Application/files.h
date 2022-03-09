#ifndef __CATPP_FILES_H__
#define __CATPP_FILES_H__

#include <fmt/core.h>
#include <vector>
#include <string>

struct File {
  std::string path;
  bool exists;

  const std::string to_string() const { return path; }
};

struct FileList {
  bool all_exist;

  FileList(const std::vector<std::string>& paths);

  const std::string to_string() const;
  const std::string invalids_to_string() const;

  size_t size() const { return m_list.size(); }
  auto begin() const { return m_list.begin(); }
  auto end() const { return m_list.end(); }

private:
  std::vector<File> m_list;
};

#endif
