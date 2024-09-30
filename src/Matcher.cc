#include "Matcher.hpp"
#include <algorithm>
#include <stack>
#include <string>

int Matcher::match_once(const std::string &input_line, int from_offset) {
  return std::visit(
      [&input_line, &from_offset](auto &&arg) -> int {
        return arg.match_once(input_line, from_offset);
      },
      character_class);
}

void Matcher::match(const std::string &input_line,
                    std::stack<MatchState> &states) {
  auto state = states.top();
  states.pop();
  if (state.line_offset > input_line.length())
    return;

  switch (quantifier) {

  case Quantifier::ZeroOrOne:
    states.push(MatchState{.matcher_idx = state.matcher_idx + 1,
                           .line_offset = state.line_offset});
  case Quantifier::ExactlyOne: {
    int offset_shift = match_once(input_line, state.line_offset);
    if (offset_shift == -1)
      break;

    int new_line_offset = state.line_offset + offset_shift;
    if (new_line_offset > input_line.length())
      break;

    states.push(MatchState{
        .matcher_idx = state.matcher_idx + 1,
        .line_offset = state.line_offset + offset_shift,
    });

    break;
  }

  case Quantifier::ZeroOrMore:
    states.push(MatchState{.matcher_idx = state.matcher_idx + 1,
                           .line_offset = state.line_offset});
  case Quantifier::OneOrMore:
    int offset_shift = 0;
    while (true) {
      int increment = match_once(input_line, state.line_offset + offset_shift);

      if (increment == -1)
        break;

      offset_shift += increment;
      int new_line_offset = state.line_offset + offset_shift;
      if (new_line_offset > input_line.length())
        break;

      states.push(MatchState({.matcher_idx = state.matcher_idx + 1,
                              .line_offset = new_line_offset}));
    };
  }
}

int LiteralCharacter::match_once(const std::string &input_line,
                                 int from_offset) {
  if (input_line.at(from_offset) == character)
    return 1;

  return -1;
}

int PositiveCharacterGroup::match_once(const std::string &input_line,
                                       int from_offset) {

  bool matches = std::any_of(characters, characters + len,
                             [&input_line, &from_offset](char character) {
                               return input_line.at(from_offset) == character;
                             });

  return matches ? 1 : -1;
}

int NegativeCharacterGroup::match_once(const std::string &input_line,
                                       int from_offset) {
  bool matches = std::all_of(characters, characters + len,
                             [&input_line, &from_offset](char character) {
                               return input_line.at(from_offset) != character;
                             });

  return matches ? 1 : -1;
}

int Digit::match_once(const std::string &input_line, int from_offset) {
  if (std::isdigit(input_line.at(from_offset)))
    return 1;

  return -1;
}

int Alphanumeric::match_once(const std::string &input_line, int from_offset) {
  if (std::isalnum(input_line.at(from_offset)))
    return 1;

  return -1;
}

int StartOfString::match_once(const std::string &input_line, int from_offset) {
  return from_offset == 0 ? 0 : -1;
}

int EndOfString::match_once(const std::string &input_line, int from_offset) {
  return from_offset == input_line.length() ? 0 : -1;
}

int Wildcard::match_once(const std::string &input_line, int from_offset) {
  return 1;
}
