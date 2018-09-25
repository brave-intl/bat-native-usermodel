/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <string>
#include <limits>
#include <cmath>
#include <numeric>

#include "../include/user_profile.h"

#include "rapidjson/document.h"
#include "rapidjson/error/en.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

using namespace usermodel;

UserProfile::UserProfile() {}

UserProfile::~UserProfile() {}

void UpdateProfile(std::vector<double> to, const std::vector<double> from, double weight) {
    if ( to.size() ==0 ) {
        to.resize(from.size(), 1.0/from.size());
    }

    for (size_t i = 0; i < to.size(); i++) {
        to.at(i) = (1-weight)*to.at(i) + weight*from.at(i);
    }
}

double log2(double number) {
    return log( number ) / log( 2 );
}

double UserProfile::Entropy(const std::vector<double> scores) {
    double sum = std::accumulate(scores.begin(), scores.end(), 0.0);
    if (sum == 0.0) {
        return 1.0;
    }

    double entropy = 0.0;
    for (size_t i = 0; i < scores.size(); i++) {
        if (scores.at(i) > 0.0) {
            double a = scores.at(i) / sum;
            entropy -= a*log2(a);
        }
    }

    return entropy;
}

bool UserProfile::Update(const std::vector<double> scores, const std::string& url) {
    UpdateProfile(this->long_term_interests_, scores, 0.8);
    UpdateProfile(this->short_term_interests_, scores, 0.1);

    // check if search url and update search_intent


    return true;
}

void LoadToMap(const rapidjson::Value& vector, std::map<std::string, double>* data) {
    for (rapidjson::Value::ConstMemberIterator itr = vector.MemberBegin(); itr != vector.MemberEnd(); ++itr) {
      double v = vector[itr->name.GetString()].GetDouble();
      (*data)[itr->name.GetString()] = v;
    }
}

void LoadToVector(const rapidjson::Value& vector, std::vector<double>* data) {
    data->clear();
    for (rapidjson::SizeType i = 0; i < vector.Size(); i++) {
        data->push_back(vector[i].GetDouble());
    }
}

std::unique_ptr<UserProfile> UserProfile::FromJSON(const std::string& json) {
    std::unique_ptr<UserProfile> res = std::unique_ptr<UserProfile>(new UserProfile());

    rapidjson::Document d;
    d.Parse(json.c_str());

    if (d.FindMember("long_term_interests") != d.MemberEnd()) {
        LoadToVector(d["long_term_interests"], &(res->long_term_interests_));
    }

    if (d.FindMember("short_term_interests") != d.MemberEnd()) {
        LoadToVector(d["short_term_interests"], &(res->short_term_interests_));
    }

    if (d.FindMember("search_intent") != d.MemberEnd()) {
        LoadToVector(d["search_intent"], &(res->search_intent_));
    }

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

void SerializeVector(std::string key, std::vector<double> data, rapidjson::Writer<rapidjson::StringBuffer>* writer) {
    writer->Key(key.c_str());

    writer->StartArray();
    for ( auto c : data ) {
        writer->Double(c);
    }
    writer->EndArray();
}

const std::string UserProfile::ToJSON() const {
    rapidjson::StringBuffer sb;
    rapidjson::Writer<rapidjson::StringBuffer> writer(sb);

    writer.StartObject();

    SerializeVector("long_term_interests", this->long_term_interests_, &writer);
    SerializeVector("short_term_interests", this->short_term_interests_, &writer);
    SerializeVector("search_intent", this->search_intent_, &writer);

    writer.EndObject();

    return sb.GetString();
}
