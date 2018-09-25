#ifndef INCLUDE_ADS_RELEVANCE_H_
#define INCLUDE_ADS_RELEVANCE_H_

#include "ad.h"
#include "user_profile.h"

namespace usermodel {

class AdsRelevance {
 public:
    static std::vector<double> DeriveFeatures(const UserProfile& profile, const Ad& ad);
    int SampleAd(std::vector<double> features);
};

}  // namespace usermodel

#endif  // INCLUDE_ADS_RELEVANCE_H_
