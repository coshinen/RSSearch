 ///
 /// @file    RssReader.cc
 /// @author  mistydew(mistydew@qq.com)
 /// @date    2017-11-12 10:23:29
 ///

#include "RssReader.h"
#include "tinyxml2.h"
#include "Configuration.h"

#include <boost/regex.hpp>

#include <sys/types.h>
#include <dirent.h>

#include <iostream>
#include <fstream>

namespace my
{

void RssReader::init(const std::string & path)
{
	DIR * pDir = ::opendir(path.c_str());
	struct dirent * pd;
	char fileName[256];
	while ((pd = ::readdir(pDir)) != NULL)
	{
		if (::strcmp(pd->d_name, ".") && ::strcmp(pd->d_name, "..")) {
			::sprintf(fileName, "%s%s", path.c_str(), pd->d_name);
			_docs.push_back(fileName);
		}
	}
}

void RssReader::parseRss()
{
	for (auto elem : _docs)
	{
		std::cout << elem << std::endl;
		parseRss(elem);
	}
}

void RssReader::dump(const std::string & fileName)
{
	std::ofstream ofs(fileName);
	if (!ofs.good()) {
		std::cout << "ofstream open file failed!" << std::endl;
		return;
	}

	for (std::size_t idx = 0; idx != _articles.size(); ++idx)
	{
		std::size_t offset = ofs.tellp();
		std::string text = std::string("<doc>\n")
			+ "<docid>" + int2str(idx + 1) + "</docid>\n"
			+ "<url>" + _articles[idx].link + "</url>\n"
			+ "<title>" + _articles[idx].title + "</title>\n"
			+ "<content>" + _articles[idx].content + "</content>\n"
			+ "</doc>\n";
		std::size_t length = text.size();
		ofs << text;
		_offset.push_back(std::make_pair(offset, length));
	}

	ofs.close();

	dumpOffset(Configuration::getInstance()->getConfigMap()[OFFSET_LIB_PATH]);
}

void RssReader::parseRss(const std::string & fileName)
{
	tinyxml2::XMLDocument doc;
	doc.LoadFile(fileName.c_str());
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

		rssItem.title = title;
		rssItem.link = link;
		rssItem.content = processContent;
		_articles.push_back(rssItem);
	} while (itemElement = itemElement->NextSiblingElement());
}

void RssReader::dumpOffset(const std::string & fileName)
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

} // end of namespace my
