#ifndef _FORCEZ_CONF_
#define _FORCEZ_CONF_

#include "bit7z/bit7zlibrary.hpp"
#include <memory>
#include <string>
#include <vector>

namespace forcez {

class Conf {
private:
  const static bit7z::Bit7zLibrary defaultLib;

  std::unique_ptr<bit7z::Bit7zLibrary> lib7z_;
  std::vector<std::string> arcs_;
  std::string char_;
  int jobs_;
  int length_;
  bool verbose_;

public:
  Conf &SetLib(const std::string &libPath);
  Conf &SetArcs(std::vector<std::string> arcs);
  Conf &SetChar(std::string ch);
  Conf &SetJobs(int jobs);
  Conf &SetLength(int length);
  Conf &SetVerbose(bool verbose);

  const bit7z::Bit7zLibrary &Lib() const;
  const std::vector<std::string> Arcs() const;
  const std::string Char() const;
  int Jobs() const;
  int Length() const;
  bool Verbose() const;
};

} // namespace forcez

#endif
