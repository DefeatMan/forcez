#pragma once

#include <fstream>
#include <string>
#include <vector>

class FileBook {
private:
  const static inline int kPassWdSize = 300;
  const static inline std::string kEmptyPassWd = "";
  std::vector<std::string> passwd_lst_;

public:
  FileBook(const std::string &book_file) {
    std::ifstream fin(book_file);
    char pwd[kPassWdSize];
    while (fin.getline(pwd, kPassWdSize, '\n')) {
      passwd_lst_.emplace_back(pwd);
    }
  }

  FileBook(FileBook &&other) { passwd_lst_ = std::move(other.passwd_lst_); }

  auto size() const { return passwd_lst_.size(); }

  static const auto &emptyPassWd() { return kEmptyPassWd; }

  const std::string &getPassWd(int index) const {
    if (index < size()) {
      return passwd_lst_[index];
    }
    return emptyPassWd();
  }
};


