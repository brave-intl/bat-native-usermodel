/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <map>
#include <algorithm>

#include "user_model_impl.h"

#include "bag_of_words_extractor.h"
#include "naive_bayes.h"

namespace usermodel {

// static
UserModel* UserModel::CreateInstance() {
  return new UserModelImpl();
}

UserModelImpl::UserModelImpl() :
    is_initialized_(false),
    region_("en") {
}

bool UserModelImpl::InitializePageClassifier(
    const std::string& model,
    const std::string& region) {
  auto lowercase_region = region;
  std::transform(lowercase_region.begin(), lowercase_region.end(),
      lowercase_region.begin(), ::tolower);
  if (lowercase_region == "ja") {
    page_classifier_pipeline_ = Pipeline();
    is_initialized_ = page_classifier_pipeline_.FromJson(model);
  } else {
    is_initialized_ = page_classifier_.LoadModel(model);
  }

  if (is_initialized_) {
    region_ = lowercase_region;
  }

  return is_initialized_;
}

bool UserModelImpl::IsInitialized() const {
  return is_initialized_;
}

const std::vector<double> UserModelImpl::ClassifyPage(
    const std::string& html) {
  std::vector<double> classification;

  if (region_ == "ja") {
    classification =
        page_classifier_pipeline_.Get_Advertising_Predictions(html);
  } else {
    BagOfWords bag_of_words;
    if (!bag_of_words.Process(html)) {
      return {};
    }

    auto frequencies = bag_of_words.GetFrequencies();
    classification = page_classifier_.Predict(frequencies);
  }

  return classification;
}

const std::string UserModelImpl::GetWinningCategory(
    const std::vector<double>& scores) {
  if (scores.size() == 0) {
    return "";
  }

  auto max = std::max_element(scores.begin(), scores.end());
  auto argmax = std::distance(scores.begin(), max);

  std::string winning_category;

  if (region_ == "ja") {
    winning_category = page_classifier_pipeline_.get_category(argmax);
  } else {
    winning_category = page_classifier_.Classes().at(argmax);
  }

  return winning_category;
}

const std::string UserModelImpl::GetTaxonomyAtIndex(
    const int index) {
  auto classes = page_classifier_.Classes();
  return classes.at(index);
}

}  // namespace usermodel
