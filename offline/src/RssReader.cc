 ///
 /// @file    RssReader.cc
 /// @author  mistydew(mistydew@qq.com)
 /// @date    2017-11-12 10:23:29
 ///

#include "RssReader.h"
#include "Configuration.h"

#include "tinyxml2.h"
#include "../deps/boost/regex.hpp"
#include "../deps/simhash/Simhasher.hpp"

#include <sys/types.h>
#include <dirent.h>

#include <iostream>
#include <fstream>
#include <utility>

namespace my
{

void RssReader::init(const char * path)
{
	DIR * pDir = ::opendir(path);
	struct dirent * pd;
	char fileName[256];
	while ((pd = ::readdir(pDir)) != NULL)
	{
		if (::strcmp(pd->d_name, ".") && ::strcmp(pd->d_name, "..")) {
			::sprintf(fileName, "%s%s", path, pd->d_name);
			_docs.push_back(fileName);
		}
	}
}

void RssReader::parseRss()
{
	for (auto & doc : _docs)
	{
		std::cout << doc << std::endl;
		parseRss(doc.c_str());
	}
	
	deduplication();
}

void RssReader::dump(const char * fileName)
{
	std::ofstream ofs(fileName);
	if (!ofs.good()) {
		std::cout << "ofstream open file failed!" << std::endl;
		return;
	}

	simhash::Simhasher simhasher(Configuration::getInstance()->getConfigMap()[DICT_PATH].c_str(),
			Configuration::getInstance()->getConfigMap()[HMM_PATH].c_str(),
			Configuration::getInstance()->getConfigMap()[IDF_PATH].c_str(),
			Configuration::getInstance()->getConfigMap()[STOP_WORD_PATH].c_str());
	for (std::size_t idx = 0; idx != _newArticles.size(); ++idx)
	{
		std::size_t offset = ofs.tellp();
		std::string text = std::string("<doc>\n")
			+ "<docid>" + int2str(idx + 1) + "</docid>\n"
			+ "<url>" + _newArticles[idx]._link + "</url>\n"
			+ "<title>" + _newArticles[idx]._title + "</title>\n"
			+ "<content>" + _newArticles[idx]._content + "</content>\n"
			+ "</doc>\n";
		std::size_t length = text.size();
		/* ripepage library */
		ofs << text;
		/* offset library */
		_offset.push_back(std::make_pair(offset, length));

		/* invert index library */
		std::vector<std::pair<std::string, double> > res;
		simhasher.extract(_newArticles[idx]._content, res, 65536);
		for (std::size_t i = 0; i != res.size(); ++i)
		{
			_invertIndexTable[res[i].first].insert(std::make_pair(idx + 1, res[i].second));
		}
	}

	ofs.close();

	dumpOffset(Configuration::getInstance()->getConfigMap()[OFFSET_LIB_PATH].c_str());
	dumpInvert(Configuration::getInstance()->getConfigMap()[INVERT_LIB_PATH].c_str());
}

void RssReader::parseRss(const char * fileName)
{
	tinyxml2::XMLDocument doc;
	doc.LoadFile(fileName);
	if (doc.ErrorID()) {
		std::cout << "tinyxml2::XMLDocument load file failed!" << std::endl;
		return;
	}

	tinyxml2::XMLElement * rootElement = doc.FirstChildElement("rss");
	if (NULL == rootElement)
		return;
	tinyxml2::XMLElement * channelElement = rootElement->FirstChildElement("channel");
	if (NULL == channelElement)
		return;
	tinyxml2::XMLElement * itemElement = channelElement->FirstChildElement("item");
	if (NULL == itemElement)
		return;
	
	RssItem rssItem;

	simhash::Simhasher simhasher(Configuration::getInstance()->getConfigMap()[DICT_PATH].c_str(),
			Configuration::getInstance()->getConfigMap()[HMM_PATH].c_str(),
			Configuration::getInstance()->getConfigMap()[IDF_PATH].c_str(),
			Configuration::getInstance()->getConfigMap()[STOP_WORD_PATH].c_str());

	do
	{
		tinyxml2::XMLElement * titleElement = itemElement->FirstChildElement("title");
		tinyxml2::XMLElement * linkElement = itemElement->FirstChildElement("link");
		tinyxml2::XMLElement * descriptionElement = itemElement->FirstChildElement("description");
		tinyxml2::XMLElement * contentElement = itemElement->FirstChildElement("content:encoded");

		std::string title;
		if (titleElement)
			title = std::string(titleElement->GetText());
		else
			continue;
		std::string link;
		if (linkElement)
			link = std::string(linkElement->GetText());
		else
			continue;
		std::string description;
		if (descriptionElement)
			description = std::string(descriptionElement->GetText());
		std::string content;
		if (contentElement)
			content = std::string(contentElement->GetText());
		if (description != std::string("") && std::string("") == content)
			content = description;
		else if (std::string("") != description && std::string("") == content)
			continue;
		
		boost::regex re("<.*?>");
		std::string processContent = boost::regex_replace(content, re, "");

		size_t topN = 7;
		uint64_t u64 = 0;
		simhasher.make(processContent, topN, u64);
		std::cout << "simhash = " << u64 << std::endl;

		rssItem._title = title;
		rssItem._link = link;
		rssItem._content = processContent;
		rssItem._simhash = u64;
		_articles.push_back(std::move(rssItem));
	} while ((itemElement = itemElement->NextSiblingElement()));
}

void RssReader::deduplication()
{
	_newArticles.push_back(_articles[0]);
	for (std::size_t idx = 1; idx != _articles.size(); ++idx)
	{
		std::size_t pos;
		std::size_t size = _newArticles.size();
		for (pos = 0; pos != size; ++pos)
		{
			if (1 == simhash::Simhasher::isEqual(_newArticles[pos]._simhash, _articles[idx]._simhash))
				break;
		}
		if (size == pos)
			_newArticles.push_back(_articles[idx]);
	}
}

void RssReader::dumpOffset(const char * fileName)
{
	std::ofstream ofs(fileName);
	if (!ofs.good()) {
		std::cout << "ofstream open file failed!" << std::endl;
		return;
	}

	for (std::size_t idx = 0; idx != _offset.size(); ++idx)
	{
		ofs << idx + 1 << " " << _offset[idx].first << " " << _offset[idx].second << std::endl;
	}

	ofs.close();
}

void RssReader::dumpInvert(const char * fileName)
{
	std::ofstream ofs(fileName);
	if (!ofs.good()) {
		std::cout << "ofstream open file failed!" << std::endl;
		return;
	}

	for (auto invertIndex : _invertIndexTable)
	{
		ofs << invertIndex.first << " ";
		for (auto temp : invertIndex.second)
		{
			ofs << temp.first << " " << temp.second << " ";
		}
		ofs << std::endl;
	}

	ofs.close();
}

} // end of namespace my
