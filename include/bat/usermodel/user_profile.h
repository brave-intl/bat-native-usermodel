/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BAT_USERMODEL_USER_PROFILE_H_
#define BAT_USERMODEL_USER_PROFILE_H_

#include <string>
#include <map>
#include <vector>

#include "bat/usermodel/export.h"

namespace usermodel {

class USERMODEL_EXPORT UserProfile {
 public:
  UserProfile();
  UserProfile(const std::string& user_id);
  UserProfile(const UserProfile& info);
  ~UserProfile();

  static std::unique_ptr<UserProfile> FromJSON(const std::string& json);
  const std::string ToJSON() const;

  bool Update(const std::vector<double> scores, time_t time_since_last_update, bool isSearch);
  static double Entropy(const std::vector<double>& scores);

  std::string user_id;
  std::vector<std::string> taxonomy;
  std::vector<double> long_term_interests_;
  std::vector<double> short_term_interests_;
  std::vector<double> search_intent_;
  std::vector<double> shopping_intent_;
};

}  // namespace usermodel

#endif  // BAT_USERMODEL_USER_PROFILE_H_
