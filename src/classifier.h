#include <iostream>
#include "data_point.h"
#include <vector>
namespace usermodel{

std::map<std::string, float> Softmax( std::map<std::string, float> y);

class Linear_classifier{
    public:
        Linear_classifier();
        Linear_classifier(Linear_classifier &other);
        Linear_classifier(std::string model);
        Linear_classifier(std::map<std::string, DataPoint> weights_, std::map<std::string,float> biases_);
        ~Linear_classifier();
        std::map<std::string, float> Predict(DataPoint x);
    private: 
        std::map<std::string, DataPoint> weights;
        std::map<std::string,float> biases;
};
}