#pragma once

#include "../Engine.h"
#include "rapidxml.hpp"
#include "rapidxml_print.hpp"

using namespace rapidxml;

struct tLayer {
	std::string		strLayerName;
	std::vector<unsigned int> vecLayerData;

	tLayer(const std::string& _strLayerName) {
		strLayerName = _strLayerName;
	}
};

class CXmlParser
{
private :
	Vector2			m_vecResolution;
	std::vector<tLayer>	m_vecLayers;

public :
	CXmlParser();
	~CXmlParser();

public :
	void init(const std::string& _strFile);

public :
	std::vector<unsigned int>	GetTileDataByLayerName(const std::string& _strLayerName);
	Vector2	GetResolution() { return m_vecResolution; }

};

