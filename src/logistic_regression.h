/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef SRC_LOGISTIC_REGRESSION_H_
#define SRC_LOGISTIC_REGRESSION_H_

#include <vector>
#include <cmath>
#include "bag_of_words_extractor.h"
#include "deps/bat-native-rapidjson/include/rapidjson/document.h"
#include "deps/bat-native-rapidjson/include/rapidjson/writer.h"
#include "deps/bat-native-rapidjson/include/rapidjson/stringbuffer.h"

namespace usermodel {
class LogisticRegression {
 public:
  LogisticRegression();
  ~LogisticRegression();
  bool LoadModel(const std::string& model);
  std::vector<double> Predict(std::map<std::string, double> features);

 private:
  std::vector<std::string> features_;
  std::map<std::string, double> weights_;
  double bias_;
};
}  // namespace usermodel

#endif  // SRC_LOGISTIC_REGRESSION_H_
