#include "Hash.h"


int main()
{

	Hash_table<int, int> ht;
	int a[] = { 1,111,4,7,15,25,44,9 };
	for (auto& e : a)
	{
		ht.insert(make_pair(e, e));
	}
	ht.Print();

	ht.erase(15);
	ht.erase(25);

	ht.Print();


}