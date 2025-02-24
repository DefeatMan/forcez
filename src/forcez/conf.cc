#include "forcez/conf.h"

namespace forcez {

const bit7z::Bit7zLibrary Conf::defaultLib =
    bit7z::Bit7zLibrary(bit7z::kDefaultLibrary);

Conf &Conf::SetLib(const std::string &libPath) {
  if (!libPath.empty()) {
    lib7z_.reset(new bit7z::Bit7zLibrary(libPath));
  }
  return *this;
}

Conf &Conf::SetArcs(std::vector<std::string> arcs) {
  arcs_ = std::move(arcs);
  return *this;
}

Conf &Conf::SetChar(std::string ch) {
  char_ = std::move(ch);
  return *this;
}

Conf &Conf::SetJobs(int jobs) {
  jobs_ = jobs;
  return *this;
}

Conf &Conf::SetLength(int length) {
  length_ = length;
  return *this;
}

Conf &Conf::SetVerbose(bool verbose) {
  verbose_ = verbose;
  return *this;
}

const bit7z::Bit7zLibrary &Conf::Lib() const {
  if (!lib7z_) {
    return defaultLib;
  }
  return *lib7z_.get();
}

const std::vector<std::string> Conf::Arcs() const { return arcs_; }

const std::string Conf::Char() const { return char_; }

int Conf::Jobs() const { return jobs_; }

int Conf::Length() const { return length_; }

bool Conf::Verbose() const { return verbose_; }
}; // namespace forcez
