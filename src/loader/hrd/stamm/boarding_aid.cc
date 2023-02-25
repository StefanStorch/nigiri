#include "nigiri/loader/hrd/stamm/boarding_aid.h"

#include "nigiri/loader/hrd/util.h"

namespace nigiri::loader::hrd {

boarding_aid_t parse_boarding_aid(config const& c,
                                  dir const& d) {
  scoped_timer timer{"parse boarding aid"};

  boarding_aid_t boarding_aids;

  std::string_view file_content;
  try {
    file_content = d.get_file(c.core_data_ / "boarding_aid.csv").data();
  } catch (...) {
    return boarding_aids;
  }

  utl::for_each_line_numbered(file_content, [&](utl::cstr line, unsigned const line_number) {
    if (line_number == 0) {
      return;
    }
    boarding_aid current_aid{};
    if (std::count_if(line.begin(), line.end(), [&](const auto& item) {
          return item == ';';
        }) < 2) {
      for_each_token_numbered(line, ';', [&](utl::cstr day, unsigned const token_number) {
        if (token_number == 0) {
          current_aid.id_ = parse_eva_number(day.c_str());
        }
        for_each_token(day, '/', [&](utl::cstr token) {
          auto start = get_until(token, '-');
          auto end = token.substr(start.length());
          auto start_time = hhmm_to_min(atoi(start.c_str()));
          auto end_time = hhmm_to_min(atoi(end.c_str()));
          for (int i = 0; i < 7; ++i) {
            current_aid.add_time_window(std::chrono::weekday(i),
                                        duration_t(start_time),
                                        duration_t(end_time));
          }
        });
      });
      boarding_aids[current_aid.id_] = current_aid;
      return;
    }
    for_each_token_numbered(line, ';', [&](utl::cstr day, unsigned const token_number) {
      if (token_number == 0) {
        current_aid.id_ = parse_eva_number(day.c_str());
      }
      for_each_token(day, '/', [&](utl::cstr token) {
        auto start = get_until(token, '-');
        auto end = token.substr(start.length());
        auto start_time = hhmm_to_min(atoi(start.c_str()));
        auto end_time = hhmm_to_min(atoi(end.c_str()));
        current_aid.add_time_window(std::chrono::weekday(token_number), duration_t(start_time), duration_t(end_time));
      });
    });
    boarding_aids[current_aid.id_] = current_aid;
  });

  return boarding_aids;
}

} // namespace nigiri::loader::hrd