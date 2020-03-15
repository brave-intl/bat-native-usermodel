/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */
 
#include <map>
#include <algorithm>

#include "user_model_impl.h"

namespace usermodel {

// static
UserModel* UserModel::CreateInstance() {
  return new UserModelImpl();
}

UserModelImpl::UserModelImpl() :
    is_initialized_(false),
    region_("en") {
}

bool UserModelImpl::InitializePageClassifier(const std::string& model) {
  page_classifier_pipeline_ = Pipeline();
  is_initialized_ = page_classifier_pipeline_.FromJson(model);
  return is_initialized_;
}

bool UserModelImpl::IsInitialized() const {
  return is_initialized_;
}

const std::map<std::string,double> UserModelImpl::ClassifyPage(const std::string& html) {
  std::map<std::string,double> topPredictions;
  topPredictions = page_classifier_pipeline_.Get_Top_Predictions(html);
  return topPredictions;
}

}  // namespace usermodel
