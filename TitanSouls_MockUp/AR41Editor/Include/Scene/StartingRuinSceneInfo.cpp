#include "StartingRuinSceneInfo.h"

#include "Engine.h"

#include "Scene/Scene.h"
#include "RapidXml/CXmlParser.h"
#include "Component/TileMapComponent.h"

#include "../GameObject/Player.h"
#include "../GameObject/Portal.h"
#include "../GameObject/Goliath.h"
#include "../GameObject/GoliathHand.h"
#include "../GameObject/GoliathDoor.h"

#include "../UI/GameDefaultUI.h"

// debug
#include <atlstr.h>

CStartingRuinSceneInfo::CStartingRuinSceneInfo()
{
	m_ClassTypeName = "StartingRuinSceneInfo";
	m_BossStart = false;
}

CStartingRuinSceneInfo::~CStartingRuinSceneInfo()
{
}

bool CStartingRuinSceneInfo::Init()
{
	CSceneInfo::Init();

	// StartingRuin Sound
	{
		m_Owner->GetResource()->LoadSound("BGM", "StartingRuin_Bgm", true, "MUSIC/WORLD/Overworld1.ogg");
		m_Owner->GetResource()->LoadSound("BGM", "Colossus", true, "MUSIC/BOSS/Colossus.ogg");
		m_Owner->GetResource()->LoadSound("Effect", "Colossus_Rise", false, "SFX/BOSS/COLOSSUS/RiseBodyLand.ogg");
		m_Owner->GetResource()->LoadSound("Effect", "Colossus_Roar", false, "SFX/BOSS/COLOSSUS/Roar.ogg");
		m_Owner->GetResource()->LoadSound("Effect", "Colossus_Hit", false, "SFX/BOSS/COLOSSUS/Hit1.ogg");

		m_Owner->GetResource()->LoadSound("BGM", "Release", false, "SFX/TITANKILL/Release.ogg");


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

		m_Owner->GetResource()->LoadSound("Effect", "DoorMove", false, "PLAYER/SwitchDoor.ogg");

	}


	// UI
	CGameDefaultUI* SceneUI = m_Owner->GetViewport()->CreateUIWindow<CGameDefaultUI>("GameDefaultUI");
	SceneUI->SetNowScene(ESceneList::StartingRuins);

	m_Owner->SetName("StartingRuinScene");



	// TileMap
	CXmlParser* xmlParser = new CXmlParser;

	xmlParser->init("floor1.tmx");

	int TileMapColX = (int)xmlParser->GetResolution().x;
	int TileMapColY = (int)xmlParser->GetResolution().y;
	int TileMapTotalSize = TileMapColX * TileMapColY;

	float TileSize = 35.f;

	// TileMap Setting
	{
		std::vector<const char*> vecLayer_ground = { "1_BG1", "1_BG2", "WALL", "1_FG1" };
		std::vector<const char*> vecLayer_col = { "1_COL", "2_COL", "1_COLA", "1_MAT" };

		// 배경 타일맵
		for (int i = 0; i < vecLayer_ground.size(); i++)
		{
			char ObjectName[40] = "StartingRuins_TilemapObjLayer_";
			strcat_s(ObjectName, vecLayer_ground[i]);
			char ComponentName[40] = "StartingRuins_TilemapCompLayer_";
			strcat_s(ComponentName, vecLayer_ground[i]);


			CGameObject* Background = m_Owner->CreateObject<CGameObject>(ObjectName);
			CTileMapComponent* TilemapBackground = Background->CreateComponent<CTileMapComponent>(ComponentName);

			TilemapBackground->CreateTile(ETileShape::Rect, TileMapColX, TileMapColY, Vector2(TileSize, TileSize));
			TilemapBackground->SetTileMaterial("TileMap_Overworld");
			TilemapBackground->SetTileTexture("overworld", TEXT("IMG/overworld.png"));

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
			char ObjectName[40] = "StartingRuins_TilemapObjLayer_";
			strcat_s(ObjectName, vecLayer_col[i]);
			char ComponentName[40] = "StartingRuins_TilemapCompLayer_";
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


			std::vector<unsigned int> vecCol2Data = xmlParser->GetTileDataByLayerName("2_COL");
			std::vector<unsigned int> vecCol2Data2;

			for (int i = 0; i < TileMapColY; i++) {
				int temp = TileMapColY - 1 - i;


				for (int j = 0; j < TileMapColX; j++) {
					int temp2 = temp * TileMapColX + j;
					temp2 = vecCol2Data[temp2];

					if (temp2 != 0) {
						temp2 -= 4096;
					}

					vecCol2Data2.push_back(temp2);
				}
			}


			std::vector<unsigned int> vecMatData = xmlParser->GetTileDataByLayerName("1_MAT");
			std::vector<unsigned int> vecMatData2;

			for (int i = 0; i < TileMapColY; i++) {
				int temp = TileMapColY - 1 - i;


				for (int j = 0; j < TileMapColX; j++) {
					int temp2 = temp * TileMapColX + j;
					temp2 = vecMatData[temp2];

					if (temp2 != 0) {
						temp2 -= 4096;
					}

					vecMatData2.push_back(temp2);
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

				int temp2 = vecCol2Data2[i];

				if (temp2 != 0) {
					RootTilemapBackground->ChangeTileOptionByIdx(i, ETileOption::Wall);
					continue;
				}

				int temp3 = vecMatData2[i];

				if (temp3 > 0) {
					if (temp3 == 19)
						RootTilemapBackground->ChangeTileOptionByIdx(i, ETileOption::Water);
					else if (temp3 == 48 || temp3 == 49)
						RootTilemapBackground->ChangeTileOptionByIdx(i, ETileOption::Stair);
					else if (temp3 == 50)
						RootTilemapBackground->ChangeTileOptionByIdx(i, ETileOption::Climb);

					continue;
				}
			}



		}

		{	}// vs 에러? 처리용 코드블럭

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
	Player->SetWorldPosition(CenterX - 1.5f * TileSize, 13.5f * TileSize, 10.f);
	//Player->SetWorldPosition(CenterX, 5200.f, 10.f);

	SetPlayerObject(Player);

	const Vector2& PortalSize = Vector2(TileSize * 1.5f, TileSize * 1.5f);

	CPortal* Portal = m_Owner->CreateObject<CPortal>("SlimePortal");
	Portal->SetWorldPosition(50.5 * TileSize, 119.5f * TileSize, 150.f);
	Portal->SetPortalSize(PortalSize);
	Portal->SetPortalDest(ESceneList::SludgeHeart);
	Portal->SetStartScene(ESceneList::StartingRuins);


	Portal = m_Owner->CreateObject<CPortal>("YetiPortal");
	Portal->SetWorldPosition(34.5f * TileSize, 128.5f * TileSize, 150.f);
	Portal->SetPortalSize(PortalSize);
	Portal->SetPortalDest(ESceneList::Yeti);
	Portal->SetStartScene(ESceneList::StartingRuins);


	Portal = m_Owner->CreateObject<CPortal>("KnightPortal");
	Portal->SetWorldPosition(66.5f * TileSize, 128.5f * TileSize, 150.f);
	Portal->SetPortalSize(PortalSize);
	Portal->SetPortalDest(ESceneList::Knight);
	Portal->SetStartScene(ESceneList::StartingRuins);


	CGoliathDoor* Door = m_Owner->CreateObject<CGoliathDoor>("ColossusDoor");
	Door->SetWorldPosition(50.5 * TileSize, 140.5f * TileSize, 150.f);



	CGoliath* Colossus = m_Owner->CreateObject<CGoliath>("Colossus");
	Colossus->SetWorldPosition(CenterX - 1.5f * TileSize, 162.f * TileSize);

	// Hand
	{
		const Vector3& vecMyPos = Colossus->GetWorldPos();

		CGoliathHand* LeftHand = m_Owner->CreateObject<CGoliathHand>("Colossus_LeftHand");
		LeftHand->SetHandLeft();


		LeftHand->SetWorldPosition(vecMyPos.x - 250.f, vecMyPos.y - 200.f, vecMyPos.z);



		CGoliathHand* RightHand = m_Owner->CreateObject<CGoliathHand>("Colossus_RightHand");
		RightHand->SetHandRight();

		RightHand->SetWorldPosition(vecMyPos.x + 250.f, vecMyPos.y - 200.f, vecMyPos.z);
	}




	return true;
}

void CStartingRuinSceneInfo::Update(float DeltaTime)
{
	CSceneInfo::Update(DeltaTime);


	CSound* Sound = m_Owner->GetResource()->FindSound("StartingRuin_Bgm");

	if(!m_BossStart && !Sound->GetPlay())
		Sound->Play();
	
}

void CStartingRuinSceneInfo::TitanStart()
{
	CSceneInfo::TitanStart();

	m_BossStart = true;

	CSound* Sound = m_Owner->GetResource()->FindSound("StartingRuin_Bgm");
	Sound->Stop();

	Sound = m_Owner->GetResource()->FindSound("Colossus");
	Sound->Play();

	Sound = m_Owner->GetResource()->FindSound("Colossus_Rise");
	Sound->Play();

	Sound = m_Owner->GetResource()->FindSound("Colossus_Roar");
	Sound->Play();



	// 문 닫히기
}

void CStartingRuinSceneInfo::TitanEnd()
{
	CSceneInfo::TitanEnd();

	m_Owner->GetResource()->SoundAllStop();

	CTileMapComponent* tileComp = (CTileMapComponent*)m_Owner->FindObject("StartingRuins_TilemapObjLayer_1_BG1")->FindComponent("StartingRuins_TilemapCompLayer_1_BG1");
	tileComp->GetTileMaterial()->SetBaseColor(Vector4::Gray);
	
	// 문 열리기

}
