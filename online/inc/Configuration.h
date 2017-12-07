 ///
 /// @file    Configuration.h
 /// @author  mistydew(mistydew@qq.com)
 /// @date    2017-11-07 12:23:19
 ///

#ifndef __CONFIGURATION_H__
#define __CONFIGURATION_H__

#include "Noncopyable.h"

#include <string>
#include <sstream>
#include <map>
#include <set>

namespace my
{

const char * const CONF_PATH = "/home/mistydew/cpp/rssSearchEngine/online/conf/Config.json";
const char * const THREAD_NUM = "thread_num";
const char * const TASK_NUM = "task_num";
const char * const IP = "ip";
const char * const PORT = "port";
const char * const CACHE_PATH = "cache";
const char * const PAGE_LIB_PATH = "pagelib";
const char * const OFFSET_LIB_PATH = "offsetlib";
const char * const INVERT_LIB_PATH = "invertlib";
const char * const DICT_PATH = "jieba_dict";
const char * const HMM_PATH = "hmm_model";
const char * const USER_DICT_PATH = "user_dict";
const char * const IDF_PATH = "idf";
const char * const STOP_WORD_PATH = "stop_word";

inline std::string uint2str(std::size_t uint)
{
	std::ostringstream oss;
	oss << uint;
	return oss.str();
}

inline std::size_t str2uint(const std::string & str)
{
	std::istringstream iss(str);
	std::size_t uint;
	iss >> uint;
	return uint;
}

inline std::size_t nBytesUTF8Code(const char & ch)
{
	std::size_t nBytes = 0;
	char chTemp = 1;

	for (std::size_t idx = 0; idx != 6; ++idx)
	{
		if (ch & (chTemp << (7 - idx)))
			++nBytes;
		else 
			break;
	}

	return nBytes;
}

class Configuration
: Noncopyable
{
public:
	static Configuration * getInstance();

	bool init(const char * filePath);

	void display() const;

	std::map<std::string, std::string> & getConfigMap();
	std::set<std::string> & getStopWord();
private:
	void initStopWord();
private:
	Configuration();
	~Configuration();
	
	class AutoRelease
	{
	public:
		AutoRelease();
		~AutoRelease();
	};
private:
	static Configuration * _pInstance;
	static AutoRelease _autoRelease;

	std::map<std::string, std::string> _configMap;
	std::set<std::string> _stopWord;
};

} // end of namespace my

#endif
