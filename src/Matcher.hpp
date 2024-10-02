#ifndef MATCHER_H
#define MATCHER_H

#include <stack>
#include <string>
#include <variant>

enum class Quantifier { ExactlyOne, OneOrMore, ZeroOrOne, ZeroOrMore };

struct MatchState {
  int matcher_idx;
  int line_offset;
};

struct StartOfString {
  int match_once(const std::string &input_line, int from_offset);
};

struct EndOfString {
  int match_once(const std::string &input_line, int from_offset);
};

struct Wildcard {
  int match_once(const std::string &input_line, int from_offset);
};

struct Digit {
  int match_once(const std::string &input_line, int from_offset);
};

struct Alphanumeric {
  int match_once(const std::string &input_line, int from_offset);
};

struct LiteralCharacter {
  char character;
  int match_once(const std::string &input_line, int from_offset);
};

struct PositiveCharacterGroup {
  const char *characters;
  int len;
  int match_once(const std::string &input_line, int from_offset);
};

struct NegativeCharacterGroup {
  const char *characters;
  int len;
  int match_once(const std::string &input_line, int from_offset);
};

using CharacterClass =
    std::variant<StartOfString, EndOfString, Wildcard, Digit, Alphanumeric,
                 LiteralCharacter, PositiveCharacterGroup,
                 NegativeCharacterGroup>;

class Matcher {
private:
  CharacterClass character_class;
  Quantifier quantifier;

  int match_once(const std::string &input_line, int from_offset);

public:
  Matcher(const Matcher &) = delete ;
  Matcher(Matcher &&) = default;
  Matcher &operator=(const Matcher &) = delete;
  Matcher &operator=(Matcher &&) = default;
  Matcher(CharacterClass &&character_class, Quantifier quantifier)
      : character_class(character_class), quantifier(quantifier){};

  void match(const std::string &input_line, std::stack<MatchState> &states);
};

#endif // MATCHER_H
