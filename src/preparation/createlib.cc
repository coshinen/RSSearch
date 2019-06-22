 ///
 /// @file    createlib.cc
 /// @author  mistydew(mistydew@qq.com)
 /// @date    2017-11-12 09:57:31
 ///

#include "configuration.h"
#include "rssreader.h"

int main(void)
{
    /* load configuration */
    md::Configuration::getInstance()->init(md::CONF_PATH);
    md::Configuration::getInstance()->display();

    /* parse xml */
    md::RssReader rssReader;
    rssReader.init(md::Configuration::getInstance()->getConfigMap()[md::RES_PATH].c_str());
    rssReader.parseRss();
    rssReader.dump(md::Configuration::getInstance()->getConfigMap()[md::PAGE_LIB_PATH].c_str());

    return 0;
}
