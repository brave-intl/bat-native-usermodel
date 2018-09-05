/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef SRC_NAIVE_BAYES_H_
#define SRC_NAIVE_BAYES_H_

#include <vector>
#include <cmath>
#include "bag_of_words_extractor.h"
#include "deps/bat-native-rapidjson/include/rapidjson/document.h"
#include "deps/bat-native-rapidjson/include/rapidjson/writer.h"
#include "deps/bat-native-rapidjson/include/rapidjson/stringbuffer.h"

namespace usermodel {
class NaiveBayes {
 public:
  NaiveBayes();
  ~NaiveBayes();
  bool LoadModel(const std::string& model);
  std::vector<double> Predict(std::map<std::string, double> features);
  std::vector<std::string> Classes();
  std::string WinningCategory(std::vector<double> scores);

 private:
  std::vector<std::string> classes_;
  std::vector<double> priors_;
  std::map<std::string, std::vector<double>> features_;
};
}  // namespace usermodel

#endif  // SRC_NAIVE_BAYES_H_
