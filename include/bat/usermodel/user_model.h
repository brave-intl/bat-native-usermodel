/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BAT_USERMODEL_USER_MODEL_H_
#define BAT_USERMODEL_USER_MODEL_H_

#include <string>
#include <vector>

#include "bat/usermodel/export.h"

namespace usermodel {

class USERMODEL_EXPORT UserModel {
 public:
  static UserModel* CreateInstance();

  virtual ~UserModel() = default;
  virtual bool initializePageClassifier(const std::string& model) = 0;
  virtual void OnPageLoad(const std::string& html,
                          const std::string& url,
                          uint32_t window_id,
                          uint32_t tab_id) = 0;
  virtual std::vector<double> classifyPage(const std::string& data) = 0;
  virtual std::string winningCategory(const std::vector<double>& scores) = 0;
  virtual bool IsInitialized() = 0;
};

}  // namespace usermodel

#endif  // BAT_USERMODEL_USER_MODEL_H_
