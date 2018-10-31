#ifndef BAT_USERMODEL_AD_CATALOG_H_
#define BAT_USERMODEL_AD_CATALOG_H_

#include <vector>
#include <map>

#include "ad.h"

namespace usermodel {

class AdCatalog {
 public:
  AdCatalog();
  ~AdCatalog();

  std::vector<Ad> ads_;
  std::map<std::string, std::vector<Ad> > category_to_ads_;

  bool load(const std::string& json);
};

}  // namespace usermodel

#endif  // BAT_USERMODEL_AD_CATALOG_H_
