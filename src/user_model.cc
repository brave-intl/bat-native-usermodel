/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "../include/user_model.h"
#include "user_model.h"
#include "bag_of_words_extractor.h"

#include <iostream>

bool usermodel::UserModel::initializePageClassifier(const std::string& model) {
    if (page_classifier.LoadModel(model)) {
        initialized_ = true;
    }

    return initialized_;
}

std::string usermodel::UserModel::winningCategory(const std::vector<double>& scores, const std::vector<std::string>& taxonomies) {
    auto max = std::max_element(scores.begin(), scores.end());
    auto argmax =  std::distance(scores.begin(), max);

    return taxonomies.at(argmax);
}

bool usermodel::UserModel::IsInitialized() {
    return initialized_;
}

std::vector<double>  usermodel::UserModel::classifyPage(const std::string& data) {
    usermodel::BagOfWords bow;
    bow.Process(data);
    return page_classifier.Predict(bow.GetFrequencies());
}

void usermodel::UserModel::onPageLoad(const std::string& html, const std::string& url, uint32_t window_id, uint32_t tab_id) {
    // auto profile = this->GetUserProfile();

    auto scores = this->classifyPage(html);
    // user_profile.update(scores, url);
    // tabs_classification_store.set(window_id, tab_id, winning_category);

    // this->UpdateProfile(profile);
}
