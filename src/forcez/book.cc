#include "forcez/book.h"

#include <cmath>
#include <fstream>

namespace forcez {

const int TxtDict::kMaxLength = 300;

TxtDict::TxtDict(const std::string &txtFs) {
  std::ifstream fin(txtFs);
  char st[kMaxLength];
  while (fin.getline(st, kMaxLength, '\n')) {
    sto_.emplace_back(st);
  }
}

TxtDict::TxtDict(const TxtDict &other) : sto_(other.sto_) {}

TxtDict::TxtDict(TxtDict &&other) : sto_(std::move(other.sto_)) {}

size_t TxtDict::Size() const { return sto_.size(); }

const std::string &TxtDict::Get(int index) const { return sto_[index]; }

CharSet::CharSet(const std::string &chars, size_t len)
    : chars_(chars), len_(len),
      size_((chars_.size() > 0 && len_ > 0)
                ? ((std::pow(chars_.size(), len_ + 1) - chars_.size()) /
                   (chars_.size() - 1))
                : 0) {}

CharSet::CharSet(const CharSet &other)
    : chars_(other.chars_), len_(other.len_), size_(other.size_) {}

CharSet::CharSet(CharSet &&other)
    : chars_(std::move(other.chars_)), len_(other.len_), size_(other.size_) {}

size_t CharSet::Size() const { return size_; }

std::string CharSet::Get(int idx) const {
  auto tot = chars_.size();
  char pwd[20];
  int i = 0;
  while (idx >= 0) {
    pwd[i++] = chars_[idx % tot];
    idx /= tot;
    --idx;
  }
  pwd[i] = '\0';
  return pwd;
}

} // namespace forcez
