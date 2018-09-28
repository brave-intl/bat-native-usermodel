#include "ad.h"
#include <iostream>
usermodel::Ad::Ad() = default;
usermodel::Ad::Ad(const Ad& other) {
    this->category = other.category;
    this->advertiser = other.advertiser;
    this->notification_text = other.notification_text;
    this->notification_url = other.notification_url;
    this->uuid = other.uuid;
}
usermodel::Ad::~Ad() = default;
