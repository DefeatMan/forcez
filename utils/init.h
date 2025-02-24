#pragma once

#include "argparse/argparse.hpp"
#include "spdlog/spdlog.h"

void InitCmdParser(argparse::ArgumentParser &parser) {
  parser.add_description("Force to extract zipfiles by password dictionaries");
  parser.add_argument("-7z")
      .help("include 7z dll path")
      .default_value<std::string>("./7z.dll");
  parser.add_argument("-j")
      .help("allow N jobs at once")
      .default_value<int>(1)
      .scan<'i', int>();
  parser.add_argument("-v", "--verbose")
      .help("print verbose info")
      .default_value(false)
      .implicit_value(true)
      .nargs(0);
  parser.add_argument("files")
      .metavar("[FILE]...")
      .help("zipfile/path to extract")
      .append();
  parser.add_argument("-b", "--book")
      .help("include passwd book path")
      .default_value<std::vector<std::string>>({"./passwd.txt"})
      .append();
}

void InitLogging() {}
