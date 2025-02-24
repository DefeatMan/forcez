#pragma once

#include "argparse/argparse.hpp"
#include "easylogging++.h"

void InitCmdParser(argparse::ArgumentParser &parser) {
  parser.add_argument("-7z")
      .help("Include 7z dll path")
      .default_value<std::string>("./7z.dll");
  parser.add_argument("-j")
      .help("Allow N jobs at once")
      .default_value<int>(1)
      .scan<'i', int>();
  parser.add_argument("string").help("Zip file path to forcez").append();
  parser.add_argument("-b", "--book")
      .help("Include passwd book path")
      .default_value<std::vector<std::string>>({"./passwd.txt"})
      .append();
}

void InitLogging() {
  el::Configurations conf;
  conf.setToDefault();
  conf.set(el::Level::Global, el::ConfigurationType::Enabled, "true");
  conf.set(el::Level::Global, el::ConfigurationType::ToStandardOutput, "true");
  conf.set(el::Level::Global, el::ConfigurationType::ToFile, "true");
  conf.set(el::Level::Global, el::ConfigurationType::Filename, "forcez.log");
  conf.set(el::Level::Global, el::ConfigurationType::MillisecondsWidth, "3");
  conf.set(el::Level::Global, el::ConfigurationType::LogFlushThreshold, "0");
  conf.set(el::Level::Global, el::ConfigurationType::Format,
           "%datetime{%H:%m:%s} %msg");
  conf.set(el::Level::Debug, el::ConfigurationType::Format,
           "%datetime{%H:%m:%s} %loc %msg");
  conf.set(el::Level::Warning, el::ConfigurationType::Format, "%msg");
  el::Loggers::addFlag(el::LoggingFlag::ColoredTerminalOutput);
  el::Loggers::reconfigureAllLoggers(conf);
}
