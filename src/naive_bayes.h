/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef NAIVE_BAYES_H_
#define NAIVE_BAYES_H_

#include <string>
#include <vector>
#include <map>
#include <cmath>

#include "bag_of_words_extractor.h"

namespace usermodel {

class NaiveBayes {
 public:
  NaiveBayes();
  ~NaiveBayes();

  bool LoadModel(const std::string& json);

  std::vector<std::string> Classes();
  std::vector<double> Predict(std::map<std::string, double> features);

 private:
  std::vector<std::string> classes_;
  std::vector<double> priors_;
  std::map<std::string, std::vector<double>> features_;
};

}  // namespace usermodel

#endif  // NAIVE_BAYES_H_
