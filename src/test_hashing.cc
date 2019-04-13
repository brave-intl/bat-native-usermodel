#include <stddef.h>
#include <utility>
#include <iostream>
#include "hashing_extractor.h"
#include <fstream>
#include <algorithm>
#include <vector>
int main(){
	std::wstring input_value=L"testing, motherfucker";
	usermodel::HashVectorizer vectorizer;
    	vectorizer.Process(input_value);
	auto rez = vectorizer.GetFrequencies();
	std::cout<<"length: "<< rez.size()
	<<"-----------------------------\n";
	auto idx = std::vector<int>();	
	for (auto kv : rez){
		idx.push_back(kv.first);
	}        
	std::sort(idx.begin(), idx.end());
	for(int i = 0; i < idx.size(); i++){
		std::cout<< idx[i]<<": " << rez[idx[i]] <<"\n";
	}
	std::wstring input_value2=L"καλημέρα μαδαφάκα 123";
	std::wstring input_value3=L"kalimera madafaka 123";
	std::cout<<"and the grand finale? "<<input_value2.size()<<
	" "<<  input_value3.size()<<"\n";
	std::wcout<<input_value2<<L".\n";
	for (int i =0; i < input_value2.size();i++){
	 	std::wstring hrmf = input_value2.substr(i, input_value2.size()-i);
		//std::wcout<< input_value2<<L".\n";
		//std::cout<<hrmf.size() << hrmf <<".\n";
	}
	return 0;
}
