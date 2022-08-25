#include "nigiri/routing/journey.h"

#include "utl/enumerate.h"
#include "utl/overloaded.h"

#include "nigiri/common/indent.h"
#include "nigiri/print_transport.h"

namespace nigiri::routing {

void journey::leg::print(std::ostream& out,
                         nigiri::timetable const& tt,
                         unsigned const n) const {
  uses_.apply(utl::overloaded{[&](transport_enter_exit const& t) {
                                print_transport(tt, out, t.t_, t.stop_range_, n,
                                                true);
                              },
                              [&](footpath_idx_t const) {
                                indent(out, 1);
                                out << "FOOTPATH\n";
                              },
                              [&](std::uint8_t const x) {
                                indent(out, 1);
                                out << "MUMO ID " << x << "\n";
                              }});
}

void journey::print(std::ostream& out, nigiri::timetable const& tt) const {
  if (legs_.empty()) {
    out << "no legs\n";
    return;
  }

  out << "[" << start_time_ << ", " << dest_time_ << "]\n";
  out << "TRANSFERS: " << static_cast<int>(transfers_) << "\n";
  out << "     FROM: " << location{tt, legs_.front().from_} << " ["
      << legs_.front().dep_time_ << "]\n";
  out << "       TO: " << location{tt, legs_.back().to_} << " ["
      << legs_.back().arr_time_ << "]\n";
  for (auto const [i, l] : utl::enumerate(legs_)) {
    out << "leg " << i << ": " << location{tt, l.from_} << " [" << l.dep_time_
        << "] -> " << location{tt, l.to_} << " [" << l.arr_time_ << "]\n";
    l.print(out, tt, 1);
  }
}

}  // namespace nigiri::routing