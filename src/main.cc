#include "Regex.hpp"
#include <cstdio>
#include <iostream>
#include <string>

int main(int argc, char *argv[]) {
  // Flush after every std::cout / std::cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  if (argc != 3) {
    std::cerr << "Expected two arguments" << std::endl;
    return 1;
  }

  std::string flag = argv[1];
  std::string pattern = argv[2];

  if (flag != "-E") {
    std::cerr << "Expected first argument to be '-E'" << std::endl;
    return 1;
  }

  std::string input_line;
  std::getline(std::cin, input_line);

  try {
    auto regex = Regex(pattern);
    auto match = regex.test(input_line);
    if (match) {
      int match_len = match->end - match->start;
      auto matched_substr = input_line.substr(match->start, match_len);
      printf("matched substring: %s\n", matched_substr.c_str());

      return 0;
    } else {

      printf("no match\n");
      return 1;
    }
  } catch (const std::runtime_error &e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }
}

