/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef DEPS_BAT_USERMODEL_SRC_BAG_OF_WORDS_EXTRACTOR_H_
#define DEPS_BAT_USERMODEL_SRC_BAG_OF_WORDS_EXTRACTOR_H_

#include <string>
#include <map>
#include <vector>

namespace usermodel {

class HashVectorizer {
 public:
  HashVectorizer();
  ~HashVectorizer();

  bool Process(const std::string& html);
  std::map<std::string, double> GetFrequencies();

 private:
  std::string get_hash(std::string& substring);
  std::vector<int> substring_sizes;
  std::map<std::string, double> frequencies_;
  bool to_lower_;
  int num_buckets; // number of buckets to use for hashing
};

}  // namespace usermodel

#endif  // DEPS_BAT_USERMODEL_SRC_BAG_OF_WORDS_EXTRACTOR_H_
