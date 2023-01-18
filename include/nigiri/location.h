#pragma once

#include <iosfwd>

#include "geo/latlng.h"

#include "nigiri/common/it_range.h"
#include "nigiri/footpath.h"
#include "nigiri/types.h"

namespace nigiri {

struct timetable;

struct location {
  //TODO: Was soll location genau aussagen? Eventuell footpaths ändern und profile Informationen hinzufügen
  friend std::ostream& operator<<(std::ostream&, location const&);
  location(timetable const&, location_idx_t);
  location(std::string_view id,
           std::string_view name,
           geo::latlng pos,
           source_idx_t,
           location_type,
           osm_node_id_t,
           location_idx_t parent,
           timezone_idx_t,
           duration_t transfer_time,
           it_range<vector<location_idx_t>::const_iterator> equivalences,
           it_range<vector<footpath>::const_iterator> footpaths_in,
           it_range<vector<footpath>::const_iterator> footpaths_out);
  std::string_view id_;
  std::string_view name_;
  geo::latlng pos_;
  source_idx_t src_;
  location_type type_;
  osm_node_id_t osm_id_;
  location_idx_t parent_;
  timezone_idx_t timezone_idx_;
  duration_t transfer_time_;
  it_range<vector<location_idx_t>::const_iterator> equivalences_;
  it_range<vector<footpath>::const_iterator> footpaths_out_, footpaths_in_;
};

}  // namespace nigiri
