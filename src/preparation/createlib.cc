 ///
 /// @file    createLibrary.cc
 /// @author  mistydew(mistydew@qq.com)
 /// @date    2017-11-12 09:57:31
 ///

#include "Configuration.h"
#include "RssReader.h"

int main(void)
{
    /* load configuration */
    my::Configuration::getInstance()->init(my::CONF_PATH);
    my::Configuration::getInstance()->display();

    /* parse xml */
    my::RssReader rssReader;
    rssReader.init(my::Configuration::getInstance()->getConfigMap()[my::RES_PATH].c_str());
    rssReader.parseRss();
    rssReader.dump(my::Configuration::getInstance()->getConfigMap()[my::PAGE_LIB_PATH].c_str());

    return 0;
}
