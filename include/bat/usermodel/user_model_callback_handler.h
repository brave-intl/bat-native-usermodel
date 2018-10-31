/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BAT_USERMODEL_USERMODEL_CALLBACK_HANDLER_H_
#define BAT_USERMODEL_USERMODEL_CALLBACK_HANDLER_H_

#include <string>

#include "bat/usermodel/export.h"

namespace usermodel {

enum USERMODEL_EXPORT Result {
  OK = 0,
  BAD_MODEL = 1
};

class USERMODEL_EXPORT UsermodelCallbackHandler {
 public:
  virtual ~UsermodelCallbackHandler() = default;

  virtual void OnUsermodelLoaded(Result result,
                                 const std::string& data) {}
};
}  // namespace usermodel

#endif  // BAT_USERMODEL_USERMODEL_CALLBACK_HANDLER_H_
