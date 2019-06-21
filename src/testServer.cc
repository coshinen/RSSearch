 ///
 /// @file    testServer.cc
 /// @author  mistydew(mistydew@qq.com)
 /// @date    2017-11-04 17:25:12
 ///

#include "Configuration.h"
#include "WordSegmentation.h"
#include "RssSearchServer.h"

#include <iostream>

int main(void)
{
    /* load Configuration */
    std::cout << "load configuration file..." << std::endl;
    if (!my::Configuration::getInstance()->init(my::CONF_PATH)) {
        std::cout << "load configuration file failed!" << std::endl;
        return -1;
    }
    my::Configuration::getInstance()->display();

    /* initialize cppjieba */
    std::cout << "initialize cppjieba..." << std::endl;
    my::WordSegmentation::getInstance();
    std::cout << "initializition completed " << std::endl;

    /* start the server */
    my::RssSearchServer server;
    server.start();

    return 0;
}
