 ///
 /// @file    Configuration.cc
 /// @author  mistydew(mistydew@qq.com)
 /// @date    2017-11-07 13:22:13
 ///

#include "Configuration.h"

#include <iostream>
#include <fstream>
#include <sstream>

namespace my
{

Configuration * Configuration::_pInstance = getInstance();
Configuration::AutoRelease Configuration::_autoRelease;

Configuration * Configuration::getInstance()
{
    if (NULL == _pInstance)
        _pInstance = new Configuration();
    return _pInstance;
}

bool Configuration::init(const char * filePath)
{
    std::ifstream ifs(filePath);
    if (!ifs.good()) {
        std::cout << "ifstream open file failed!" << std::endl;
        return false;
    }

    std::string line;
    while (getline(ifs, line))
    {
        std::istringstream iss(line);
        std::string key;
        std::string value;
        iss >> key >> value >> value;
        _configMap.insert(std::make_pair(key, value));
    }

    ifs.close();
    return true;
}

void Configuration::display()
{
    for (auto & map : _configMap)
    {
        std::cout << map.first << " --> " << map.second << std::endl;
    }
}

std::map<std::string, std::string> & Configuration::getConfigMap()
{ return _configMap; }

Configuration::Configuration()
{ std::cout << "Configuration()" << std::endl; }

Configuration::~Configuration()
{ std::cout << "~Configuration()" << std::endl; }

Configuration::AutoRelease::AutoRelease()
{ std::cout << "Configuration::AutoRelease()" << std::endl; }

Configuration::AutoRelease::~AutoRelease()
{
    std::cout << "Configuration::~AutoRelease()" << std::endl;
    if (_pInstance)
        delete _pInstance;
}

} // end of namespace my
