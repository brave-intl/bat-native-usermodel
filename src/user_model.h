
#include <string>
#include "user_profile.h"
#include "notification_event_type.h"
#include "naive_bayes.h"

namespace usermodel {
    class UserModel {
        public:
        bool initializePageClassifier(const std::string& model);
        void onPageLoad(const std::string& html,const std::string& url, uint32_t window_id, uint32_t tab_id);
        //void onTabFocus(uint32_t window_id, uint32_t tab_id);
        //void onNotificationEvent(NotificationEventType type);
        //void updateState();
        std::vector<double>  classifyPage(const std::string& data);
        std::string winningCategory(std::vector<double> scores);

        private:
        //UserProfile user_profile;
        //bool isSearchURL(const std::string& url);
        //bool isShoppingIntent(const std::string& url, const std::string& html);
        
        NaiveBayes page_classifier;
    };
}

/*

const translate = { 'clicked': 'clicked', 'closed': 'dismissed', 'ignored': 'timeout' }
              
              
onLoad() {
    auto categories = cl.predict(html);

    auto winning_category = cl.winning_category(categories);

    tabs_classification_store.set(window_id, tab_id, winning_category);

    // is shopping?

    // is searching?

    user_profile.update(winning_category)
}

*/