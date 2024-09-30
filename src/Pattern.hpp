#ifndef PATTERN_H
#define PATTERN_H

#include "Matcher.hpp"
#include <optional>
#include <string>

class Pattern {
private:
  const std::string &pattern_string;
  int idx;

  CharacterClass parse_escape_seq();
  CharacterClass parse_character_group();
  CharacterClass next_character_class();

public:
  Pattern(const std::string &pattern_string) : pattern_string(pattern_string){};

  std::optional<Matcher> next_matcher();
};

#endif // PATTERN_H
