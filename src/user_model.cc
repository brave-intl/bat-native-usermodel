#include "user_model.h"
#include "bag_of_words_extractor.h"

bool usermodel::UserModel::initializePageClassifier(const std::string& model) {
    return page_classifier.LoadModel(model);
}

std::string usermodel::UserModel::winningCategory(std::vector<double> scores) {
    return page_classifier.WinningCategory(scores);
}

std::vector<double>  usermodel::UserModel::classifyPage(const std::string& data) {
    usermodel::BagOfWords bow;
    bow.Process(data);
    return page_classifier.Predict(bow.GetFrequencies());
}

void usermodel::UserModel::onPageLoad(const std::string& html, const std::string& url, uint32_t window_id, uint32_t tab_id) {
    auto scores = this->classifyPage(html);
    // user_profile.update(scores, url);
    // tabs_classification_store.set(window_id, tab_id, winning_category);
}
