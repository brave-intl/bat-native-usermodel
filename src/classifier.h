#include <iostream>
#include "data_point.h"

namespace usermodel{

class Classifier{
    public:
        Classifier();
        ~Classifier();
        std::map<std::string, float> Predict(Data_point x);
};
class Linear_classifier: public Classifier{
    public:
        Linear_classifier();
        // Linear_classifier(Linear_classifier *other);
        Linear_classifier(std::string model);
        Linear_classifier(std::map<std::string, Data_point> weights_, std::map<std::string,float> biases_);
        ~Linear_classifier();
        std::map<std::string, float> Predict(Data_point x);
    private: 
        std::map<std::string, Data_point> weights;
        std::map<std::string,float> biases;
};
}