#include <iostream>
#include <fstream>

#include "util.h"

/*Read document into a string*/
void load_data(const std::string& filepath, std::string& str) {
	std::ifstream f(filepath, std::ios_base::binary);

	if (!f.good()) {
		std::cout << "Invalid filepath! ";
		return;
	}

	f.seekg(0, std::ios_base::end);
	str.reserve(f.tellg());
	f.seekg(0, std::ios_base::beg);
	str.assign((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());

	return;
}