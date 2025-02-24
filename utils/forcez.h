#pragma once

#include "bit7z/bit7zlibrary.hpp"
#include "bit7z/bitarchivereader.hpp"
#include "bit7z/bitexception.hpp"
#include <functional>
#include <future>
#include <string>
#include <vector>

extern bit7z::Bit7zLibrary *forcez_7zlib;

template <typename PWBook> class Forcez {
private:
  std::vector<std::future<std::string>> th_lst_;
  PWBook pw_book_;
  const std::string f_path_;
  std::string passwd_;

private:
  std::string worker() {
    bit7z::BitArchiveReader arc{*forcez_7zlib, f_path_, bit7z::BitFormat::Auto};
    while (true) {
      const auto &pw = pw_book_.getPassWd();
      if (pw == pw_book_.emptyPassWd()) {
        return pw;
      }
      arc.setPassword(pw);
      try {
        arc.test();
      } catch (const bit7z::BitException &ex) {
        continue;
      }

      pw_book_.setPassword(pw);
      return pw;
    }
  }

public:
  Forcez(PWBook &&pw_book, const std::string &file_path, int thread_num = 1)
      : pw_book_(std::forward<PWBook>(pw_book)), f_path_(file_path),
        passwd_(pw_book_.emptyPassWd()) {
    thread_num = std::max(thread_num, 1);
    th_lst_.resize(thread_num);
    auto func = std::bind(&Forcez::worker, this);
    for (int i = 0; i < thread_num; ++i) {
      th_lst_.emplace_back(std::async(std::launch::async, func));
    }
  }

  ~Forcez() {
    for (auto &fu : th_lst_) {
      if (fu.valid()) {
        fu.wait();
      }
    }
  }

  const auto &emptyPassWd() const { return pw_book_.emptyPassWd(); }

  std::string getResult() {
    if (passwd_ != emptyPassWd()) {
      return passwd_;
    }
    for (auto &fu : th_lst_) {
      if (fu.valid()) {
        if (const auto &pwd = fu.get(); pwd != emptyPassWd()) {
          passwd_ = pwd;
          return passwd_;
        }
      }
    }
    return emptyPassWd();
  }

  void createNewThread() {
    th_lst_.emplace_back(
        std::async(std::launch::async, std::bind(&Forcez::worker, this)));
  }
};
