#include "CXmlParser.h"

#include <fstream>
#include "../PathManager.h"


CXmlParser::CXmlParser()
	: m_vecResolution(0, 0)
{
}

CXmlParser::~CXmlParser()
{

}

void CXmlParser::init(const std::string& _strFile)
{
	const PathInfo* Path = CPathManager::GetInst()->FindPath("Map");
	std::wstring wPath = Path->Path;
	
	std::string strFilePath(wPath.begin(), wPath.end());
	strFilePath.append(_strFile);

	std::basic_ifstream<char> xmlFile(strFilePath);
	xmlFile.seekg(0, std::ios::end);
	unsigned int size = xmlFile.tellg();
	xmlFile.seekg(0);

	std::vector<char> xmlData(size + 1);
	xmlData[size] = 0;
	xmlFile.read(&xmlData.front(), (std::streamsize)size);

	xml_document<char> xmlDoc;
	xmlDoc.parse<0>(&xmlData.front());

	xml_node<char>* pRootNode = xmlDoc.first_node();
	xml_attribute<char>* attr;

	for (xml_node<char>* item = pRootNode->first_node(); item; item = item->next_sibling()) {
		if (strcmp("layer", item->name()) == 0) {
			for (attr = item->first_attribute(); attr; attr = attr->next_attribute()) {
				if (strcmp("name", attr->name()) == 0) {
					//tLayer* tempLayer = new tLayer(attr->value());
					//m_vecLayers.push_back(*tempLayer);

					m_vecLayers.push_back((tLayer)attr->value());
				}
			}

			if (m_vecResolution != Vector2(0, 0))
				continue;

			for (attr = item->first_attribute(); attr; attr = attr->next_attribute()) {
				if (strcmp("width", attr->name()) == 0) {
					m_vecResolution.x = (float)atoi(attr->value());
				}

				if (strcmp("height", attr->name()) == 0) {
					m_vecResolution.y = (float)atoi(attr->value());
				}
			}
		}
	}

	for (xml_node<char>* item = pRootNode->first_node(); item; item = item->next_sibling()) {

		if (strcmp("layer", item->name()) == 0) {
			for (attr = item->first_attribute(); attr; attr = attr->next_attribute()) {
				if (strcmp("name", attr->name()) == 0) {
					for (auto iter = m_vecLayers.begin(); iter != m_vecLayers.end(); iter++) {
						if (strcmp(iter->strLayerName.c_str(), attr->value()) == 0) {
							//std::vector<unsigned int> vecMapTile;

							std::string strArray(item->last_node()->value());
							int iResoltionMaxCount = (int)(m_vecResolution.x * m_vecResolution.y);

							for (int j = 0; j < iResoltionMaxCount; j++) {
								int newStopNum = (int)strArray.find(",");

								unsigned int num = stoul(strArray.substr(0, newStopNum));

								strArray.erase(0, newStopNum + 1);

								if (num != 0)
									num -= 1;

								//vecMapTile.push_back(num);
								iter->vecLayerData.push_back(num);
							}


							//iter->vecLayerData = vecMapTile;


							//for (int i = 0; i < vecMapTile.size(); i++)
							//{
							//	unsigned int temp = vecMapTile[i];

							//	delete &temp;
							//}



							break;
						}
					}
				}
			}
		}
	}

	xmlFile.close();
}

std::vector<unsigned int> CXmlParser::GetTileDataByLayerName(const std::string& _strLayerName)
{
	std::vector<unsigned int> vecData;

	for (tLayer tempLayer : m_vecLayers) {
		if (strcmp(tempLayer.strLayerName.c_str(), _strLayerName.c_str()) == 0) {
			vecData = tempLayer.vecLayerData;

			break;
		}
	}

	return vecData;
}
