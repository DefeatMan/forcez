#include "bit7z/bit7zlibrary.hpp"
#include "utils/book.h"
#include "utils/book_handle.h"
#include "utils/forcez.h"
#include "utils/init.h"
#include <functional>
#include <string>

INITIALIZE_EASYLOGGINGPP

argparse::ArgumentParser global_parser("forcez", "0.0.1");
bit7z::Bit7zLibrary *forcez_7zlib = nullptr;

void ForceZip(const std::vector<FileBook> &file_book_vec,
              const std::string &file_path) {
  LOG(INFO) << "ForceZip " << file_path << " ...";
  for (const auto &book : file_book_vec) {
    BookHandle pw_book(book);
    Forcez forcez(std::move(pw_book), file_path, global_parser.get<int>("-j"));
    if (auto passwd = forcez.getResult(); passwd != forcez.emptyPassWd()) {
      LOG(INFO) << "Password: [" << passwd << "]";
    }
  }
}

auto MakeFileBookVec(const std::vector<std::string> &arg_book_vec) {
  std::vector<FileBook> res;
  res.reserve(arg_book_vec.size());
  for (const auto &book_path : arg_book_vec) {
    res.emplace_back(book_path);
  }
  return res;
}

int main(int argc, char *argv[]) {
  InitLogging();
  InitCmdParser(global_parser);
  global_parser.parse_args(argc, argv);
  forcez_7zlib = new bit7z::Bit7zLibrary(global_parser.get<std::string>("-7z"));
  const auto &arg_file_vec =
      global_parser.get<std::vector<std::string>>("string");
  const auto &arg_book_vec = global_parser.get<std::vector<std::string>>("-b");

  auto file_book_vec = MakeFileBookVec(arg_book_vec);
  for (const auto &file_path : arg_file_vec) {
    ForceZip(file_book_vec, file_path);
  }

  delete forcez_7zlib;
  return 0;
}
