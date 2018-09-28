/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef INCLUDE_USER_MODEL_CALLBACK_HANDLER_H_
#define INCLUDE_USER_MODEL_CALLBACK_HANDLER_H_

#include <string>

#include "export.h"

namespace usermodel {

USERMODEL_EXPORT enum Result {
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

#endif  // INCLUDE_USER_MODEL_CALLBACK_HANDLER_H_
