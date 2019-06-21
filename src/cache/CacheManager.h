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
    static void initCache(const std::size_t & cacheNum, const char * fileName);
    static LRUCache & getCache(const std::size_t & idx);
    static void periodicUpdateCaches();
private:
    static std::vector<LRUCache> _cacheMap;
};

} // end of namespace my

#endif
