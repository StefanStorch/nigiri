#include "nigiri/loader/hrd/stamm/boarding_aid.h"

#include "nigiri/loader/hrd/util.h"

namespace nigiri::loader::hrd {

boarding_aid_t parse_boarding_aid(config const& c,
                                  dir const& d) {
  scoped_timer timer{"parse boarding aid"};

  boarding_aid_t boarding_aids;

  std::string_view file_content;
  file the_file;
  try {
    the_file = d.get_file(c.fplan_ / "boarding_aid.txt");
    file_content = the_file.data();
  } catch (...) {
    return boarding_aids;
  }
  utl::for_each_line_numbered(file_content, [&](utl::cstr line, unsigned const line_number) {
    boarding_aid current_aid{};
    if (std::count_if(line.begin(), line.end(), [&](const auto& item) {
          return item == ';';
        }) < 2) {
      for_each_token_numbered(line, ';', [&](utl::cstr day, unsigned const token_number) {
        if (token_number == 1) {
          current_aid.id_ = parse_eva_number(day);
          return;
        }
        for_each_token(day, '/', [&](utl::cstr token) {
          auto start = get_until(token, '-');
          auto end = token.substr(start.length() + 1);
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
    for_each_token_numbered(line, ';', [&](utl::cstr day, unsigned const day_number) {
      if (day_number == 1) {
        current_aid.id_ = parse_eva_number(day);
        return;
      }
      if (day.length()<1) {
        return;
      }
      for_each_token(day, '/', [&](utl::cstr token) {
        auto start = get_until(token, '-');
        auto end = token.substr(start.length() + 1);
        auto start_time = hhmm_to_min(atoi(start.c_str()));
        auto end_time = hhmm_to_min(atoi(end.c_str()));
        current_aid.add_time_window(std::chrono::weekday((day_number-1)%7), duration_t(start_time), duration_t(end_time));
      });
    });
    boarding_aids[current_aid.id_] = current_aid;
  });
  std::clog << "Found Boarding Aids for " << boarding_aids.size() << " Stations!\n";
  return boarding_aids;
}

} // namespace nigiri::loader::hrd