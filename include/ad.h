#ifndef INCLUDE_AD_H_
#define INCLUDE_AD_H_

#include <string>

namespace usermodel {
class Ad {
 public:
    Ad();
    Ad(const Ad& other);
    ~Ad();

    std::string category;
    std::string advertiser;
    std::string notification_text;
    std::string notification_url;
    std::string uuid;
};

}  // namespace usermodel

#endif  // INCLUDE_AD_H_
