/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "user_model_impl.h"

#include "bag_of_words_extractor.h"
#include "naive_bayes.h"

namespace usermodel {

// static
UserModel* UserModel::CreateInstance() {
  return new UserModelImpl();
}

UserModelImpl::UserModelImpl() : initialized_(false) {}

bool UserModelImpl::initializePageClassifier(const std::string& model) {
    if (page_classifier.LoadModel(model)) {
        initialized_ = true;
    }

    return initialized_;
}

std::string UserModelImpl::winningCategory(const std::vector<double>& scores) {
    auto max = std::max_element(scores.begin(), scores.end());
    auto argmax =  std::distance(scores.begin(), max);

    return page_classifier.Classes().at(argmax);
}

bool UserModelImpl::IsInitialized() {
    return initialized_;
}

std::vector<double>  UserModelImpl::classifyPage(const std::string& data) {
    BagOfWords bow;
    bow.Process(data);
    return page_classifier.Predict(bow.GetFrequencies());
}

void UserModelImpl::OnPageLoad(const std::string& html, const std::string& url, uint32_t window_id, uint32_t tab_id) {
    (void)url;
    (void)window_id;
    (void)tab_id;

    // auto profile = this->GetUserProfile();

    auto scores = this->classifyPage(html);
    // user_profile.update(scores, url);
    // tabs_classification_store.set(window_id, tab_id, winning_category);

    // this->UpdateProfile(profile);
}

}  // namespace usermodel
