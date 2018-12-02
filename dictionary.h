
#include "HashTable.h"
using namespace std;

template<class HashedObj>
class Dictionary
{
public :
	Dictionary(const HashedObj & notFound, int size);

	void insert(const HashedObj & key);
	const HashedObj & lookup (const HashedObj & key) const;
	void makeEmpty();
private :
	HashTable <HashedObj> items;
};

#include "dictionary.cpp"
