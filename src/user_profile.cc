/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <string>

#include "../include/user_profile.h"

#include "deps/bat-native-rapidjson/include/rapidjson/document.h"
#include "deps/bat-native-rapidjson/include/rapidjson/writer.h"
#include "deps/bat-native-rapidjson/include/rapidjson/stringbuffer.h"
#include "deps/bat-native-rapidjson/include/rapidjson/prettywriter.h"


using namespace usermodel;

UserProfile::UserProfile() {}

UserProfile::~UserProfile() {}

void UpdateProfile(std::vector<double>* a, std::vector<double> b, double weight) {
}

bool UserProfile::Update(std::vector<double> scores, const std::string& url) {
    // UpdateProfile(&(this->long_term_interests_), scores, 0.8);
    // UpdateProfile(&(this->short_term_interests_), scores, 0.1);
    return false;
}

void LoadToMap(const rapidjson::Value& vector, std::map<std::string, double>* data) {
    for (rapidjson::Value::ConstMemberIterator itr = vector.MemberBegin(); itr != vector.MemberEnd(); ++itr) {
      double v = vector[itr->name.GetString()].GetDouble();
      (*data)[itr->name.GetString()] = v;
    }
}

std::unique_ptr<UserProfile> UserProfile::FromJSON(const std::string& json) {
    std::unique_ptr<UserProfile> res = std::unique_ptr<UserProfile>(new UserProfile());

    rapidjson::Document d;
    d.Parse(json.c_str());

    LoadToMap(d["long_term_interests"], &(res->long_term_interests_));
    LoadToMap(d["short_term_interests"], &(res->short_term_interests_));
    LoadToMap(d["search_intent"], &(res->search_intent_));
    LoadToMap(d["shopping_intent"], &(res->shopping_intent_));

    return res;
}

void SerializeMap(std::string key, std::map<std::string, double> data, rapidjson::Writer<rapidjson::StringBuffer>* writer) {
    writer->Key(key.c_str());

    writer->StartObject();
    for ( auto c : data ) {
        writer->Key(c.first.c_str());
        writer->Double(c.second);
    }
    writer->EndObject();
}

const std::string UserProfile::ToJSON() const {
    rapidjson::StringBuffer sb;
    rapidjson::Writer<rapidjson::StringBuffer> writer(sb);

    writer.StartObject();

    SerializeMap("long_term_interests", this->long_term_interests_, &writer);
    SerializeMap("short_term_interests", this->short_term_interests_, &writer);
    SerializeMap("search_intent", this->search_intent_, &writer);
    SerializeMap("shopping_intent", this->shopping_intent_, &writer);

    writer.EndObject();

    return sb.GetString();
}
