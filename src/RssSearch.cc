 ///
 /// @file    RssSearch.cc
 /// @author  mistydew(mistydew@qq.com)
 /// @date    2017-12-02 17:04:55
 ///

#include "RssSearch.h"
#include "Configuration.h"
#include "WordSegmentation.h"
#include "cache/LRUCache.h"
#include "cache/CacheManager.h"
#include "bo_threadpool/Thread.h"

#include "json/json.h"
#include "cpp_redis/cpp_redis"

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

namespace my
{

RssSearch::RssSearch()
{
    std::cout << "RssSearch()" << std::endl;
    loadLibrary();
}

RssSearch::~RssSearch()
{ std::cout << "~RssSearch()" << std::endl; }

std::string RssSearch::doQuery(const std::string & query)
{
    std::cout << "query: " << query << std::endl;
    /* word segmentation and remove stop words */
    std::vector<std::string> words;
    WordSegmentation::getInstance()->cut(query, words);

    std::size_t nBytes;
    std::vector<std::string> queryWords;
    for (auto & word : words)
    {
        if(0 == Configuration::getInstance()->getStopWord().count(word)) {
            nBytes = nBytesUTF8Code(word[0]);
            if (0 == nBytes) {
                word = processWords(word);
                if ("" == word)
                    continue;
            }

            queryWords.push_back(word);
        }
    }

#if 0
    for (auto & word : queryWords)
    {
        std::cout << word << std::endl;
        if (0 == _invertIndexTable.count(word)) {
            std::cout << "No such result" << std::endl;
            return noAnswer();
        }
    }
#endif

    /* get query words weight vector */
    std::vector<double> weightVec;
    getQueryWordsWeightVector(queryWords, weightVec);
    SimilarityCompare similarityCompare(weightVec);

    /* execute query */
    //LRUCache & lruCache = CacheManager::getCache(str2uint(curthread::threadName));
    std::vector<std::pair<std::size_t, std::vector<double> > > resultVec;
    std::string result;
    if (executeQuery(queryWords, resultVec)) {
        std::stable_sort(resultVec.begin(), resultVec.end(), similarityCompare);
        std::vector<std::size_t> docIdVec;
        for (auto & result : resultVec)
        {
            docIdVec.push_back(result.first);
        }
        result = makeJson(docIdVec, queryWords);
        //lruCache.setLRUCache(query, result);
        //return result; 
        //return makeJson(docIdVec, queryWords); 
    } else {
        result = noAnswer(); 
        //lruCache.setLRUCache(query, result);
        //return result;
        //return noAnswer();
    }

    cpp_redis::client client;
    client.connect("192.168.40.128", 6379, [](const std::string& host, std::size_t port, cpp_redis::client::connect_state status) {
        if (status == cpp_redis::client::connect_state::dropped) {
            std::cout << "client disconnected from " << host << ":" << port << std::endl;
        }
    });

    client.set(query, result, [](cpp_redis::reply& reply) {
        std::cout << "set cache: " << reply << std::endl;
    });

    // synchronous commit, no timeout
    client.sync_commit();
    return result;
}

void RssSearch::showPageLib() const
{
    for (auto & map : _pageLib)
    {
        std::cout << map.first << std::endl
            << map.second << std::endl;
    }
}

void RssSearch::showOffsetLib() const
{
    for (auto & map : _offsetLib)
    {
        std::cout << map.first << " " << map.second.first << " " << map.second.second << std::endl;
    }
}

void RssSearch::showInvertIndexLib() const
{
    for (auto & map : _invertIndexTable)
    {
        std::cout << map.first;
        for (auto & pair : map.second)
        {
            std::cout << " " << pair.first << " " << pair.second;
        }
        std::cout << std::endl;
    }
}

void RssSearch::loadLibrary()
{
    /* load ripe page library */
    std::cout << "load library..." << std::endl;
    std::ifstream ifsPageLib(Configuration::getInstance()->getConfigMap()[PAGE_LIB_PATH].c_str());
    if (!ifsPageLib.good()) {
        std::cout << "ifstream open file failed" << std::endl;
        return;
    }

    std::ostringstream oss;
    oss << ifsPageLib.rdbuf();
    std::string pagelib = oss.str();

    ifsPageLib.close();

    /* load page offset library */
    std::ifstream ifsOffset(Configuration::getInstance()->getConfigMap()[OFFSET_LIB_PATH].c_str());
    if (!ifsOffset.good()) {
        std::cout << "ifstream open file failed" << std::endl;
        return;
    }

    std::size_t idocId;
    std::size_t ibegin;
    std::size_t ilength;
    std::string line;
    while (getline(ifsOffset, line))
    {
        std::istringstream iss(line);
        iss >> idocId >> ibegin >> ilength;
        std::cout << "docId: " << idocId << " ";
        //_pageLib[idocId] = WebPage(pagelib.substr(ibegin, ilength)); // There is overhead for copying temporary object.
        //_offsetLib[idocId] = std::make_pair(ibegin, ilength);
        _pageLib.insert(std::make_pair(idocId, pagelib.substr(ibegin, ilength))); // && rvalue reference
        _offsetLib.insert(std::make_pair(idocId, std::make_pair(ibegin, ilength)));
    }

    ifsOffset.close();

    /* load invert index library */
    std::ifstream ifsInvertIndex(Configuration::getInstance()->getConfigMap()[INVERT_LIB_PATH].c_str());
    if (!ifsInvertIndex.good()) {
        std::cout << "ifstream open file failed" << std::endl;
        return;
    }

    std::string word;
    double dweight;
    while (getline(ifsInvertIndex, line))
    {
        std::istringstream iss(line);
        iss >> word;
        while (iss >> idocId >> dweight)
        {
            _invertIndexTable[word].insert(std::make_pair(idocId, dweight));
        }
    }

    ifsInvertIndex.close();
    std::cout << "load library file completed" << std::endl;
}

std::string RssSearch::processWords(const std::string & word)
{
    std::string temp;
    for (std::string::size_type idx = 0; idx != word.size(); ++idx)
    {
        if (isalpha(word[idx])) {
            if (isupper(word[idx]))
                temp += word[idx] + 32;
            else
                temp += word[idx];
        }
    }
    return temp;
}

void RssSearch::getQueryWordsWeightVector(const std::vector<std::string> & queryWords, std::vector<double> & weightVec)
{
    /* statistic term frequency */
    std::map<std::string, std::size_t> termFreq;
    for (auto & word : queryWords)
    {
        ++termFreq[word];
    }

    /* calculate weight */
    std::size_t docSum = _pageLib.size();
    double weightSum = 0;
    for (auto & word : queryWords)
    //for (auto & map : termFreq)
    {
        std::size_t tf = termFreq[word]; // tf: term frequency
        std::size_t df = _invertIndexTable[word].size(); // df: document frequency
        //std::size_t tf = map.second; // tf: term frequency
        //std::size_t df = _invertIndexTable[map.first].size(); // df: document frequency
        double idf = std::log(static_cast<double>(docSum) / df + 0.05) / std::log(2); // idf: inverse document frequency
        double weight = static_cast<double>(tf) * idf;
        weightVec.push_back(weight);
        weightSum += std::pow(weight, 2);
    }

    /* normalization processing */
    for (auto & weight : weightVec)
    {
        weight /= std::sqrt(weightSum);
    }

    std::cout << "word:" << std::endl;
    for (auto & word : queryWords)
    {
        std::cout << word << " ";
    }
    std::cout << std::endl;
    std::cout << "word's weight:" << std::endl;
    for (auto & weight : weightVec)
    {
        std::cout << weight << " ";
    }
    std::cout << std::endl;
}

bool RssSearch::executeQuery(const std::vector<std::string> & queryWords, std::vector<std::pair<std::size_t, std::vector<double> > > & resultVec)
{
    std::cout << "executeQuery()" << std::endl;
    if (0 == queryWords.size()) {
        std::cout << "Query word is a empty string" << std::endl;
        return false;
    }

    typedef std::set<std::pair<std::size_t, double> >::iterator iterSet;
    std::vector<std::pair<iterSet, int> > iterVec;
    int minIterNum = 0x7FFFFFFF;
    for (auto & word : queryWords)
    {
        int size = _invertIndexTable[word].size();
        if (0 == size)
            return false;

        if (minIterNum > size)
            minIterNum = size;

        iterVec.push_back(std::make_pair(_invertIndexTable[word].begin(), 0));
    }
    std::cout << "minIterNum = " << minIterNum << std::endl;

    /* make intersection */
    bool isExit = false;
    while (!isExit)
    {
        std::size_t idx = 0;
        for (; idx != iterVec.size() - 1; ++idx)
        {
            if (iterVec[idx].first->first != iterVec[idx + 1].first->first)
                break;
        }

        if (iterVec.size() - 1 == idx) { // 所有查询词对应iterator的第一个元素(docId, weight)的id均相同的情况。
            /* 把所有查询词的weight打包至weightVec，同时记录docId，放入结果集中 */
            std::vector<double> weightVec;
            std::size_t docId = iterVec[0].first->first;
            for (idx = 0; idx != iterVec.size(); ++idx)
            {
                weightVec.push_back(iterVec[idx].first->second);
                ++iterVec[idx].first;
                ++iterVec[idx].second;
                if (minIterNum == iterVec[idx].second)
                    isExit = true;
            }
            resultVec.push_back(std::make_pair(docId, weightVec));
        } else { // 所有查询词对应iterator的第一个元素(docId, weight)的id存在不同的情况。
            /* 找到docId最小的迭代器，将其位置后移一位 */
            int minDocId = 0x7FFFFFFF;
            int iterIdx;
            for (idx = 0; idx != iterVec.size(); ++idx)
            {
                if (minDocId > static_cast<int>(iterVec[idx].first->first)) {
                    minDocId = static_cast<int>(iterVec[idx].first->first);
                    iterIdx = idx;
                }
            }
            ++iterVec[iterIdx].first;
            ++iterVec[iterIdx].second;
            if (minIterNum == iterVec[iterIdx].second)
                isExit = true;
        }
    }

    return true;
}

std::string RssSearch::makeJson(const std::vector<std::size_t> & docIdVec, const std::vector<std::string> & queryWords)
{
    Json::Value arr;
    std::size_t count = 0;
    for (auto & docId : docIdVec)
    {
        Json::Value elem;
        elem["title"] = _pageLib[docId].getDocTitle();
        elem["summary"] = _pageLib[docId].summary(queryWords);
        elem["url"] = _pageLib[docId].getDocUrl();
        arr.append(elem);

        if (20 == ++count)
            break;
    }

    Json::Value root;
    root["files"] = arr;

    Json::StyledWriter writer;
    return writer.write(root);
}

std::string RssSearch::noAnswer()
{
    Json::Value elem;
    elem["title"] = "404 Not Found";
    elem["summary"] = "I cannot find what you want";
    elem["url"] = "";

    Json::Value arr;
    arr.append(elem);

    Json::Value root;
    root["files"] = arr;

    Json::StyledWriter writer;
    return writer.write(root);
}

} // end of namespace my
