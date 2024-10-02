#include "Pattern.hpp"
#include "Matcher.hpp"
#include <optional>

CharacterClass Pattern::parse_escape_seq() {
  idx += 1;
  auto next_character = pattern_string.at(idx);
  switch (next_character) {
  case 'd':
    idx += 1;
    return Digit();
  case 'w':
    idx += 1;
    return Alphanumeric();
  case '\\':
    idx += 1;
    return LiteralCharacter{ .character = '\\' };
  case '.':
    idx += 1;
    return LiteralCharacter{ .character = '.' };
  default:
    throw "Unexpected escape sequence";
  }
}

CharacterClass Pattern::parse_character_group() {
  int closing_bracket_idx = pattern_string.find_first_of(']', idx);
  if (closing_bracket_idx == std::string::npos)
    throw "Unterminated character group in pattern string";

  bool is_positive = pattern_string.at(idx + 1) != '^';
  if (is_positive) {
    int len = closing_bracket_idx - idx - 1;
    const auto characters = &pattern_string.at(idx + 1);
    auto matcher = PositiveCharacterGroup{.characters = characters, .len = len};

    idx = closing_bracket_idx + 1;
    return matcher;
  }

  int len = closing_bracket_idx - idx - 2;
  const auto characters = &pattern_string.at(idx + 2);
  auto matcher = NegativeCharacterGroup{.characters = characters, .len = len};

  idx = closing_bracket_idx + 1;
  return matcher;
}

CharacterClass Pattern::next_character_class() {
  auto character = pattern_string.at(idx);
  switch (character) {
  case '\\':
    return parse_escape_seq();

  case '[':
    return parse_character_group();

  case '^':
    idx += 1;
    return StartOfString();

  case '$':
    idx += 1;
    return EndOfString();

  case '.':
    idx += 1;
    return Wildcard();

  default:
    idx += 1;
    return LiteralCharacter{ .character = character };
  }
}

std::optional<Matcher> Pattern::next_matcher() {
  if (idx >= pattern_string.length())
    return std::nullopt;

  auto character_class = next_character_class();
  if (idx >= pattern_string.length())
    return Matcher(std::move(character_class), Quantifier::ExactlyOne);

  char quantifier_char = pattern_string.at(idx);

  Quantifier quantifier;
  if (quantifier_char == '+') {
    idx += 1;
    quantifier = Quantifier::OneOrMore;
  } else if (quantifier_char == '*') {
    idx += 1;
    quantifier = Quantifier::ZeroOrMore;
  } else if (quantifier_char == '?') {
    idx += 1;
    quantifier = Quantifier::ZeroOrOne;
  } else {
    quantifier = Quantifier::ExactlyOne;
  }

  return Matcher(std::move(character_class), quantifier);
}
