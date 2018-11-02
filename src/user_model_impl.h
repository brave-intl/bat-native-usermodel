/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef DEPS_BAT_USERMODEL_SRC_USER_MODEL_IMPL_H_
#define DEPS_BAT_USERMODEL_SRC_USER_MODEL_IMPL_H_

#include "bat/usermodel/user_model.h"
#include "naive_bayes.h"

namespace usermodel {

class UserModelImpl : public UserModel {
 public:
  UserModelImpl();

  bool initializePageClassifier(const std::string& model) override;
  void OnPageLoad(const std::string& html,
                  const std::string& url,
                  uint32_t window_id,
                  uint32_t tab_id) override;
  // void onTabFocus(uint32_t window_id, uint32_t tab_id);
  // void onNotificationEvent(NotificationEventType type);
  // void updateState();
  std::vector<double>  classifyPage(const std::string& data) override;
  std::string winningCategory(const std::vector<double>& scores) override;
  bool IsInitialized() override;

 private:
  // UserProfile user_profile;
  // bool isSearchURL(const std::string& url);
  // bool isShoppingIntent(const std::string& url, const std::string& html);
  bool initialized_;
  NaiveBayes page_classifier;

  // Not copyable, not assignable
  UserModelImpl(const UserModelImpl&) = delete;
  UserModelImpl& operator=(const UserModelImpl&) = delete;
};

}  // namespace usermodel

#endif  // DEPS_BAT_USERMODEL_SRC_USER_MODEL_IMPL_H_
