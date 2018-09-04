#ifndef INCLUDE_USER_PROFILE_H_
#define INCLUDE_USER_PROFILE_H_

#include <string>
#include <map>
#include <vector>
#include <functional>

#include "export.h"

namespace usermodel {
/*
using UserProfileReadCallback = std::function<void(Result,
    std::unique_ptr<UserProfile>)>;

using UserProfileWriteCallback = std::function<void(Result,
    std::unique_ptr<UserProfile>)>;
*/
USERMODEL_EXPORT class UserProfile {
 public:
    UserProfile();
    UserProfile(const std::string& user_id);
    UserProfile(const UserProfile& info);
    ~UserProfile();

    static std::unique_ptr<UserProfile> FromJSON(const std::string& json);
    const std::string ToJSON() const;

    bool Update(std::vector<double> scores, const std::string& url);

    std::string user_id;
    std::map<std::string, double> long_term_interests_;
    std::map<std::string, double> short_term_interests_;
    std::map<std::string, double> search_intent_;
    std::map<std::string, double> shopping_intent_;
};
}  // namespace usermodel

#endif  // INCLUDE_USER_PROFILE_H_
