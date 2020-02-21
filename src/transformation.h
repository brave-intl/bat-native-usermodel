#ifndef TRANSFORMATION_H
#define TRANSFORMATION_H
#include <iostream> 
#include "data_point.h"
#include "hashing_extractor.h"
#include <vector>
namespace usermodel{

enum class transformation_type{
    TO_LOWER = 1,
    HASHED_NGRAMS=2,
    NORMALIZE=3
};

class Transformation{
    public:
        Transformation();
        Transformation(transformation_type t);
        Transformation(std::string parameters);
        ~Transformation();
        Data_point get(Data_point inp);
        transformation_type getType();
    protected:
        HashVectorizer hash_vectorizer;
        transformation_type type;
    private:
        Data_point get_lower(Data_point inp);
        Data_point get_ngrams(Data_point inp);
        Data_point get_normalized(Data_point inp);

};

class To_lower: public Transformation{
    public:
        To_lower();
        // Data_point get(Data_point inp);
};

class Hashed_ngrams: public Transformation{
    public:
        Hashed_ngrams();
        Hashed_ngrams(int n_b, std::vector<int> subgrams);
        Hashed_ngrams(std::string parameters);
};

class Normalize: public Transformation{
    public:
        Normalize();
        // Data_point get(Data_point inp);
};

}
#endif