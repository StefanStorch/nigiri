#pragma once

#include <chrono>
#include <set>
#include <vector>

#include "cista/reflection/comparable.h"

#include "nigiri/routing/query.h"
#include "nigiri/timetable.h"
#include "nigiri/types.h"

namespace nigiri::routing {

struct start {
  CISTA_COMPARABLE()
  unixtime_t time_at_start_;
  unixtime_t time_at_stop_;
  location_idx_t stop_;
};

void get_starts(direction,
                timetable const&,
                variant<unixtime_t, interval<unixtime_t>> const& start_time,
                std::vector<offset> const& station_offsets,
                location_match_mode,
                bool use_start_footpaths,
                std::vector<start>&,
                bool add_ontrip,
                int profile=0,
                bool wheelchair_profile=false);                

void collect_destinations(timetable const&,
                          std::vector<std::vector<offset>> const& destinations,
                          location_match_mode const,
                          std::vector<std::set<location_idx_t>>&,
                          std::vector<bool>& is_destination);

}  // namespace nigiri::routing
