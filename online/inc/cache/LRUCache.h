 ///
 /// @file    LRUCache.h
 /// @author  mistydew(mistydew@qq.com)
 /// @date    2017-11-27 19:55:44
 ///

#ifndef __LRUCACHE_H__
#define __LRUCACHE_H__

#include <string>
#include <unordered_map>
#include <list>

namespace my
{

/* LRU cache unit */
struct CacheEntry
{
	CacheEntry(const std::string & key, const std::string & data)
	: _key(key)
	, _data(data)
	{}
	CacheEntry(const CacheEntry & rhs)
	: _key(rhs._key)
	, _data(rhs._data)
	{}

	std::string _key;
	std::string _data;
};

/* LRU cache */
class LRUCache
{
public:
	LRUCache(const std::size_t & capacity = 65536);
	LRUCache & operator=(const LRUCache & rhs);
	~LRUCache();

	bool setLRUCache(const std::string & key, const std::string & data);
	std::string getLRUCache(const std::string & key);
	void printLRUCache() const;
	
	void initFromFile(const char * fileName);
	void dumpToFile(const char * fileName) const;
	void update(const LRUCache & rhs);
	std::size_t count(const std::string & key) const;
private:
	void updateLRU(const std::string & key);
private:
	std::size_t _capacity;
	std::unordered_map<std::string, std::list<CacheEntry>::iterator> _hashMap;

	std::list<CacheEntry> _lruList;
};

} // end of namespace my

#endif
