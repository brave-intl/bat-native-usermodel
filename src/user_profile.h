
#ifndef SRC_USER_PROFILE_H_
#define SRC_USER_PROFILE_H_

#include <string>
#include <map>
#include <vector>

namespace usermodel {
class UserProfile {
 public:
    UserProfile();
    ~UserProfile();
    bool loadJSON(std::string data);
    std::string toJSON();

    bool updateLongTermInterests(std::map<std::string, double> page_scores);
    bool updateShortTermInterests(std::map<std::string, double> page_scores);
    bool updateSearchIntent(std::map<std::string, double> page_scores);
    bool updateShoppingIntent(std::map<std::string, double> page_scores);

    bool update(std::vector<double> scores, const std::string& url);

 private:
    std::map<std::string, double> longTermInterests;
    std::map<std::string, double> shortTermInterests;
    std::map<std::string, double> searchIntent;
    std::map<std::string, double> shoppingIntent;
};
}  // namespace usermodel

#endif  // SRC_USER_PROFILE_H_
