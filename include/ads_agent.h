/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef INCLUDE_ADS_AGENT_H_
#define INCLUDE_ADS_AGENT_H_

#include <string>
#include <vector>

#include "user_model.h"
#include "user_profile.h"
#include "logistic_regression.h"
#include "ad.h"

namespace usermodel {

class AdsAgent {
 public:
    explicit AdsAgent(UserModel *usermodel);
    void LoadRelevanceModel(const std::string& json);
    void LoadDeliveryModel(const std::string& json);

    // this checks if the notification model is
    // giving a good score for sending a notification.
    // if yes, show the notification, else return false.
    void CheckAndNotify();
    void DeriveFeatures();

    double ScoreRelevance(std::vector<double> features);
    int AdsScoreAndSample(std::vector<Ad> ads, UserProfile profile);

    UserModel* usermodel;
    usermodel::LogisticRegression relevance_model;
};

}  // namespace usermodel

#endif  // INCLUDE_ADS_AGENT_H_
