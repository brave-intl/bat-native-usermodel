
namespace usermodel {
    class UserModel {
        public:
        void onLoad(std::string html, uint32_t tab_id, uint32_t windo_id);
        void onTabload(uint32_t tab_id, uint32_t window_id);
        void onNotificationEvent();

        private:
        std::map<std::string, double> longTermInterests;
        std::map<std::string, double> shortTermInterests;
        std::map<std::string, double> searchIntent;
        std::map<std::string, double> shoppingIntent;
    }
}