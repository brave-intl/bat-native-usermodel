#ifndef BAT_USERMODEL_AD_H_
#define BAT_USERMODEL_AD_H_

#include <string>

namespace usermodel {

struct Ad {
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

#endif  // BAT_USERMODEL_AD_H_
