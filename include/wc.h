#ifndef __FUNC_H__
#define __FUNC_H__

#include "util.h"

struct KeyValue {
	std::string key;
	std::string value;

	bool operator<(const KeyValue& kv2) const {
        return key < kv2.key;
    }
};

extern "C" {
	std::vector<KeyValue> mapFunc(const std::string& filename, std::string content);
	std::string reduceFunc(const std::string& key, const std::vector<std::string>& values);
}

#endif // __FUNC_H__