#include "hash-map.h"

int main()
{
	HashMap<int, int> obj;
	obj.append(10, 12);
	srand(time(0));
	for (int i = 0; i < 10; ++i)
	{
		obj.append(i, i);
	}

	obj.append(13, 15);
	
	cout << obj;
	cout << "TEST FIND-METHOD (search by key = 13): \n";
	cout << "value: " << obj[13] << endl;
	obj.remove(13);
	// cout << "value: " << obj[13] << endl;
	cout << "Amount elements in hash-table: " << obj.getAmount() << endl;
	cout << "Max load factor of hash-table:" << obj.getMaxLF() << endl;
	cout << "Current load factor of hash-table:" << obj.getCurLF() << endl;

	
	cout << "QUIT\n";
	return 0;

}