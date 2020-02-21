#ifndef TRANSFORMATION_H
#define TRANSFORMATION_H
#include <iostream> 
#include "data_point.h"
#include "hashing_extractor.h"
#include <vector>
namespace usermodel{

enum class TransformationType{
    TO_LOWER = 1,
    HASHED_NGRAMS=2,
    NORMALIZE=3
};

class Transformation{
    public:
        Transformation();
        Transformation(TransformationType t);
        Transformation(std::string parameters);
        ~Transformation();
        DataPoint get(DataPoint inp);
        TransformationType getType();
    protected:
        HashVectorizer hash_vectorizer;
        TransformationType type;
    private:
        DataPoint get_lower(DataPoint inp);
        DataPoint get_ngrams(DataPoint inp);
        DataPoint get_normalized(DataPoint inp);

};

class To_lower: public Transformation{
    public:
        To_lower();
        // DataPoint get(DataPoint inp);
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
        // DataPoint get(DataPoint inp);
};

}
#endif