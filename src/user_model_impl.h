/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BAT_USERMODEL_USER_MODEL_IMPL_H_
#define BAT_USERMODEL_USER_MODEL_IMPL_H_

#include "base/macros.h"
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

  DISALLOW_COPY_AND_ASSIGN(UserModelImpl);
};

}  // namespace usermodel

#endif  // BAT_USERMODEL_USER_MODEL_IMPL_H_
