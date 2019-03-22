#include "hashing_extractor.h"
#include <iostream>
int main(){
	usermodel::HashVectorizer vectorizer;
        auto test_string = "eenie meenie minie mo";
	auto status = vectorizer.Process(test_string);
	if (status){
		std::cout<< "success\n";
	}
	auto counts = vectorizer.GetFrequencies();
	for (auto const& x : counts)
	{
    		std::cout << x.first  // string (key)
              	<< ':' 
              	<< x.second // string's value 
              	<< std::endl ;
}
	
	return 0;
}
