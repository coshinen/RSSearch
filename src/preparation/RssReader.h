 ///
 /// @file    RssReader.h
 /// @author  mistydew(mistydew@qq.com)
 /// @date    2017-11-12 10:18:38
 ///

#ifndef __RSSREADER_H__
#define __RSSREADER_H__

#include <string>
#include <vector>
#include <utility>
#include <unordered_map>
#include <set>

namespace md
{

struct RssItem
{
    std::string _title;
    std::string _link;
    std::string _content;
    uint64_t _simhash;
};

class RssReader
{
public:
    void init(const char * path);
    void parseRss();
    void dump(const char * fileName);
private:
    void parseRss(const char * fileName);
    void deduplication();
    void dumpOffset(const char * fileName);
    void dumpInvert(const char * fileName);
private:
    std::vector<std::string> _docs;
    std::vector<RssItem> _articles;
    std::vector<RssItem> _newArticles;
    std::vector<std::pair<std::size_t, std::size_t> > _offset;
    std::unordered_map<std::string, std::set<std::pair<int, double> > > _invertIndexTable;
};

} // end of namespace md

#endif
