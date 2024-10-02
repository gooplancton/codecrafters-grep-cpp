#ifndef REGEX_H
#define REGEX_H

#include "Matcher.hpp"
#include <optional>
#include <stack>
#include <string>
#include <vector>

struct RegexMatch {
  int start;
  int end;
};

class Regex {
private:
  std::vector<Matcher> matchers;
  std::stack<MatchState> states;

public:
  Regex(const std::string &pattern_string);

  std::optional<RegexMatch> test(const std::string &input_line);
};

#endif // REGEX_H
