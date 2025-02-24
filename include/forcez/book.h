#ifndef _FORCEZ_BOOK_
#define _FORCEZ_BOOK_

#include <string>
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
  const static int kMaxLength;
  std::vector<std::string> sto_;

public:
  TxtDict(const std::string &txtFs);
  TxtDict(const TxtDict &other);
  TxtDict(TxtDict &&other);
  size_t Size() const;
  const std::string &Get(int index) const;
};

class CharSet : public IBook<CharSet> {
private:
  std::string chars_;
  size_t len_;
  size_t size_;

public:
  CharSet(const std::string &chars, size_t len);
  CharSet(const CharSet &other);
  CharSet(CharSet &&other);
  size_t Size() const;
  std::string Get(int idx) const;
};

} // namespace forcez

#endif
