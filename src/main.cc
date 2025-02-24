#include "argparse/argparse.hpp"
#include "config.h"
#include "forcez/forcez.h"

int main(int argc, char *argv[]) {
  argparse::ArgumentParser p("forcez", PROJECT_VERSION_STRING);
  p.add_description("Force to Extract ARCHIVE by PWD_TXT_BOOK");
  p.add_argument("-7z")
      .help("include 7z dll path")
      .default_value<std::string>("");
  p.add_argument("-j")
      .help("allow N jobs at once")
      .default_value<int>(1)
      .scan<'i', int>();
  p.add_argument("-v", "--verbose")
      .help("print verbose info")
      .default_value(false)
      .implicit_value(true)
      .nargs(0);
  p.add_argument("-l", "--length")
      .help("maximum length of passwd generator")
      .default_value<int>(0)
      .scan<'i', int>();
  p.add_argument("-c", "--char")
      .help("charset of passwd generator")
      .default_value("1234567890")
      .nargs(argparse::nargs_pattern::optional);
  p.add_argument("arcs")
      .metavar("FILES")
      .help("[PWD_TXT_BOOK...] ARCHIVE")
      .nargs(argparse::nargs_pattern::at_least_one);

  p.parse_args(argc, argv);

  forcez::Conf c;
  c.SetLib(p.get<std::string>("-7z"))
      .SetArcs(p.get<std::vector<std::string>>("arcs"))
      .SetChar(p.get<std::string>("-c"))
      .SetJobs(p.get<int>("-j"))
      .SetLength(p.get<int>("-l"))
      .SetVerbose(p.get<bool>("-v"));

  forcez::ExtractArchive(c);
  return 0;
}
