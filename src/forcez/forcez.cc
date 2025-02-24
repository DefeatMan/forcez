#include "forcez/forcez.h"
#include "bit7z/bit7zlibrary.hpp"
#include "bit7z/bitarchivereader.hpp"
#include "bit7z/bitexception.hpp"
#include "bit7z/bitformat.hpp"
#include "forcez/book.h"
#include "indicators/cursor_control.hpp"
#include "indicators/progress_bar.hpp"
#include "indicators/setting.hpp"

#include <atomic>
#include <chrono>
#include <filesystem>
#include <limits>
#include <memory>
#include <mutex>
#include <thread>
#include <type_traits>
#include <vector>

namespace forcez {

template <typename T,
          typename = std::enable_if_t<std::is_convertible_v<
              std::remove_cvref_t<T>, IBook<typename T::value_type>>>>
std::optional<std::string>
ExtractArchiveImpl(const bit7z::Bit7zLibrary &lib, int jobs,
                   const std::string &arcPath, const T &book,
                   const std::string &bookPath = "<unknown> ") {
  std::mutex rl;
  std::optional<std::string> ret;
  std::vector<std::thread> jobThs;
  std::atomic<size_t> prog;

  // Book
  for (int i = 0; i < jobs; ++i) {
    jobThs.emplace_back([&]() {
      bit7z::BitArchiveReader arc(lib, arcPath, bit7z::BitFormat::Auto);
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
          prog.store(n + jobs, std::memory_order::relaxed);
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
                  option::PrefixText{bookPath},
                  option::ForegroundColor{Color::green},
                  option::ShowPercentage{true},
                  option::ShowElapsedTime{true},
                  option::ShowRemainingTime{true},
                  option::FontStyles{std::vector<FontStyle>{FontStyle::bold}}};
  jobThs.emplace_back([&]() {
    auto n = book.ISize();
    auto curIdx = prog.load(std::memory_order::relaxed);
    while (curIdx < n + jobs - 1) {
      std::this_thread::sleep_for(std::chrono::microseconds(200));
      if (curIdx < jobs) {
        bar.set_option(option::PostfixText{"preparing.."});
        bar.set_progress(0);
      } else {
        auto ci = curIdx - jobs;
        bar.set_option(option::PostfixText{book.IGet(ci)});
        bar.set_progress((ci + 1) * 100 / n);
      }
      curIdx = prog.load(std::memory_order::relaxed);
    }
  });

  // Join
  for (auto &job : jobThs) {
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

std::optional<std::string> ExtractArchive(const Conf &conf) {
  auto arcs = conf.Arcs();
  const auto &arcPath = arcs.back();
  std::vector<std::string> bookPaths(arcs.begin(), arcs.end() - 1);
  if (bookPaths.empty()) {
    bookPaths.emplace_back("./passwd.txt");
  }
  for (const auto &bookPath : bookPaths) {
    if (auto bfs = std::filesystem::path(bookPath);
        std::filesystem::exists(bfs) && std::filesystem::is_regular_file(bfs)) {
      if (auto ret = ExtractArchiveImpl(
              conf.Lib(), conf.Jobs(), arcPath, TxtDict(bookPath),
              std::string("<> ").insert(1, bfs.filename().string()));
          ret.has_value()) {
        return ret;
      }
    }
  }
  if (conf.Length() > 0) {
    if (auto ret =
            ExtractArchiveImpl(conf.Lib(), conf.Jobs(), arcPath,
                               CharSet(conf.Char(), conf.Length()), "CharSet ");
        ret.has_value()) {
      return ret;
    }
  }
  return std::nullopt;
}

} // namespace forcez
