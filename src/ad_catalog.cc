#include <iostream>

#include "ad_catalog.h"

#include "rapidjson/document.h"
#include "rapidjson/error/en.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

usermodel::AdCatalog::AdCatalog() = default;
usermodel::AdCatalog::~AdCatalog() = default;

bool usermodel::AdCatalog::load(const std::string& json) {
    rapidjson::Document d;

    d.Parse(json.c_str());
    assert(d.IsObject());

    const rapidjson::Value& categories = d["categories"];
    assert(categories.IsObject());

    for (rapidjson::Value::ConstMemberIterator itr = categories.MemberBegin(); itr != categories.MemberEnd(); ++itr) {
        const std::string& category = itr->name.GetString();
        const rapidjson::Value& ads = itr->value;
        assert(ads.IsArray());
        for (rapidjson::SizeType i = 0; i < ads.Size(); i++) {
            const rapidjson::Value& ad = ads[i];
            assert(ad.IsObject());
            Ad new_ad;
            new_ad.advertiser = ad["advertiser"].GetString();
            new_ad.notification_text = ad["notificationText"].GetString();
            new_ad.notification_url = ad["notificationURL"].GetString();
            new_ad.uuid = ad["uuid"].GetString();
            new_ad.category = category;

            ads_.push_back(new_ad);
            if (category_to_ads_.find(category) == category_to_ads_.end()) {
                category_to_ads_.insert(std::pair<std::string, std::vector<Ad>>(category, std::vector<Ad>()));
            }
            category_to_ads_[category].push_back(new_ad);
        }
    }

    return true;
}
