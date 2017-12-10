 ///
 /// @file    LRUCache.cc
 /// @author  mistydew(mistydew@qq.com)
 /// @date    2017-11-27 19:55:44
 ///

#include "./cache/LRUCache.h"

#include <iostream>
#include <fstream>
#include <sstream>

namespace my
{

/* create a LRU cache */
LRUCache::LRUCache(const std::size_t & capacity)
: _capacity(capacity)
, _hashMap()
, _lruList()
{ std::cout << "LRUCache()" << std::endl; }

/* duplicate a LRU cache */
LRUCache::LRUCache(const LRUCache & rhs)
: _capacity(rhs._capacity)
{
	update(rhs);
	std::cout << "LRUCache(const LRUCache &)" << std::endl;
}

/* duplicate a LRU cache */
LRUCache & LRUCache::operator=(const LRUCache & rhs)
{
	std::cout << "operator=(const LRUCache &)" << std::endl;
	if (this != &rhs) {
		_capacity = rhs._capacity;
		update(rhs);
		//_hashMap = rhs._hashMap; // error，这里迭代器不能复制，因为复制后还是原迭代器，指向原list。
		//_lruList = rhs._lruList;
	}
	return *this;
}

/* free a LRU cache */
LRUCache::~LRUCache()
{ std::cout << "~LRUCache()" << std::endl; }

/***********************
 * LRU cache operation *
 ***********************/

/* set data to LRU cache */
bool LRUCache::setLRUCache(const std::string & key, const std::string & data)
{
	std::cout << "setLRUCache() " << key << " --> " << data << std::endl;
	auto it = _hashMap.find(key); // search key from hash map
	if (it != _hashMap.end()) { // exist
		_hashMap[key]->_data = data; // update
		updateLRU(key);
	} else { // does not exist
		if (_capacity == _lruList.size()) {
			_hashMap.erase(_lruList.back()._key);
			_lruList.pop_back();
		}
		_lruList.push_front(CacheEntry(key, data));
		_hashMap[key] = _lruList.begin();
	}
	std::cout << "setLRUCache() sucess" << std::endl;
	return 0;
}

/* get data from LRU cache */
std::string LRUCache::getLRUCache(const std::string & key)
{
	auto it = _hashMap.find(key); // search key from hash map
	if (it != _hashMap.end()) { // exist
		updateLRU(key);
		return _hashMap[key]->_data;
	} else { // does not exist
		return "";
	}
}

/* print LRU cache list */
void LRUCache::printLRUCache() const
{
	std::cout << "\n>>>>>>>>\n"
		<< "cache:\n"
		<< "key --> data:" << std::endl;
	for (auto & entry : _lruList)
	{
		std::cout << entry._key << " --> " << entry._data << std::endl;
	}
	std::cout << "\n>>>>>>>>\n" << std::endl;
}

/* initialize LRU cache from file */
void LRUCache::initFromFile(const char * fileName)
{
	std::ifstream ifs(fileName);
	if (!ifs.good()) {
		std::cout << "ifstream open file filed!" << std::endl;
		return;
	}

	std::string line;
	std::cout << "add Cache" << std::endl;
	while (getline(ifs, line))
	{
		std::istringstream iss(line);
		std::string key;
		std::string value1;
		std::string value2;
		std::string value3;
		iss >> key >> value1 >> value2 >> value3;
		std::string value;
		if (value1 != "")
			value = value1 + " " + value2 + " " + value3;
		std::cout << "key = " << key << std::endl
			<< "value = " << value << std::endl;
		setLRUCache(key, value);
	}

	ifs.close();
}

/* dump LRU cache list to file */
void LRUCache::dumpToFile(const char * fileName) const
{
	std::ofstream ofs(fileName);
	if (!ofs.good()) {
		std::cout << "ofstream open file filed!" << std::endl;
		return;
	}

	for (auto & entry : _lruList)
	{
		ofs << entry._key << " " << entry._data << std::endl;
	}

	ofs.close();
}

/* update entire LRU cache */
void LRUCache::update(const LRUCache & rhs)
{
	for (auto & entry : rhs._lruList)
	{
		setLRUCache(entry._key, entry._data);
	}
}

/* query LRU cache via key, return the number of elements matching specific key */
std::size_t LRUCache::count(const std::string & key) const
{ return _hashMap.count(key); }

/* update one of the LRU cache entry */
void LRUCache::updateLRU(const std::string & key)
{
	_lruList.splice(_lruList.begin(), _lruList, _hashMap[key]);
	_hashMap[key] = _lruList.begin();
}

} // end of namespace my
