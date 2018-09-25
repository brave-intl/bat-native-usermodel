#include "ads_agent.h"
#include "ads_relevance.h"
#include <algorithm>
#include <vector>

usermodel::AdsAgent::AdsAgent(UserModel *usermodel) {
    this->usermodel = usermodel;
}

void usermodel::AdsAgent::LoadRelevanceModel(const std::string& json) {
    relevance_model.LoadModel(json);
}

double usermodel::AdsAgent::ScoreRelevance(std::vector<double> features) {
    auto features_map = std::map<std::string, double>();

    features_map["long_term_interest"] = features.at(0);
    features_map["short_term_interest"] = features.at(1);
    features_map["search_intent"] = features.at(2);

    return relevance_model.Predict(features_map).at(0);
}

int usermodel::AdsAgent::AdsScoreAndSample(std::vector<Ad> ads, UserProfile profile) {
    std::vector<double> scores;

    // TODO(ptigas): find better way to do this
    profile.taxonomies_ = usermodel->page_classifier.Classes();

    if (this->usermodel->IsInitialized()) {
        for (auto ad : ads) {
            auto feature_vector = usermodel::AdsRelevance::DeriveFeatures(profile, ad);
            scores.push_back(ScoreRelevance(feature_vector));
        }

        auto max = std::max_element(scores.begin(), scores.end());
        return std::distance(scores.begin(), max);
    }

    return -1;
}
