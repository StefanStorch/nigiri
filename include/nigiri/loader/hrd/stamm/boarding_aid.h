#pragma once

#include <string_view>

#include "geo/latlng.h"

#include "nigiri/loader/hrd/eva_number.h"
#include "nigiri/loader/hrd/parser_config.h"
#include "nigiri/loader/hrd/stamm/timezone.h"
#include "nigiri/timetable.h"
#include "nigiri/types.h"
#include "nigiri/loader/dir.h"

namespace nigiri::loader::hrd {

struct boarding_aid {
  explicit boarding_aid(eva_number id) : id_(id) {
    while(times_.size() < 7) {
      times_.emplace_back();
    }
  }
  boarding_aid() = default;

  struct time_window {
    bool during(duration_t time) const {
      return start_ < time && time < end_;
    }
    duration_t start_; // mam local time
    duration_t end_; // mam local time
  };

  struct time_windows {
    void add(time_window window) {
      windows_.emplace_back(window);
    }

    bool during(duration_t time) const {
      return std::any_of(windows_.begin(), windows_.end(), [&](auto const& window) {
        return window.during(time);
      });
    }

    vector<time_window> windows_;
  };


  bool empty() const {
    return times_.empty();
  }

  time_windows get_time_windows(std::chrono::weekday day) const {
    return times_[day.c_encoding()];
  }

  void add_time_window(std::chrono::weekday day, duration_t start, duration_t end) {
    times_[day.c_encoding()].add(time_window{start, end});
  }

  void add_time_window(std::chrono::weekday day, time_window window) {
    times_[day.c_encoding()].add(window);
  }

  eva_number id_{-1};

private:
  vector<time_windows> times_{7};
};

using boarding_aid_t = hash_map<eva_number, boarding_aid>;

boarding_aid_t parse_boarding_aid(config const& c,
                        dir const& d);

}  // namespace nigiri::loader::hrd
