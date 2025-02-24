#pragma once

#include <atomic>
#include <functional>
#include <memory>

template <typename Book> class BookHandle {
private:
  std::reference_wrapper<const Book> book_;
  std::atomic<int> cursor_;

public:
  BookHandle(const Book &book) : book_(std::ref(book)) {
    cursor_.store(0, std::memory_order::memory_order_relaxed);
  }

  BookHandle(const BookHandle &other) : book_(other.book_) {
    cursor_.store(other.cursor_.load(std::memory_order::memory_order_relaxed),
                  std::memory_order::memory_order_relaxed);
  }

  BookHandle(BookHandle &&other) : book_(other.book_) {
    cursor_.store(other.cursor_.load(std::memory_order::memory_order_relaxed),
                  std::memory_order::memory_order_relaxed);
  }

  const auto &emptyPassWd() const { return book_.get().emptyPassWd(); }

  const std::string &getPassWd() {
    auto c = cursor_.fetch_add(1, std::memory_order::memory_order_relaxed);
    return book_.get().getPassWd(c);
  }

  auto size() { return book_.get().size(); }

  void setNewBook(const Book &book) {
    book_ = std::ref(book);
    cursor_.store(0, std::memory_order::memory_order_relaxed);
  }

  void setPassword(const std::string &passwd) {
    if (passwd != emptyPassWd()) {
      cursor_.store(size(), std::memory_order::memory_order_relaxed);
    }
  }
};
