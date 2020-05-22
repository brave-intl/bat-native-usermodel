/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <algorithm>
#include <map>

#include "user_model_impl.h"

namespace usermodel {

// static
UserModel* UserModel::CreateInstance() {
  return new UserModelImpl();
}

UserModelImpl::UserModelImpl()
    : is_initialized_(false) {}

bool UserModelImpl::InitializePageClassifier(
    const std::string& model) {
  if (is_initialized_) {
    return false;
  }

  page_classifier_pipeline_ = Pipeline();
  is_initialized_ = page_classifier_pipeline_.FromJson(model);
  return is_initialized_;
}

bool UserModelImpl::IsInitialized() const {
  return is_initialized_;
}

const std::map<std::string, double> UserModelImpl::ClassifyPage(
    const std::string& content) {
  if (!is_initialized_ || content.empty()) {
    return {};
  }

  const std::map<std::string, double> top_predictions
      = page_classifier_pipeline_.Get_Top_Predictions(content);

  return top_predictions;
}

}  // namespace usermodel
