#ifndef DESETS_H
#define DESETS_H

#include <vector>

class DisjointSets 
{
	/* thorough descriptions are in dsets.cpp */
	public:
		void addelements(int num);
		int find(int elem);
		void setunion(int a, int b);
		int size(int elem) const;

	private:
		std::vector<int> elems_;
};


#endif /* DESETS_H */