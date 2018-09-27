#include "ads_agent.h"
#include "ads_relevance.h"
#include "logistic_regression.h"

#include <algorithm>
#include <vector>

usermodel::AdsAgent::AdsAgent(UserModel *usermodel) {
    this->usermodel = usermodel;
    relevance_model = new LogisticRegression();
}

void usermodel::AdsAgent::LoadRelevanceModel(const std::string& json) {
    relevance_model->LoadModel(json);
}

double usermodel::AdsAgent::ScoreRelevance(const std::vector<double>& features) {
    auto features_map = std::map<std::string, double>();

    features_map["long_term_interest"] = features.at(0);
    features_map["short_term_interest"] = features.at(1);
    features_map["search_intent"] = features.at(2);

    return relevance_model->Predict(features_map).at(0);
}

int usermodel::AdsAgent::AdsScoreAndSample(const std::vector<Ad>& ads, const UserProfile& profile) {
    std::vector<double> scores;

    if (this->usermodel->IsInitialized()) {
        for (auto ad : ads) {
            auto feature_vector = usermodel::AdsRelevance::DeriveFeatures(profile, ad, usermodel->page_classifier.Classes());
            scores.push_back(ScoreRelevance(feature_vector));
        }

        auto max = std::max_element(scores.begin(), scores.end());
        return std::distance(scores.begin(), max);
    }

    return -1;
}
