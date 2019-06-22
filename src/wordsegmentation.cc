 ///
 /// @file    wordsegmentation.cc
 /// @author  mistydew(mistydew@qq.com)
 /// @date    2017-12-06 19:22:11
 ///

#include "wordsegmentation.h"
#include "configuration.h"

namespace md
{

WordSegmentation * WordSegmentation::_pInstance = NULL;
WordSegmentation::AutoRelease WordSegmentation::_autoRelease;

WordSegmentation * WordSegmentation::getInstance()
{
    if (NULL == _pInstance)
        _pInstance = new WordSegmentation();
    return _pInstance;
}

void WordSegmentation::cut(const std::string & str, std::vector<std::string> & words)
{
    _jieba.Cut(str, words, true);
}

WordSegmentation::WordSegmentation()
: _jieba(Configuration::getInstance()->getConfigMap()[DICT_PATH].c_str(),
        Configuration::getInstance()->getConfigMap()[HMM_PATH].c_str(),
        Configuration::getInstance()->getConfigMap()[USER_DICT_PATH].c_str())
{ std::cout << "WordSegmentation()" << std::endl; }

WordSegmentation::~WordSegmentation()
{ std::cout << "~WordSegmentation()" << std::endl; }

WordSegmentation::AutoRelease::AutoRelease()
{ std::cout << "WordSegmentation::AutoRelease()" << std::endl; }

WordSegmentation::AutoRelease::~AutoRelease()
{
    std::cout << "WordSegmentation::~AutoRelease()" << std::endl;
    if (_pInstance)
        delete _pInstance;
}

} // end of namespace md
