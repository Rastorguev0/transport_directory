#include "utils.h"

#include <cctype>

using namespace std;

string_view Strip(string_view line) {
  while (!line.empty() && isspace(line.front())) {
    line.remove_prefix(1);
  }
  while (!line.empty() && isspace(line.back())) {
    line.remove_suffix(1);
  }
  return line;
}

int ConvertToMinutes(int day, int hours, int minutes) {
  return day * 24 * 60 + hours * 60 + minutes;
}