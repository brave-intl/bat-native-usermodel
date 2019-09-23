/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef DEPS_BAT_USERMODEL_SRC_BIGRAM_EXTRACTOR_H_
#define DEPS_BAT_USERMODEL_SRC_BIGRAM_EXTRACTOR_H_

# include <iostream>
# include <map>
# include <vector>
# include <sstream>
# include <regex>

namespace usermodel {

class BiGram {
 public:
  BiGram();
  ~BiGram();

  bool Process(const std::string& html);
  std::map<std::string, double> GetFrequencies();

 private:
  std::map<std::string, double> token_frequencies;
};

}  // namespace usermodel

#endif  // DEPS_BAT_USERMODEL_SRC_BIGRAM_EXTRACTOR_H_
