 ///
 /// @file    WebPage.cc
 /// @author  mistydew(mistydew@qq.com)
 /// @date    2017-12-02 15:36:06
 ///

#include "WebPage.h"
#include "Configuration.h"
#include "WordSegmentation.h"

#include <queue>
#include <sstream>

namespace my
{

WebPage::WebPage(const std::string & doc)
{
    std::cout << "WebPage(const std::string &)" << std::endl;
    _topWords.reserve(TOPK);
    processDoc(doc);
}

WebPage::~WebPage()
{ std::cout << "~WebPage()" << std::endl; }

std::string WebPage::getDocTitle()
{ return _docTitle; }

std::string WebPage::getDocUrl()
{ return _docUrl; }

std::string WebPage::summary(const std::vector<std::string> & queryWords)
{
    std::vector<std::string> summaryVec;
    std::istringstream iss(_docContent);
    std::string line;
    while (iss >> line)
    {
        for (auto & word : queryWords)
        {
            if (line.find(word) != std::string::npos) {
                summaryVec.push_back(line);
                break;
            }
        }

        if (summaryVec.size() > 6)
            break;
    }

    std::string summary;
    for (auto & line : summaryVec)
    {
        summary.append(line).append("\n");
    }
    return summary;
}

void WebPage::processDoc(const std::string & doc)
{
    std::cout << "process document..." << std::endl;
    std::string begId = "<docid>";
    std::string endId = "</docid>";
    std::string begUrl = "<url>";
    std::string endUrl = "</url>";
    std::string begTitle = "<title>";
    std::string endTitle = "</title>";
    std::string begContent = "<content>";
    std::string endContent = "</content>";

    std::string::size_type bpos = doc.find(begId);
    std::string::size_type epos = doc.find(endId);
    std::string docId = doc.substr(bpos + begId.size(), epos - bpos - begId.size());
    _docId = str2uint(docId);

    bpos = doc.find(begUrl);
    epos = doc.find(endUrl);
    _docUrl = doc.substr(bpos + begUrl.size(), epos - bpos - begUrl.size());

    bpos = doc.find(begTitle);
    epos = doc.find(endTitle);
    _docTitle = doc.substr(bpos + begTitle.size(), epos - bpos - begTitle.size());

    bpos = doc.find(begContent);
    epos = doc.find(endContent);
    _docContent = doc.substr(bpos + begContent.size(), epos - bpos - begContent.size());

#if 0
    std::cout << _docId << std::endl;
    std::cout << _docUrl << std::endl;
    std::cout << _docTitle << std::endl;
    std::cout << _docContent << std::endl;
#endif

    statistic();
    calcTopK();
    std::cout << "process completed" << std::endl;
}

void WebPage::statistic()
{
    std::vector<std::string> words;
    WordSegmentation::getInstance()->cut(_docContent, words);
    std::size_t nBytes;
    for (auto & word : words)
    {
        if(0 == Configuration::getInstance()->getStopWord().count(word)) {
            nBytes = nBytesUTF8Code(word[0]);
            if (0 == nBytes) {
                word = processWords(word);
                if ("" == word)
                    continue;
            }

            ++_wordsMap[word];
        }
    }

#if 0
    for (auto & map : _wordsMap)
    {
        std::cout << map.first << " " << map.second << std::endl;
    }
#endif
}

std::string WebPage::processWords(const std::string & word)
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

void WebPage::calcTopK()
{
    std::priority_queue<std::pair<std::string, std::size_t>, std::vector<std::pair<std::string, std::size_t> >, WordFreqCompare>
        wordFreqQue(_wordsMap.begin(), _wordsMap.end());
    while (!wordFreqQue.empty())
    {
        _topWords.push_back(wordFreqQue.top().first);
        if (_topWords.size() >= TOPK)
            break;
        wordFreqQue.pop();
    }

#if 0
    std::cout << "top words:" << std::endl;
    for (auto & top : _topWords)
    {
        std::cout << top << std::endl;
    }
#endif
}

std::ostream & operator<<(std::ostream & os, const WebPage & rhs)
{
    os << rhs._docId << std::endl
        << rhs._docTitle << std::endl
        << rhs._docUrl << std::endl
        << rhs._docContent;
    return os;
}

} // end of namespace my
