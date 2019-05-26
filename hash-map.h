#include <iostream>
#include <vector>
#include <ctime>
#include <list>
// #include <>
using namespace std;

// template <typename K, typename V, typename Hash = hash<K>> class HashMap;
struct NoSuchKeyException : public exception {
   const char * what () const throw () {
      return "NoSuchKeyException";
   }
};

struct KeyExistsYet : public exception {
   const char * what () const throw () {
      return "KeyExistsYet";
   }
};



template <	typename K,
			typename V,
			typename Hash = hash<K>
		 >
class HashMap
{
class Pair
{
private:
	K _key;
	V _value;
public:
	Pair(K k, V v): _key(k), _value(v){}
	K& getKey()
	{
		return _key;
	}
	V& getValue()
	{
		return _value;
	}
		
};
private:
	uint32_t _len = 16;
	float _loadFactor = 2.0;
	uint32_t _amount = 0;
	
	Hash hashFunc;
	vector<list<Pair>> _chainList;
	void resizeHashMap(uint32_t new_size)
	{
		// resizeCounter++;
		if (new_size < _len) return;
		_len = new_size;
		_chainList.resize(_len);
		for(int i = 0; i < _chainList.size(); ++i)
		{
			int j = 0;
			for (auto iter = _chainList[i].begin(); iter != _chainList[i].end(); ++iter)
			{

				if (j++ != hashFunc(iter->getKey()))
				{
					// хэш элемента не совпадает с нужным индексом цепочки
					auto temp = iter;;
					_chainList[i].erase(iter);
					this->append(temp->getKey(), temp->getValue());
					_amount--;
				}

			}
		}
	}
public:
	// int resizeCounter = 0;
	HashMap(uint32_t size = 16, float loadF = 2.0) : _len(size), _loadFactor(loadF)
	{
		if (!size)
		{
			_len = 16;
		}
		_chainList.resize(_len);
	}
	~HashMap()
	{
		removeAll();
		_chainList.clear();
		_loadFactor = 0;
		_amount = 0;
		_len = 1;
	}

	void append(K key, V value)
	{
		auto hash = hashFunc(key);
		++_amount;
		if (_loadFactor < getCurLF())
		{
			resizeHashMap(2*_len + 1);
		}
		auto idx = hash & (_len - 1);
		if (_len > 0)
		{
			for (auto iter = _chainList[idx].begin(); iter != _chainList[idx].end(); ++iter)
			{
				if (key == iter->getKey())
				{
					throw KeyExistsYet();
					// Какой-нибудь из двух вариантов
					return;
				}
			}
			_chainList[idx].push_back(Pair(key, value));
		}
	}

	float getMaxLF()
	{
		return _loadFactor;
	}

	uint32_t getAmount()
	{
		return _amount;
	}

	float getCurLF()
	{
		return _amount/_len;
	}

	const auto& getChainList()
	{
		return _chainList;
	}

	void setLoadFactor(uint32_t lf)
	{
		_loadFactor = lf;
		if (this->getCurLF() > this->getMaxLF())
			resizeHashMap(2 * _len + 1);
	}

	void remove(K key)
	{
		auto idx = hashFunc(key) & (_len-1);
		bool del_flag = 0;
		for (auto iter = _chainList[idx].begin(); iter != _chainList[idx].end(); ++iter){
			if (iter->getKey() == key)
			{
				_chainList[idx].erase(iter);
				del_flag = true;
				break;
			}
		}
		if (del_flag) --_amount;
		else throw (NoSuchKeyException());
		// не понятно, что с указателями в массиве - освобождается ли память?
	}
	void removeAll()
	{
		for (int i = 0; i < _chainList.size(); ++i)
		{
			_chainList[i].clear();
		}
		_amount = 0;
		_loadFactor = 2.0;
	}

	V& operator[](const K key)
	{
		auto idx = hashFunc(key) & (_len-1);
		for (auto element: _chainList[idx])
		{
			if (element.getKey() == key)
			{
				return element.getValue();
			}
		}
		throw NoSuchKeyException();
	}



};

template <	typename K,
			typename V,
			typename Hash = hash<K>
		 >
	std::ostream& operator<< (std::ostream &out, HashMap<K, V, Hash> obj)
	{
		auto _chainList = obj.getChainList();
		for (int i = 0; i < _chainList.size(); ++i)
		{
			for (auto iter = _chainList[i].begin(); iter != _chainList[i].end(); ++iter)
			{
				out << iter->getKey() << ": " << iter->getValue() << endl;
			}
		}
		return out;
	}