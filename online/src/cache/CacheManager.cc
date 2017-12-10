 ///
 /// @file    CacheManager.cc
 /// @author  mistydew(mistydew@qq.com)
 /// @date    2017-11-09 21:08:15
 ///

#include "./cache/CacheManager.h"
#include "./cache/LRUCache.h"
#include "Configuration.h"
#include "./timer/TimerThread.h"

#include <unistd.h>

#include <iostream>
#include <functional>

namespace my
{

std::vector<LRUCache> CacheManager::_cacheMap;

void CacheManager::initCache(const std::size_t & cacheNum, const char * fileName)
{
	LRUCache lruCache;
	lruCache.initFromFile(fileName);
	//这里需要获取每个计算线程的ID初始化每个线程ID对应的cache。
	std::cout << "---------------------->" << std::endl;
	_cacheMap.reserve(cacheNum);
	for (std::size_t idx = 0; idx != cacheNum; ++idx)
	{
		std::cout << "init --------> cache " << idx << std::endl;
		_cacheMap.push_back(lruCache);
	}
	std::cout << "initCache completed" << std::endl;
}

LRUCache & CacheManager::getCache(const std::size_t & idx)
{ return _cacheMap[idx]; }

void CacheManager::periodicUpdateCaches()
{
	std::vector<LRUCache>::iterator it = _cacheMap.begin();
	LRUCache & temp = *it;
	for (++it; it != _cacheMap.end(); ++it)
	{
		temp.update(*it);
	}

	it = _cacheMap.begin();
	for (++it; it != _cacheMap.end(); ++it)
	{
		(*it).update(temp);
	}

	temp.dumpToFile(Configuration::getInstance()->getConfigMap()["cache"].c_str());
	std::cout << "periodicUpdateCaches and writeToFile is completed" << std::endl;
}

} // end of namespace my
