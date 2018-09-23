#ifndef INCLUDE_AD_CATALOG_H_
#define INCLUDE_AD_CATALOG_H_

#include <vector>
#include <map>

#include "ad.h"

namespace usermodel {

class AdCatalog {
 public:
    AdCatalog();
    ~AdCatalog();

    std::vector<Ad> ads_;
    std::map<std::string, std::vector<Ad>> category_to_ads_;

    bool load(const std::string& json);
};
}  // namespace usermodel

#endif  // INCLUDE_AD_CATALOG_H_
