#ifndef PATTERN_H_
#define PATTERN_H_

#include<vector>

using namespace std;

class Pattern{
	public:
		std::vector<int> elements;
		int support,size;
		Pattern();
		~Pattern();

		// Assignment operator overload
		Pattern &operator=(const Pattern &other) {
			support = other.support;
			size = other.size;
			elements = other.elements;
			return *this;
		}
};
#endif //PATTERN_H_
