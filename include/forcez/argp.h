#ifndef _FORCEZ_ARGP_
#define _FORCEZ_ARGP_

#include "argparse/argparse.hpp"
#include "bit7z/bit7zlibrary.hpp"

namespace forcez {

class Argp {
private:
  argparse::ArgumentParser parser_;
  bit7z::Bit7zLibrary *lib7z_;
  Argp();
  Argp(const Argp &) = delete;
  Argp &operator=(const Argp &) = delete;
  static Argp &conf();
  static argparse::ArgumentParser &Conf();

public:
  static const std::string version;
  static void Init(int argc, char *argv[]);
  static int Jobs();
  static bool Verbose();
  static int Length();
  static std::string Char();
  static std::vector<std::string> Arcs();
  static const bit7z::Bit7zLibrary &Bit7zLib();
};

} // namespace forcez

#endif
