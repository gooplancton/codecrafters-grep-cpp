#ifndef REGEX_H
#define REGEX_H

#include "Matcher.hpp"
#include <stack>
#include <string>
#include <vector>

class Regex {
private:
  std::vector<Matcher> matchers;
  std::stack<MatchState> states;

public:
  Regex(const std::string &pattern_string);

  bool test(const std::string &input_line);
};

#endif // REGEX_H
