/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef INCLUDE_NOTIFICATION_DELIVERY_H_
#define INCLUDE_NOTIFICATION_DELIVERY_H_

#include <string>

namespace usermodel {

class NotificationDelivery {
 public:
    void Initialize();
    void LoadModel(const std::string& jsonModel);

    // this checks if the notification model is
    // giving a good score for sending a notification.
    // if yes, show the notification, else return false.
    void CheckAndNotify();
    void UpdateFeatures();
};

}  // namespace usermodel

#endif  // INCLUDE_NOTIFICATION_DELIVERY_H_
