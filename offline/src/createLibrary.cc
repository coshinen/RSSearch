 ///
 /// @file    createLibrary.cc
 /// @author  mistydew(mistydew@qq.com)
 /// @date    2017-11-12 09:57:31
 ///

#include "Configuration.h"
#include "RssReader.h"

#include <iostream>
using std::cout;
using std::endl;

int main(void)
{
	my::Configuration::getInstance()->init(my::CONF_PATH);
	my::Configuration::getInstance()->display();

	my::RssReader rssReader;
	rssReader.init(my::Configuration::getInstance()->getConfigMap()[my::RES_PATH]);
	rssReader.parseRss();
	rssReader.dump(my::Configuration::getInstance()->getConfigMap()[my::PAGE_LIB_PATH]);

	return 0;
}
