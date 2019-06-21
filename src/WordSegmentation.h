 ///
 /// @file    WordSegmentation.h
 /// @author  mistydew(mistydew@qq.com)
 /// @date    2017-12-06 19:18:51
 ///

#ifndef __WORDSEGMENTATION_H__
#define __WORDSEGMENTATION_H__

#include "cppjieba/Jieba.hpp"

#include <string>
#include <vector>

namespace my
{

class WordSegmentation
{
public:
    static WordSegmentation * getInstance();

    void cut(const std::string & str, std::vector<std::string> & words);
private:
    WordSegmentation();
    ~WordSegmentation();

    class AutoRelease
    {
    public:
        AutoRelease();
        ~AutoRelease();
    };
private:
    static WordSegmentation * _pInstance;
    static AutoRelease _autoRelease;

    cppjieba::Jieba _jieba;
};

} // end of namespace my

#endif
