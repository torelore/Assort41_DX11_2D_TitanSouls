#include "SlimeSceneInfo.h"

#include "Scene/Scene.h"
#include "RapidXml/CXmlParser.h"
#include "Component/TileMapComponent.h"

#include "../GameObject/Player.h"
#include "../GameObject/Sludgeheart.h"
#include "../GameObject/Portal.h"

#include "../UI/GameDefaultUI.h"

// debug
#include <atlstr.h>

CSlimeSceneInfo::CSlimeSceneInfo()
{
	m_ClassTypeName = "SlimeSceneInfo";
}

CSlimeSceneInfo::~CSlimeSceneInfo()
{
}

bool CSlimeSceneInfo::Init()
{
	CSceneInfo::Init();

	// Sound
	{
		m_Owner->GetResource()->LoadSound("BGM", "Slime1", true, "MUSIC/BOSS/AcidNerveA.ogg");
		m_Owner->GetResource()->LoadSound("BGM", "Slime2", true, "MUSIC/BOSS/AcidNerveB.ogg");
		m_Owner->GetResource()->LoadSound("BGM", "Slime3", true, "MUSIC/BOSS/AcidNerveC.ogg");
		m_Owner->GetResource()->LoadSound("BGM", "Slime4", true, "MUSIC/BOSS/AcidNerveD.ogg");
		m_Owner->GetResource()->LoadSound("BGM", "Slime5", true, "MUSIC/BOSS/AcidNerveE.ogg");
		m_Owner->GetResource()->LoadSound("BGM", "Slime_Sfx", true, "SFX/AMBIENCE/SewerWaterProximity.ogg");


		m_Owner->GetResource()->LoadSound("Effect", "arrow_charge", false, "PLAYER/arrow_charge.ogg");
		m_Owner->GetResource()->LoadSound("Effect", "arrow_retrieval", true, "PLAYER/arrow_retrieval.ogg");
		m_Owner->GetResource()->LoadSound("Effect", "arrow_shoot", false, "PLAYER/arrow_shoot.ogg");
		m_Owner->GetResource()->LoadSound("Effect", "ArrowImpact", false, "PLAYER/ArrowImpact1.ogg");
		m_Owner->GetResource()->LoadSound("Effect", "Pickuparrow", false, "PLAYER/Pickuparrow.ogg");

		m_Owner->GetResource()->LoadSound("Effect", "Roll", false, "PLAYER/Roll.ogg");

		m_Owner->GetResource()->LoadSound("Effect", "SavePoint", false, "PLAYER/SAVEPOINT/SavePoint.ogg");

		m_Owner->GetResource()->LoadSound("Effect", "WaterStep", true, "PLAYER/Water/Step1.ogg");

		m_Owner->GetResource()->LoadSound("Effect", "VineStep", true, "PLAYER/Vine/Step1.ogg");

		m_Owner->GetResource()->LoadSound("Effect", "StoneStep", true, "PLAYER/Stone/Step1.ogg");

		m_Owner->GetResource()->LoadSound("Effect", "EnterDoor", false, "PLAYER/EnterDoor.ogg");
		m_Owner->GetResource()->FindSound("EnterDoor")->Stop();

	}



	// UI
	CGameDefaultUI* SceneUI = m_Owner->GetViewport()->CreateUIWindow<CGameDefaultUI>("GameDefaultUI");
	SceneUI->SetNowScene(ESceneList::SludgeHeart);

	m_Owner->SetName("SlimeScene");



	// TileMap
	CXmlParser* xmlParser = new CXmlParser;

	xmlParser->init("sludgeheart.tmx");

	int TileMapColX = (int)xmlParser->GetResolution().x;
	int TileMapColY = (int)xmlParser->GetResolution().y;
	int TileMapTotalSize = TileMapColX * TileMapColY;

	float TileSize = 35.f;

	// TileMap Setting
	{
		std::vector<const char*> vecLayer_ground = { "1_BG1", "1_BG2", "1_FG1" };
		std::vector<const char*> vecLayer_col = { "1_COL", "1_COLA", "1_MAT" };

		// 배경 타일맵
		for (int i = 0; i < vecLayer_ground.size(); i++)
		{
			char ObjectName[40] = "SludgeHeart_TilemapObjLayer_";
			strcat_s(ObjectName, vecLayer_ground[i]);
			char ComponentName[40] = "SludgeHeart_TilemapCompLayer_";
			strcat_s(ComponentName, vecLayer_ground[i]);


			CGameObject* Background = m_Owner->CreateObject<CGameObject>(ObjectName);
			CTileMapComponent* TilemapBackground = Background->CreateComponent<CTileMapComponent>(ComponentName);
			TilemapBackground->CreateTile(ETileShape::Rect, TileMapColX, TileMapColY, Vector2(TileSize, TileSize));
			TilemapBackground->SetTileMaterial("TileMap_Underworld");
			TilemapBackground->SetTileTexture("underworld", TEXT("IMG/underworld.png"));

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

			if (strcmp("1_FG1", vecLayer_ground[i]) == 0)
				TilemapBackground->SetWorldPosition(0.f, 0.f, 50.f);
			else
				TilemapBackground->SetWorldPosition(0.f, 0.f, 0.f);
		}


		// 충돌, 마테리얼 타일맵
		for (int i = 0; i < vecLayer_col.size(); i++)
		{
			char ObjectName[40] = "SludgeHeart_TilemapObjLayer_";
			strcat_s(ObjectName, vecLayer_col[i]);
			char ComponentName[40] = "SludgeHeart_TilemapCompLayer_";
			strcat_s(ComponentName, vecLayer_col[i]);


			CGameObject* Background = m_Owner->CreateObject<CGameObject>(ObjectName);
			CTileMapComponent* TilemapBackground = Background->CreateComponent<CTileMapComponent>(ComponentName);
			TilemapBackground->CreateTile(ETileShape::Rect, TileMapColX, TileMapColY, Vector2(TileSize, TileSize));
			TilemapBackground->SetTileMaterial("TileMap_Spectiles");
			TilemapBackground->SetTileTexture("spectiles", TEXT("IMG/spectiles.png"));
			TilemapBackground->CreateTextureFrameData(16, 16, 16.f, 16.f);

			std::vector<unsigned int> vecData = xmlParser->GetTileDataByLayerName(vecLayer_col[i]);
			std::vector<unsigned int> vecData2;

			for (int i = 0; i < TileMapColY; i++) {
				int temp = TileMapColY - 1 - i;


				for (int j = 0; j < TileMapColX; j++) {
					int temp2 = temp * TileMapColX + j;
					temp2 = vecData[temp2];

					if (temp2 != 0) {
						temp2 -= 4096;
					}

					vecData2.push_back(temp2);
				}
			}


			TilemapBackground->SetTileArrayData(vecData2);
			TilemapBackground->SetWorldPosition(0.f, 0.f, 100.f);

			Background->SetEnable(false);
		}


		// 벽, 물 등을 체크하기 위한 투명 타일맵
		{
			CGameObject* RootBackground = m_Owner->CreateObject<CGameObject>("TilemapObjLayer_Root");
			CTileMapComponent* RootTilemapBackground = RootBackground->CreateComponent<CTileMapComponent>("TilemapCompLayer_Root");
			RootTilemapBackground->CreateTile(ETileShape::Rect, TileMapColX, TileMapColY, Vector2(TileSize, TileSize));
			RootTilemapBackground->SetTileMaterial("TileMap_Overworld2");
			RootTilemapBackground->SetTileTexture("overworld2", TEXT("IMG/overworld2.png"));

			std::vector<unsigned int> vecCol1Data = xmlParser->GetTileDataByLayerName("1_COL");
			std::vector<unsigned int> vecCol1Data2;

			for (int i = 0; i < TileMapColY; i++) {
				int temp = TileMapColY - 1 - i;


				for (int j = 0; j < TileMapColX; j++) {
					int temp2 = temp * TileMapColX + j;
					temp2 = vecCol1Data[temp2];

					if (temp2 != 0) {
						temp2 -= 4096;
					}

					vecCol1Data2.push_back(temp2);
				}
			}


			std::vector<unsigned int> vecData;

			for (int i = 0; i < TileMapTotalSize; i++) {
				vecData.push_back(0);
			}

			RootTilemapBackground->SetTileArrayData(vecData);
			RootTilemapBackground->SetWorldPosition(0.f, 0.f);

			for (int i = 0; i < TileMapTotalSize; i++) {
				int temp = vecCol1Data2[i];

				if (temp != 0) {
					RootTilemapBackground->ChangeTileOptionByIdx(i, ETileOption::Wall);
					continue;
				}
			}
		}


		// 전경을 체크하기 위한 투명 타일맵
		{
			CGameObject* RootBackground = m_Owner->CreateObject<CGameObject>("TilemapObjLayer_Root_FG");
			CTileMapComponent* RootTilemapBackground = RootBackground->CreateComponent<CTileMapComponent>("TilemapCompLayer_Root_FG");
			RootTilemapBackground->CreateTile(ETileShape::Rect, TileMapColX, TileMapColY, Vector2(TileSize, TileSize));
			RootTilemapBackground->SetTileMaterial("TileMap_Overworld2");
			RootTilemapBackground->SetTileTexture("overworld2", TEXT("IMG/overworld2.png"));

			std::vector<unsigned int> vecFG1Data = xmlParser->GetTileDataByLayerName("1_FG1");
			std::vector<unsigned int> vecFG1Data2;

			for (int i = 0; i < TileMapColY; i++) {
				int temp = TileMapColY - 1 - i;


				for (int j = 0; j < TileMapColX; j++) {
					int temp2 = temp * TileMapColX + j;
					temp2 = vecFG1Data[temp2];

					vecFG1Data2.push_back(temp2);
				}
			}

			std::vector<unsigned int> vecData;

			for (int i = 0; i < TileMapTotalSize; i++) {
				vecData.push_back(0);
			}

			RootTilemapBackground->SetTileArrayData(vecData);
			RootTilemapBackground->SetWorldPosition(0.f, 0.f);

			for (int i = 0; i < TileMapTotalSize; i++) {
				int temp = vecFG1Data2[i];

				if (temp != 0) {
					RootTilemapBackground->ChangeTileOptionByIdx(i, ETileOption::FG);
					continue;
				}
			}
		}



		SAFE_DELETE(xmlParser)
	}


	// Object Setting
	float CenterX = TileMapColX * TileSize / 2.f + TileSize * 1.5f;

	CPlayer* Player = m_Owner->CreateObject<CPlayer>("Player");
	//Player->SetWorldPosition(CenterX, 280.f, 10.f);
	Player->SetWorldPosition(CenterX, TileSize * 24.f, 10.f);

	SetPlayerObject(Player);


	//CSludgeheart* SludgeHeart = m_Owner->CreateObject<CSludgeheart>("Sludgeheart");
	//SludgeHeart->SetWorldPosition(CenterX, 1350.f, 20.f);
	//SludgeHeart->SetWorldPosition(CenterX, 1000.f);

	const Vector2& PortalSize = Vector2(TileSize * 1.5f, TileSize);

	CPortal* Portal = m_Owner->CreateObject<CPortal>("SlimeToRuinsPortal");
	Portal->SetWorldPosition(CenterX, TileSize * 22.f, 150.f);
	Portal->SetPortalSize(PortalSize);
	Portal->SetPortalDest(ESceneList::StartingRuins, EDirection::Down);
	Portal->SetStartScene(ESceneList::SludgeHeart);


	return true;
}

void CSlimeSceneInfo::Update(float DeltaTime)
{
	CSceneInfo::Update(DeltaTime);

	CSound* Sound = m_Owner->GetResource()->FindSound("Slime_Sfx");

	if (!Sound->GetPlay())
		Sound->Play();
}

void CSlimeSceneInfo::TitanStart()
{
	CSceneInfo::TitanStart();

	m_Owner->GetResource()->SoundAllStop();

	CSound* Sound = m_Owner->GetResource()->FindSound("Slime1");

	if (!Sound->GetPlay())
		Sound->Play();


	CPortal* Portal = (CPortal*)m_Owner->FindObject("SlimeToRuinsPortal");
	Portal->PortalClose();
}

void CSlimeSceneInfo::TitanEnd()
{
	CSceneInfo::TitanEnd();

	m_Owner->GetResource()->SoundAllStop();


	CPortal* Portal = (CPortal*)m_Owner->FindObject("SlimeToRuinsPortal");
	Portal->PortalOpen();
}
