
#ifndef SRC_BAG_OF_WORDS_EXTRACTOR_H_
#define SRC_BAG_OF_WORDS_EXTRACTOR_H_

#include <stdio.h>
#include <vector>
#include <string>
#include <map>
#include <cstring>
#include <sstream>
#include <algorithm>
#include <iostream>
#include <regex>
#include <iterator>
#include "stmr.h"

namespace usermodel {
class BagOfWords {
 public:
    BagOfWords();
    ~BagOfWords();
    bool process(const std::string& data);
    std::map<std::string, double> getFrequencies();

 private:
    std::vector<std::string> words;
    std::map<std::string, double> frequencies;
    bool to_lower;
};
}  // namespace usermodel

#endif  // SRC_BAG_OF_WORDS_EXTRACTOR_H_
