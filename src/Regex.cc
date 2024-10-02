#include "Regex.hpp"
#include "Matcher.hpp"
#include "Pattern.hpp"
#include <optional>
#include <stack>
#include <string>
#include <vector>

Regex::Regex(const std::string &pattern_string)
    : states(std::stack<MatchState>()) {
  auto pattern = Pattern(pattern_string);
  matchers = std::vector<Matcher>();
  while (true) {
    auto matcher = pattern.next_matcher();
    if (matcher)
      matchers.push_back(std::move(*matcher));
    else
      break;
  }
}

std::optional<RegexMatch> Regex::test(const std::string &input_line) {
  if (input_line.empty())
    return RegexMatch{.start = 0, .end = 0};

  for (int start = 0; start < input_line.length(); start++) {
    states.push(MatchState{.matcher_idx = 0, .line_offset = start});

    while (!states.empty()) {
      auto state = states.top();
      if (state.matcher_idx == matchers.size())
        return RegexMatch{.start = start, .end = state.line_offset};

      auto matcher = &matchers.at(state.matcher_idx);
      matcher->match(input_line, states);
    }
  }

  return std::nullopt;
}
