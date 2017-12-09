 ///
 /// @file    Configuration.cc
 /// @author  mistydew(mistydew@qq.com)
 /// @date    2017-11-07 13:22:13
 ///

#include "Configuration.h"

#include "json/json.h"

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
#if 0
	std::size_t begin = ifs.tellg();
	ifs.seekg(0, std::ios_base::end);
	std::size_t end = ifs.tellg();
	ifs.seekg(0, std::ios_base::beg);
	std::size_t length = end - begin;
	char * temp = new char[length + 1]();
	ifs.read(temp, length);
	std::string conf(temp);
	delete [] temp;
#endif
	std::ostringstream oss;
	oss << ifs.rdbuf();
	std::string conf = oss.str();

	ifs.close();

	Json::Reader reader;
	Json::Value value;

	if (reader.parse(conf, value)) {
		if (!value[THREAD_NUM].isNull())
			_configMap.insert(std::make_pair(std::string(THREAD_NUM), uint2str(value[THREAD_NUM].asUInt())));
		if (!value[TASK_NUM].isNull())
			_configMap.insert(std::make_pair(std::string(TASK_NUM), uint2str(value[TASK_NUM].asUInt())));
		if (!value[IP].isNull())
			_configMap.insert(std::make_pair(std::string(IP), value[IP].asString()));
		if (!value[PORT].isNull())
			_configMap.insert(std::make_pair(std::string(PORT), uint2str(value[PORT].asUInt())));
		if (!value[CACHE_PATH].isNull())
			_configMap.insert(std::make_pair(std::string(CACHE_PATH), value[CACHE_PATH].asString()));
		if (!value[PAGE_LIB_PATH].isNull())
			_configMap.insert(std::make_pair(std::string(PAGE_LIB_PATH), value[PAGE_LIB_PATH].asString()));
		if (!value[OFFSET_LIB_PATH].isNull())
			_configMap.insert(std::make_pair(std::string(OFFSET_LIB_PATH), value[OFFSET_LIB_PATH].asString()));
		if (!value[INVERT_LIB_PATH].isNull())
			_configMap.insert(std::make_pair(std::string(INVERT_LIB_PATH), value[INVERT_LIB_PATH].asString()));
		if (!value[DICT_PATH].isNull())
			_configMap.insert(std::make_pair(std::string(DICT_PATH), value[DICT_PATH].asString()));
		if (!value[HMM_PATH].isNull())
			_configMap.insert(std::make_pair(std::string(HMM_PATH), value[HMM_PATH].asString()));
		if (!value[USER_DICT_PATH].isNull())
			_configMap.insert(std::make_pair(std::string(USER_DICT_PATH), value[USER_DICT_PATH].asString()));
		if (!value[IDF_PATH].isNull())
			_configMap.insert(std::make_pair(std::string(IDF_PATH), value[IDF_PATH].asString()));
		if (!value[STOP_WORD_PATH].isNull())
			_configMap.insert(std::make_pair(std::string(STOP_WORD_PATH), value[STOP_WORD_PATH].asString()));
	}

	initStopWord();

	return true;
}

void Configuration::display() const
{
	for (auto & map : _configMap)
	{
		std::cout << map.first << " --> " << map.second << std::endl;
	}
#if 0
	for (auto & word : _stopWord)
	{
		std::cout<< word << std::endl;
	}
#endif
}

std::map<std::string, std::string> & Configuration::getConfigMap()
{ return _configMap; }

std::set<std::string> & Configuration::getStopWord()
{ return _stopWord; }

void Configuration::initStopWord()
{
	std::ifstream ifs(_configMap[STOP_WORD_PATH].c_str());
	if (!ifs.good()) {
		std::cout << "ifstream open file failed!" << std::endl;
		return;
	}

	std::string word;
	while (getline(ifs, word))
	{
		_stopWord.insert(word);
	}

	ifs.close();
}

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
