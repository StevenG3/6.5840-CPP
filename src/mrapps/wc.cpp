#include "../../include/wc.h"

std::vector<KeyValue> mapFunc(const std::string& filename, std::string content) {
	std::vector<KeyValue> kvs;

	std::regex alphabetic("[a-zA-Z]+");
    // Use regex_iterator to split the content based on the alphabetic characters
    std::regex_iterator<std::string::iterator> iter(content.begin(), content.end(), alphabetic);
    std::regex_iterator<std::string::iterator> end;

    while (iter != end) {
        std::string word = iter->str();
        kvs.emplace_back(KeyValue{word, "1"});
        ++iter;
    }

	return kvs;
}

std::string reduceFunc(const std::string& key, const std::vector<std::string>& values) {
	return std::to_string(values.size());
}
