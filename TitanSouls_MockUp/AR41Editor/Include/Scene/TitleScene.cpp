#include "TitleScene.h"

#include "Scene/Scene.h"
#include "Scene/SceneViewport.h"
#include "../UI/TitleSceneUI.h"
#include "Component/Tile.h"
#include "GameObject/GameObject.h"
#include "Component/TileMapComponent.h"
#include "RapidXml/CXmlParser.h"

// debug
#include <atlstr.h>

CTitleScene::CTitleScene()
{
	m_ClassTypeName = "TitleSceneInfo";
}

CTitleScene::~CTitleScene()
{
}

bool CTitleScene::Init()
{
	CSceneInfo::Init();



	// Title Sound
	{
		if (!m_Owner->GetResource()->LoadSound("BGM", "Title_Bgm", true, "SFX/AMBIENCE/Forest.ogg")) {
			return false;
		}

		m_Owner->GetResource()->FindSound("Title_Bgm")->Play();


		if (!m_Owner->GetResource()->LoadSound("UI", "StartGame", false, "UI/StartGame.ogg")) {
			return false;
		}

		if (!m_Owner->GetResource()->LoadSound("UI", "No", false , "UI/No.ogg")) {
			return false;
		}

		if (!m_Owner->GetResource()->LoadSound("UI", "Select", false, "UI/Select.ogg")) {
			return false;
		}

		if (!m_Owner->GetResource()->LoadSound("UI", "Move", false, "UI/Move.ogg")) {
			return false;
		}
	}



	// UI
	m_Owner->GetViewport()->CreateUIWindow<CTitleSceneUI>("TitleSceneUI");
	m_Owner->SetName("TitleScene");



	// TileMap
	//CXmlParser* xmlParser = nullptr; 
	//xmlParser = (CXmlParser*)malloc(1000 * sizeof(CXmlParser));
	CXmlParser* xmlParser = new CXmlParser;

	xmlParser->init("FLOOR2/WOODS/title.tmx");

	int TileMapColX = (int)xmlParser->GetResolution().x;
	int TileMapColY = (int)xmlParser->GetResolution().y;
	int TileMapTotalSize = TileMapColX * TileMapColY;

	float TileSize = 18.f;

	std::vector<const char*> vecLayer_ground = { "1_BG1", "1_BG2", "WALL", "1_FG1", "1_FG2" };
	std::vector<const char*> vecLayer_col = { "1_COL", "1_COLA", "1_MAT" };

	// 배경 타일맵
	for (int i = 0; i < vecLayer_ground.size(); i++)
	{
		char ObjectName[40] = "Title_TilemapObjLayer_";
		strcat_s(ObjectName, vecLayer_ground[i]);
		char ComponentName[40] = "Title_TilemapCompLayer_";
		strcat_s(ComponentName, vecLayer_ground[i]);


		CGameObject* Background = m_Owner->CreateObject<CGameObject>(ObjectName);
		CTileMapComponent* TilemapBackground = Background->CreateComponent<CTileMapComponent>(ComponentName);
		TilemapBackground->CreateTile(ETileShape::Rect, TileMapColX, TileMapColY, Vector2(TileSize, TileSize));
		TilemapBackground->SetTileMaterial("TileMap_Overworld2");
		TilemapBackground->SetTileTexture("overworld2", TEXT("IMG/overworld2.png"));

		std::vector<unsigned int> vecData = xmlParser->GetTileDataByLayerName(vecLayer_ground[i]);
		std::vector<unsigned int> vecData2;

		for (int i = 0; i < TileMapColY; i++) {
			int temp = TileMapColY - 1 - i;

			for (int j = 0; j < TileMapColX; j++) {
				int temp2 = temp * TileMapColX + j;
				temp2 = vecData[temp2];

				vecData2.push_back(temp2);
			}
		}


		TilemapBackground->SetTileArrayData(vecData2);
		TilemapBackground->SetWorldPosition(0.f, -250.f);
	}

	// 타이틀은 전경, 충돌 등을 체크하기 위한 별도 타일맵은 필요 없다.

	SAFE_DELETE(xmlParser)

	return true;
}
