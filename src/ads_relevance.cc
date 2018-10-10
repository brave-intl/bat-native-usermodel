
#include "ads_relevance.h"
#include "user_profile.h"
#include "user_model.h"
#include "ad.h"
#include "naive_bayes.h"

#include <string>
#include <vector>

std::vector<std::string> Split(const std::string &s, char delim) {
  std::stringstream ss(s);
  std::string item;
  std::vector<std::string> elems;
  while (std::getline(ss, item, delim)) {
    elems.push_back(std::move(item));
  }
  return elems;
}

double CategoryMatchScore(const std::string& ad_category, const std::string& category, double entropy) {
    if (entropy > 0.4) {
        return 0.0;
    }

    double score = 1.0;
    auto hierarchy = Split(category, '-');
    int level = 0;
    for (auto c : hierarchy) {
        auto topic = hierarchy[level];
        if (topic == ad_category) {
            break;
        }
        score -= (1.0 / hierarchy.size());
        level++;
    }

    return score;
}

std::vector<double> usermodel::AdsRelevance::DeriveFeatures(
    const usermodel::UserProfile& profile, const usermodel::Ad& ad, const std::vector<std::string>& taxonomy) {
    std::vector<double> features;

    // long term interest match
    features.push_back(
        CategoryMatchScore(ad.category,
        usermodel::UserModel::winningCategory(
            profile.long_term_interests_,
            taxonomy),
        usermodel::UserProfile::Entropy(profile.long_term_interests_)));

    // short term interest match
    features.push_back(
        CategoryMatchScore(ad.category,
        usermodel::UserModel::winningCategory(
            profile.short_term_interests_,
            taxonomy),
        usermodel::UserProfile::Entropy(profile.short_term_interests_)));

    // search intent match
    features.push_back(
        CategoryMatchScore(ad.category,
        usermodel::UserModel::winningCategory(
            profile.search_intent_,
            taxonomy),
        usermodel::UserProfile::Entropy(profile.search_intent_)));

    return features;
}

int usermodel::AdsRelevance::SampleAd(std::vector<double> features) {
    return 0;
}
