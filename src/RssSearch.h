 ///
 /// @file    RssSearch.h
 /// @author  mistydew(mistydew@qq.com)
 /// @date    2017-12-02 16:54:08
 ///

#ifndef __RSSSEARCH_H__
#define __RSSSEARCH_H__

#include "WebPage.h"

#include <cmath>
#include <string>
#include <unordered_map>
#include <set>
#include <vector>

namespace md
{

struct SimilarityCompare
{
    SimilarityCompare(std::vector<double> & base)
    : _base(base)
    {}

    bool operator()(const std::pair<std::size_t, std::vector<double> > & lhs,
            const std::pair<std::size_t, std::vector<double> > & rhs)
    {
        double lhsCrossProduct = 0;
        double lhsVectorLength = 0;
        double rhsCrossProduct = 0;
        double rhsVectorLength = 0;

        for (std::size_t idx = 0; idx != _base.size(); ++idx)
        {
            lhsCrossProduct += lhs.second[idx] * _base[idx];
            lhsVectorLength += std::pow(lhs.second[idx], 2);
            rhsCrossProduct += rhs.second[idx] * _base[idx];
            rhsVectorLength += std::pow(rhs.second[idx], 2);
        }

        if (lhsCrossProduct / std::sqrt(lhsVectorLength) > rhsCrossProduct / std::sqrt(rhsVectorLength))
            return true;
        else
            return false;
    }

    std::vector<double> _base;
};

class RssSearch
{
public:
    RssSearch();
    ~RssSearch();

    std::string doQuery(const std::string & query);

    void showPageLib() const;
    void showOffsetLib() const;
    void showInvertIndexLib() const;
private:
    void loadLibrary();
    std::string processWords(const std::string & word);
    void getQueryWordsWeightVector(const std::vector<std::string> & queryWords, std::vector<double> & weightVec);
    bool executeQuery(const std::vector<std::string> & queryWords, std::vector<std::pair<std::size_t, std::vector<double> > > & resultVec);
    std::string makeJson(const std::vector<std::size_t> & docIdVec, const std::vector<std::string> & queryWords);
    std::string noAnswer();
private:
    std::unordered_map<std::size_t, WebPage> _pageLib;
    std::unordered_map<std::size_t, std::pair<std::size_t, std::size_t> > _offsetLib;
    std::unordered_map<std::string, std::set<std::pair<std::size_t, double> > > _invertIndexTable;
};

} // end of namespace md

#endif
