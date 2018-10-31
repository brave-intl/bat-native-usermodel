/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BAT_USERMODEL_NOTIFICATION_EVENT_TYPE_H_
#define BAT_USERMODEL_NOTIFICATION_EVENT_TYPE_H_

#include "export.h"

namespace usermodel {

enum USERMODEL_EXPORT NotificationEventType {
  CLICKED,
  CLOSED,
  TIMED_OUT
};

}  // namespace usermodel

#endif  // BAT_USERMODEL_NOTIFICATION_EVENT_TYPE_H_
