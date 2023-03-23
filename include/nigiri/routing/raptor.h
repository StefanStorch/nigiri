#pragma once

#include <cinttypes>

#include "nigiri/routing/query.h"
#include "nigiri/routing/routing_time.h"

namespace nigiri {
struct timetable;
}

namespace nigiri::routing {

struct search_state;

struct stats {
  std::uint64_t n_routing_time_{0ULL};
  std::uint64_t n_footpaths_visited_{0ULL};
  std::uint64_t n_routes_visited_{0ULL};
  std::uint64_t n_earliest_trip_calls_{0ULL};
  std::uint64_t n_earliest_arrival_updated_by_route_{0ULL};
  std::uint64_t n_earliest_arrival_updated_by_footpath_{0ULL};
  std::uint64_t fp_update_prevented_by_lower_bound_{0ULL};
  std::uint64_t route_update_prevented_by_lower_bound_{0ULL};
  std::uint64_t lb_time_{0ULL};
  std::uint64_t fastest_direct_{0ULL};
  std::uint64_t search_iterations_{0ULL};
  std::uint64_t interval_extensions_{0ULL};
};

template <direction SearchDir, bool IntermodalTarget>
struct raptor {
  raptor(timetable const& tt, search_state& state, query q);
  void route();

  stats const& get_stats() const;

private:
  static constexpr auto const kFwd = (SearchDir == direction::kForward);
  static constexpr auto const kBwd = (SearchDir == direction::kBackward);

  bool is_ontrip() const;

  bool is_better(auto a, auto b);
  bool is_better_or_eq(auto a, auto b);
  auto get_best(auto a, auto b);

  routing_time time_at_stop(route_idx_t,
                            transport,
                            unsigned stop_idx,
                            event_type ev_type);

  routing_time time_at_stop(transport, unsigned stop_idx, event_type ev_type);

  std::string_view transport_name(transport_idx_t const t) const {
    return tt_
        .trip_display_names_
            [tt_.merged_trips_[tt_.transport_to_trip_section_[t].front()]
                 .front()]
        .view();
  }

  transport get_earliest_transport(unsigned const k,
                                   route_idx_t const r,
                                   unsigned const stop_idx,
                                   location_idx_t const l_idx);
  bool update_route(unsigned const k, route_idx_t const r);
  void update_footpaths(unsigned const k);

  unsigned end_k() const;

  void rounds();
  void force_print_state(char const* comment = "");
  void print_state(char const* comment = "");

  void reconstruct_for_destination(unixtime_t start_at_start,
                                   std::size_t dest_index,
                                   location_idx_t dest);
  void reconstruct(unixtime_t const start_at_start);

  void set_time_at_destination(unsigned round_k);

  timetable const& tt_;
  std::uint16_t n_days_;
  query q_;
  routing_time time_at_destination_{kInvalidTime<SearchDir>};
  search_state& state_;
  stats stats_;
};

}  // namespace nigiri::routing
