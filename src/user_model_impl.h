/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef DEPS_BAT_USERMODEL_SRC_USER_MODEL_IMPL_H_
#define DEPS_BAT_USERMODEL_SRC_USER_MODEL_IMPL_H_

#include <string>
#include <vector>

#include "bat/usermodel/user_model.h"
#include "pipeline.h"

namespace usermodel {

class UserModelImpl : public UserModel {
 public:
  UserModelImpl();

  bool InitializePageClassifier(
      const std::string& model) override;
  bool IsInitialized() const override;


  const std::map<std::string,double> ClassifyPage(const std::string& content) override;
 private:
  bool is_initialized_;

  Pipeline page_classifier_pipeline_;
  std::string region_;

  // Not copyable, not assignable
  UserModelImpl(const UserModelImpl&) = delete;
  UserModelImpl& operator=(const UserModelImpl&) = delete;
};

}  // namespace usermodel

#endif  // DEPS_BAT_USERMODEL_SRC_USER_MODEL_IMPL_H_
