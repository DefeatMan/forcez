#pragma once

#include <cmath>
#include <fstream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace forcez {

template <typename Derived> class IBook {
private:
  const static inline std::string kNil = "";

public:
  using value_type = Derived;

  static const std::string &Nil() { return kNil; }

  auto IGet(size_t idx) const {
    auto d = static_cast<const Derived *>(this);
    if (idx < d->Size()) {
      return d->Get(idx);
    } else {
      return kNil;
    }
  }

  auto ISize() const { return static_cast<const Derived *>(this)->Size(); }
};

class TxtDict : public IBook<TxtDict> {
private:
  const static inline int kMaxLength = 300;
  std::vector<std::string> sto_;

public:
  TxtDict(const std::string &txtFs) {
    std::ifstream fin(txtFs);
    char st[kMaxLength];
    while (fin.getline(st, kMaxLength, '\n')) {
      sto_.emplace_back(st);
    }
  }

  TxtDict(const TxtDict &other) : sto_(other.sto_) {}

  TxtDict(TxtDict &&other) : sto_(std::move(other.sto_)) {}

  auto Size() const { return sto_.size(); }

  const std::string &Get(int index) const { return sto_[index]; }
};

class CharSet : public IBook<CharSet> {
private:
  std::string chars_;
  size_t len_;
  size_t size_;

public:
  CharSet(const std::string &chars, size_t len)
      : chars_(chars), len_(len),
        size_((chars_.size() > 0 && len_ > 0)
                  ? ((std::pow(chars_.size(), len_ + 1) - chars_.size()) /
                     (chars_.size() - 1))
                  : 0) {}

  CharSet(const CharSet &other)
      : chars_(other.chars_), len_(other.len_), size_(other.size_) {}

  CharSet(CharSet &&other)
      : chars_(std::move(other.chars_)), len_(other.len_), size_(other.size_) {}

  auto Size() const { return size_; }

  std::string Get(int idx) const {
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
};

} // namespace forcez
