#include "log.h"
#include "wc.h"

// load the application Map and Reduce functions
// from a plugin file, e.g. ../mrapps/wc.so
// TODO: seek for ways to use std::function
using MapFunc = std::vector<KeyValue>(*)(const std::string&, std::string);
using ReduceFunc = std::string(*)(const std::string&, const std::vector<std::string>&);
using DlHandle = void*;

std::tuple<DlHandle, MapFunc, ReduceFunc> loadPlugin(const std::string& filename) {
    // Open the plugin file
    DlHandle handle = dlopen(filename.c_str(), RTLD_LAZY);
    if (!handle) {
        std::cerr << "Cannot load plugin " << filename << ": " << dlerror() << std::endl;
        throw std::runtime_error("Plugin load failure");
    }

    // Load the Map function
	MapFunc mapf = reinterpret_cast<MapFunc>(dlsym(handle, "mapFunc"));
    if (!mapf) {
        std::cerr << "Cannot find Map in " << filename << ": " << dlerror() << std::endl;
        dlclose(handle);
        throw std::runtime_error("Map function missing in plugin");
    }

    // Load the Reduce function
	ReduceFunc reducef = reinterpret_cast<ReduceFunc>(dlsym(handle, "reduceFunc"));
    if (!reducef) {
        std::cerr << "Cannot find Reduce in " << filename << ": " << dlerror() << std::endl;
        dlclose(handle);
        throw std::runtime_error("Reduce function missing in plugin");
    }

    return {handle, mapf, reducef};
}

int main(int argc, char** argv) {
	if(argc < 3){
		log(LogLevel::ERROR, "Usage: mrsequential xxx.so inputfiles...\n");
		std::exit(1);
	}

	// auto mapf = std::bind(&mapFunc, std::placeholders::_1, std::placeholders::_2);
	// auto reducef = std::bind(&reduceFunc, std::placeholders::_1, std::placeholders::_2);

	auto [dlh, mapf, reducef] = loadPlugin(argv[1]);

	std::vector<KeyValue> intermediate;
	for(int i = 2; i < argc; ++i) {
		std::string filename = argv[i];
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Cannot open " << filename << std::endl;
            return 1;
        }

        std::stringstream buffer;
        buffer << file.rdbuf();
        std::string content = buffer.str();
        file.close();

		std::vector<KeyValue> kva = mapf(filename, content);
        intermediate.insert(intermediate.end(), kva.begin(), kva.end());
	}

	sort(intermediate.begin(), intermediate.end());

	auto oname = "mr-out-0";
	std::ofstream ofile(oname);

	int i = 0;
	while(i < intermediate.size()) {
		int j = i + 1;
		while(j < intermediate.size() && intermediate[j].key == intermediate[i].key) {
			j++;
		}
		std::vector<std::string> values;
		for(int k = i; k < j; k++) {
			values.emplace_back(intermediate[k].value);
		}
		std::string output = reducef(intermediate[i].key, values);

		ofile << intermediate[i].key << ' ' << output << '\n';

		i = j;
	}

	ofile.close();
	dlclose(dlh);
}
