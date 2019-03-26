#include "hashing_extractor.h"
#include <iostream>
#include <assert.h>
#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>
#include "rapidjson/stringbuffer.h"
#include <rapidjson/writer.h>

#include <fstream>
int main(){
	std::ifstream ifs("hash_check.json");
	rapidjson::IStreamWrapper isw(ifs);
	rapidjson::Document d;
	d.ParseStream(isw);

	assert(d.IsObject());
	assert(d.HasMember("testcases"));
	rapidjson::Value &testcases= d["testcases"];
	for (rapidjson::Value::ConstValueIterator itr = testcases.Begin(); itr != testcases.End(); ++itr) { 
		if (itr->HasMember("input")) { 
    	std::cout<< (*itr)["input"].GetString()<<'\n'; 
	}}
	return 0;
}
