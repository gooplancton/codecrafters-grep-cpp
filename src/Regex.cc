#include "Regex.hpp"
#include "Matcher.hpp"
#include "Pattern.hpp"
#include <stack>
#include <string>
#include <vector>

Regex::Regex(const std::string &pattern_string) : states(std::stack<MatchState>()) {
  auto pattern = Pattern(pattern_string);
  matchers = std::vector<Matcher>();
  while (true) {
    auto matcher = pattern.next_matcher();
    if (matcher) matchers.push_back(*matcher);
    else break;
  }
}

bool Regex::test(const std::string &input_line) {
  if (input_line.empty())
    return true;

  for (int offset = input_line.length() - 1; offset >= 0; offset--) {
    states.push(MatchState{.matcher_idx = 0, .line_offset = offset});
  }

  while (!states.empty()) {
    auto state = states.top();
    if (state.matcher_idx == matchers.size())
      return true;

    auto matcher = matchers.at(state.matcher_idx);
    matcher.match(input_line, states);
  }

  return false;
}

