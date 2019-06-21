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
    if (!md::Configuration::getInstance()->init(md::CONF_PATH)) {
        std::cout << "load configuration file failed!" << std::endl;
        return -1;
    }
    md::Configuration::getInstance()->display();

    /* initialize cppjieba */
    std::cout << "initialize cppjieba..." << std::endl;
    md::WordSegmentation::getInstance();
    std::cout << "initializition completed " << std::endl;

    /* start the server */
    md::RssSearchServer server;
    server.start();

    return 0;
}
