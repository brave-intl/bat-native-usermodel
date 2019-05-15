/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "user_model_impl.h"

#include <map>
#include <algorithm>

#include "bag_of_words_extractor.h"
#include "naive_bayes.h"

namespace usermodel {

// static
UserModel* UserModel::CreateInstance() {
  return new UserModelImpl();
}

UserModelImpl::UserModelImpl() :
    is_initialized_(false),
    locale_("US") {
}

bool UserModelImpl::InitializePageClassifier(
    const std::string& model,
    const std::string& locale) {
  if (page_classifier_.LoadModel(model)) {
    is_initialized_ = true;

    locale_ = locale;
  } else {
    is_initialized_ = false;
  }

  return is_initialized_;
}

bool UserModelImpl::IsInitialized() const {
  return is_initialized_;
}

const std::vector<double> UserModelImpl::ClassifyPage(
    const std::string& html) {
  BagOfWords bag_of_words;
  if (!bag_of_words.Process(html)) {
    return {};
  }

  auto frequencies = bag_of_words.GetFrequencies();
  auto classification = page_classifier_.Predict(frequencies);

  return classification;
}

const std::string UserModelImpl::GetWinningCategory(
    const std::vector<double>& scores) {
  if (scores.size() == 0) {
    return "";
  }

  auto max = std::max_element(scores.begin(), scores.end());
  auto argmax = std::distance(scores.begin(), max);

  return page_classifier_.Classes().at(argmax);
}

const std::string UserModelImpl::GetTaxonomyAtIndex(
    const int index) {
  auto classes = page_classifier_.Classes();
  return classes.at(index);
}

}  // namespace usermodel
