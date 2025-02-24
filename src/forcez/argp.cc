#include "forcez/argp.h"
#include "argparse/argparse.hpp"

namespace forcez {

const std::string Argp::version = "0.0.1";

Argp::Argp() : parser_("forcez", version), lib7z_(nullptr) {
  parser_.add_description("Force to Extract ARCHIVE by PWD_TXT_BOOK");
  parser_.add_argument("-7z")
      .help("include 7z dll path")
      .default_value<std::string>("");
  parser_.add_argument("-j")
      .help("allow N jobs at once")
      .default_value<int>(1)
      .scan<'i', int>();
  parser_.add_argument("-v", "--verbose")
      .help("print verbose info")
      .default_value(false)
      .implicit_value(true)
      .nargs(0);
  parser_.add_argument("-l", "--length")
      .help("maximum length of passwd generator")
      .default_value<int>(0)
      .scan<'i', int>();
  parser_.add_argument("-c", "--char")
      .help("charset of passwd generator")
      .default_value("1234567890")
      .nargs(argparse::nargs_pattern::optional);
  parser_.add_argument("arcs")
      .metavar("FILES")
      .help("[PWD_TXT_BOOK...] ARCHIVE")
      .nargs(argparse::nargs_pattern::at_least_one);
}

Argp &Argp::conf() {
  static Argp ins;
  return ins;
}

argparse::ArgumentParser &Argp::Conf() { return conf().parser_; }

void Argp::Init(int argc, char *argv[]) {
  Conf().parse_args(argc, argv);
  auto lib7z_fp = Conf().get<std::string>("-7z");
  conf().lib7z_ = new bit7z::Bit7zLibrary(
      lib7z_fp.empty() ? bit7z::kDefaultLibrary : lib7z_fp);
}

int Argp::Jobs() { return Conf().get<int>("-j"); }

bool Argp::Verbose() { return Conf().get<bool>("-v"); }

int Argp::Length() { return Conf().get<int>("-l"); }

std::string Argp::Char() { return Conf().get<std::string>("-c"); }

std::vector<std::string> Argp::Arcs() {
  return Conf().get<std::vector<std::string>>("arcs");
}

const bit7z::Bit7zLibrary &Argp::Bit7zLib() {
  if (conf().lib7z_ == nullptr) {
    throw std::invalid_argument("7z shared libraries not found");
  }
  return *conf().lib7z_;
}

} // namespace forcez
