 ///
 /// @file    CacheManager.h
 /// @author  mistydew(mistydew@qq.com)
 /// @date    2017-11-09 20:56:02
 ///

#ifndef __CACHEMANAGER_H__
#define __CACHEMANAGER_H__

#include <pthread.h>

#include <vector>
#include <map>

namespace my
{

class LRUCache;

class CacheManager
{
public:
	static void initCache(std::vector<pthread_t> & pthIds, const char * fileName);
	static LRUCache & getCache(pthread_t pthId);
	static void periodicUpdateCaches();
private:
	static std::map<pthread_t, LRUCache> _cacheMap;
};

} // end of namespace my

#endif
