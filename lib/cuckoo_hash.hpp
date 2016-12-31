/*************************************************************************
  > File Name: lib/cuckoo_hash.hpp
  > Author: stmatengss
  > Mail: stmatengss@163.com 
  > Created Time: Sat Dec 31 14:14:47 2016
 ************************************************************************/

#include "header.hpp"

template<typename T, typename P>
class cuckoo_hash {
	typedef long long ll;
	typedef unsigned int uint;

	public:
	vector<pair<T, P> > *table1;
	vector<pair<T, P> > *table2;
	map<T, P> *extra_map;
	int n_size;
	int n_count;
	int n_per_size;
	P init_value;
	T init_key;

	private:
	double _loadfactor;

	private:
	bool rehash(uint key, uint position) {
		uint new_key;
		if (position == 1) new_key = hash_fun1(lexical_cast<string>(key).c_str());
		if (position == 2) new_key = hash_fun2(lexical_cast<string>(key).c_str());
		if (position == 1 && (*table1)[key].first == -1) {
			(*table2)[new_key] = (*table1)[key];
			return true;
		}
		if (position == 2 && (*table1)[key].first == -1) {
			(*table1)[new_key] = (*table2)[key];
			return true;
		}
		if (rehash(new_key, position == 1?2:1)) {
			if (position == 1) (*table2)[new_key] = (*table1)[key];
			if (position == 2) (*table1)[new_key] = (*table2)[key];
			return true;
		}
		return false;
	}
	// SDBMHash
	unsigned int hash_fun1(char *str) {
		unsigned int hash = 0;

		while (*str)
		{

			hash = 65599*hash + (*str++);
			hash = (*str++) + (hash << 6) + (hash << 16) - hash;
		}
		return (hash & 0x7FFFFFFF) % n_per_size;
	}

	//RSHash
	unsigned int hash_fun2(char *str) {
		unsigned int b = 378551;
		unsigned int a = 63689;
		unsigned int hash = 0;

		while (*str)
		{

			hash = hash * a + (*str++);
			a *= b;
		}

		return (hash & 0x7FFFFFFF) % n_per_size;
	}

	public:
	cuckoo_hash() {
		cout << "init_cuckoo_hash" <<endl;
	}
	cuckoo_hash(int _n_size,T _init_key, P _init_value):
		n_size(_n_size), init_key(_init_key), init_value(_init_value) {
		cout << "init_cuckoo_hash" <<endl;
		n_per_size = n_size / 2;
		extra_map = new map<T, P>();
		auto init_pair = make_pair<T, P>(init_key, init_value);
		table1 = new vector<pair<T, P> >(n_per_size, init_pair);
		table2 = new vector<pair<T, P> >(n_per_size, init_pair);
	}

	~cuckoo_hash() {
		delete table1, table2;
	}

	int size() {
		return n_size;
	}

	int count() {
		return n_count;
	}

	double loadfactor() {
		_loadfactor = (double)n_count/n_size;
		return _loadfactor;
	}

	P operator[] (T index) {
		return find(index);
	}

	P find(T _key) {
		string key = lexical_cast<string>(_key);
		uint key1 = hash_fun1(key.c_str());
		uint key2 = hash_fun2(key.c_str());
		if (_key == (*table1)[key1].first) {
			return (*table1)[key1].second;
		}
		if (_key == (*table2)[key2].first){
			return (*table2)[key2].second;
		} 
		if (extra_map->find(_key) != extra_map->end()) {
			return (*extra_map)[key];
		}
		return init_value;
		//TODO throw exception
	}

	bool insert(T _key, P _value) {
		string key = lexical_cast<string>(_key);
		uint key1 = hash_fun1(key);
		uint key2 = hash_fun2(key);
		auto key_value = make_pair(_key, _value);
		if ((*table1)[key1].first == init_key || (*table1)[key1].first = _key) {
			(*table1)[key1] = key_value;
			return true;
		}
		if ((*table2)[key2].first == init_key || (*table2)[key2].first = _key) {
			(*table2)[key2] = key_value;
			return true;
		}
		if (rehash(key1, 1)) {
			(*table1)[key1] = key_value;
			return true;
		}
		if (rehash(key2, 2)) {
			(*table2)[key2] = key_value;
			return true;
		}
		if ((*extra_map)[_key] = _value) {
			return true;
		}
		return false;
	}

	void test() {
		//cout << find("haha") << endl;
	}

	
};
