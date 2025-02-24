#include "forcez/worker.h"
#include "bit7z/bit7zlibrary.hpp"
#include "bit7z/bitarchivereader.hpp"
#include "bit7z/bitexception.hpp"
#include "bit7z/bitformat.hpp"
#include "forcez/argp.h"
#include "forcez/book.h"
#include "indicators/cursor_control.hpp"
#include "indicators/progress_bar.hpp"
#include "indicators/setting.hpp"

#include <atomic>
#include <chrono>
#include <filesystem>
#include <iterator>
#include <limits>
#include <memory>
#include <mutex>
#include <ostream>
#include <string_view>
#include <thread>
#include <type_traits>
#include <vector>

namespace forcez {

template <typename T,
          typename = std::enable_if_t<std::is_convertible_v<
              std::remove_cvref_t<T>, IBook<typename T::value_type>>>>
std::optional<std::string>
ExtractArchiveImpl(const std::string &arcfp, const T &book,
                   const std::string &bookfp = "<unknown> ") {
  std::mutex rl;
  std::optional<std::string> ret;
  std::vector<std::thread> jobs;
  std::atomic<size_t> prog;

  // Book
  for (int i = 0; i < Argp::Jobs(); ++i) {
    jobs.emplace_back([&]() {
      bit7z::BitArchiveReader arc(Argp::Bit7zLib(), arcfp,
                                  bit7z::BitFormat::Auto);
      auto n = book.ISize();
      auto curIdx = prog.fetch_add(1, std::memory_order::relaxed);
      while (curIdx < n) {
        auto pwd = book.IGet(curIdx);
        bool ok = true;
        arc.setPassword(pwd);
        try {
          auto items = arc.items();
          for (auto ii = items.begin(); ii != items.end(); ++ii) {
            if (ii->isEncrypted() && ii->packSize() > 0) {
              arc.testItem(ii - items.begin());
              break;
            }
          }
        } catch (const bit7z::BitException &e) {
          ok = false;
        }
        if (ok) {
          prog.store(n + Argp::Jobs(), std::memory_order::relaxed);
          std::lock_guard rlg(rl);
          ret.emplace(pwd);
          return;
        }
        curIdx = prog.fetch_add(1, std::memory_order::relaxed);
      }
    });
  }

  // Bar
  using namespace indicators;
  ProgressBar bar{option::BarWidth{50},
                  option::Start{"["},
                  option::Fill{"="},
                  option::Lead{">"},
                  option::Remainder{" "},
                  option::End{"]"},
                  option::PrefixText{bookfp},
                  option::ForegroundColor{Color::green},
                  option::ShowPercentage{true},
                  option::ShowElapsedTime{true},
                  option::ShowRemainingTime{true},
                  option::FontStyles{std::vector<FontStyle>{FontStyle::bold}}};
  jobs.emplace_back([&]() {
    auto n = book.ISize();
    auto curIdx = prog.load(std::memory_order::relaxed);
    auto jn = Argp::Jobs();
    while (curIdx < n + jn - 1) {
      std::this_thread::sleep_for(std::chrono::microseconds(200));
      if (curIdx < jn) {
        bar.set_option(option::PostfixText{"preparing.."});
        bar.set_progress(0);
      } else {
        auto ci = curIdx - jn;
        bar.set_option(option::PostfixText{book.IGet(ci)});
        bar.set_progress((ci + 1) * 100 / n);
      }
      curIdx = prog.load(std::memory_order::relaxed);
    }
  });

  // Join
  for (auto &job : jobs) {
    if (job.joinable())
      job.join();
  }

  std::lock_guard rlg(rl);
  bar.set_option(option::PostfixText{
      ret.has_value() ? std::string("[]").insert(1, ret.value())
                      : "-- no found --"});
  bar.set_progress(100);
  return ret;
}

std::optional<std::string> ExtractArchive() {
  auto arcs = Argp::Arcs();
  const auto &arcfp = arcs.back();
  std::vector<std::string> bookfps(arcs.begin(), arcs.end() - 1);
  if (bookfps.empty()) {
    bookfps.emplace_back("./passwd.txt");
  }
  for (const auto &bookfp : bookfps) {
    if (auto bookfs = std::filesystem::path(bookfp);
        std::filesystem::exists(bookfs) &&
        std::filesystem::is_regular_file(bookfs)) {
      if (auto ret = ExtractArchiveImpl(
              arcfp, TxtDict(bookfp),
              std::string("<> ").insert(1, bookfs.filename().string()));
          ret.has_value()) {
        return ret;
      }
    }
  }
  if (Argp::Length() > 0) {
    if (auto ret = ExtractArchiveImpl(
            arcfp, CharSet(Argp::Char(), Argp::Length()), "CharSet ");
        ret.has_value()) {
      return ret;
    }
  }
  return std::nullopt;
}

} // namespace forcez
