/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef SRC_BAG_OF_WORDS_EXTRACTOR_H_
#define SRC_BAG_OF_WORDS_EXTRACTOR_H_

#include <string>
#include <map>
#include <vector>

#include "stmr.h"

namespace usermodel {
class BagOfWords {
 public:
    BagOfWords();
    ~BagOfWords();
    bool Process(const std::string& data);
    std::map<std::string, double> GetFrequencies();

 private:
    std::vector<std::string> words_;
    std::map<std::string, double> frequencies_;
    bool to_lower_;
};
}  // namespace usermodel

#endif  // SRC_BAG_OF_WORDS_EXTRACTOR_H_
