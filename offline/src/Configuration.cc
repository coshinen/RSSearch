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

Configuration * Configuration::getInstance()
{
	if (NULL == _pInstance)
		_pInstance = new Configuration();
	return _pInstance;
}

bool Configuration::init(const std::string & filePath)
{
	std::ifstream ifs(filePath.c_str());
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
	for (auto elem : _configMap)
	{
		std::cout << elem.first << " --> " << elem.second << std::endl;
	}
}

std::map<std::string, std::string> & Configuration::getConfigMap()
{ return _configMap; }

} // end of namespace my
