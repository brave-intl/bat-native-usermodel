/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef DEPS_BAT_USERMODEL_SRC_USER_MODEL_IMPL_H_
#define DEPS_BAT_USERMODEL_SRC_USER_MODEL_IMPL_H_

#include <string>
#include <vector>

#include "bat/usermodel/user_model.h"
#include "naive_bayes.h"

namespace usermodel {

class UserModelImpl : public UserModel {
 public:
  UserModelImpl();

  bool InitializePageClassifier(
      const std::string& model,
      const std::string& locale) override;
  bool IsInitialized() const override;

  const std::vector<double> ClassifyPage(
      const std::string& html) override;

  const std::string GetWinningCategory(
      const std::vector<double>& scores) override;

  const std::string GetTaxonomyAtIndex(
      const int index) override;

 private:
  bool is_initialized_;

  NaiveBayes page_classifier_;

  std::string region_;

  // Not copyable, not assignable
  UserModelImpl(const UserModelImpl&) = delete;
  UserModelImpl& operator=(const UserModelImpl&) = delete;
};

}  // namespace usermodel

#endif  // DEPS_BAT_USERMODEL_SRC_USER_MODEL_IMPL_H_
