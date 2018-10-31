#include "bat/usermodel/ad.h"

namespace usermodel {

Ad::Ad() = default;
Ad::Ad(const Ad& other) {
  this->category = other.category;
  this->advertiser = other.advertiser;
  this->notification_text = other.notification_text;
  this->notification_url = other.notification_url;
  this->uuid = other.uuid;
}
Ad::~Ad() = default;

}  // namespace usermodel
