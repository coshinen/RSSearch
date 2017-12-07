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

std::map<::pthread_t, LRUCache> CacheManager::_cacheMap;

void CacheManager::initCache(std::vector<pthread_t> & pthIds, const char * fileName)
{
	LRUCache lruCache;
	lruCache.initFromFile(fileName);
	//这里需要获取每个计算线程的ID初始化每个线程ID对应的cache。
	for (std::size_t idx = 0; idx != pthIds.size(); ++idx)
	{
		_cacheMap[pthIds[idx]] = lruCache;
	}
	std::cout << "initCache completed" << std::endl;
}

LRUCache & CacheManager::getCache(pthread_t pthId)
{ return _cacheMap[pthId]; }

void CacheManager::periodicUpdateCaches()
{
	std::map<pthread_t, LRUCache>::iterator cit = _cacheMap.begin();
	LRUCache & temp = (*cit).second;
	for (++cit; cit != _cacheMap.end(); ++cit)
	{
		temp.update((*cit).second);
	}

	cit = _cacheMap.begin();
	for (++cit; cit != _cacheMap.end(); ++cit)
	{
		(*cit).second.update(temp);
	}

	temp.dumpToFile(Configuration::getInstance()->getConfigMap()["cache"].c_str());
	std::cout << "periodicUpdateCaches and writeToFile is completed" << std::endl;
}

} // end of namespace my
