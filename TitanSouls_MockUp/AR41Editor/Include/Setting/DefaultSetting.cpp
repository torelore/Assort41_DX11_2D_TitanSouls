#include "DefaultSetting.h"
#include "../GameObject/Player2D.h"
#include "../GameObject/Monster.h"
#include "../GameObject/Bullet.h"
#include "../UI/StartSceneUI.h"
#include "Scene/Scene.h"
#include "Input.h"
#include "CollisionManager.h"

CDefaultSetting::CDefaultSetting()
{
}

CDefaultSetting::~CDefaultSetting()
{
}

void CDefaultSetting::Init()
{
    LoadResource();

	CreateCDO();

	SetInput();

    SetCollision();
}

void CDefaultSetting::CreateCDO()
{
    CScene::CreateObjectCDO<CPlayer2D>("Player2D");
    CScene::CreateObjectCDO<CMonster>("Monster");
    CScene::CreateObjectCDO<CBullet>("Bullet");

    CScene::CreateUIWindowCDO<CStartSceneUI>("StartSceneUI");
}

void CDefaultSetting::LoadResource()
{
	std::vector<const char*> vecDir = { "Right", "Up", "Left", "Down", "RightDown", "LeftDown", "LeftUp", "RightUp" };


    // Player Animation
	{
		// Player
		{
			float standard = 16.f;

			// Idle(land)
			{
				const std::string& state = "Idle";

				for (size_t vecIdx = 0; vecIdx < vecDir.size(); vecIdx++) {
					char AnimName[40] = "";

					strcat_s(AnimName, state.c_str());
					strcat_s(AnimName, vecDir[vecIdx]);

					CResourceManager::GetInst()->CreateAnimationSequence2D(AnimName, "PlayerSprite", TEXT("IMG/player.png"));
					float xIdx = 0.f;
					float yIdx = standard * (float)vecIdx;
					CResourceManager::GetInst()->AddAnimationSequence2DFrame(AnimName, Vector2(xIdx, yIdx), Vector2(xIdx + standard, yIdx + standard));
				}
			} // Idle End


			// Move(land)
			{
				const std::string& state = "Move";

				for (size_t vecIdx = 0; vecIdx < vecDir.size(); vecIdx++) {
					char AnimName[40] = "";

					strcat_s(AnimName, state.c_str());
					strcat_s(AnimName, vecDir[vecIdx]);

					CResourceManager::GetInst()->CreateAnimationSequence2D(AnimName, "PlayerSprite", TEXT("IMG/player.png"));

					for (int i = 0; i < 6; ++i)
					{
						float xIdx = i * standard;
						float yIdx = standard * (float)vecIdx;
						CResourceManager::GetInst()->AddAnimationSequence2DFrame(AnimName, Vector2(xIdx, yIdx), Vector2(xIdx + standard, yIdx + standard));
					}
				}
			} // Move End


			// Dash
			{
				const std::string& state = "Dash";

				float standardX = 6.f * standard;

				for (size_t vecIdx = 0; vecIdx < vecDir.size(); vecIdx++) {
					char AnimName[40] = "";

					strcat_s(AnimName, state.c_str());
					strcat_s(AnimName, vecDir[vecIdx]);


					CResourceManager::GetInst()->CreateAnimationSequence2D(AnimName, "PlayerSprite", TEXT("IMG/player.png"));

					for (int i = 0; i < 8; ++i)
					{
						float xIdx = i * standard + standardX;
						float yIdx = standard * (float)vecIdx;
						CResourceManager::GetInst()->AddAnimationSequence2DFrame(AnimName, Vector2(xIdx, yIdx), Vector2(xIdx + standard, yIdx + standard));
					}
				}
			} // Dash End


			// Arrow Call
			{
				const std::string& state = "Call";

				for (size_t vecIdx = 0; vecIdx < vecDir.size(); vecIdx++) {
					char AnimName[40] = "";

					strcat_s(AnimName, state.c_str());
					strcat_s(AnimName, vecDir[vecIdx]);


					CResourceManager::GetInst()->CreateAnimationSequence2D(AnimName, "PlayerSprite", TEXT("IMG/player.png"));

					float xIdx = 14.f * standard;
					float yIdx = standard * (float)vecIdx;
					CResourceManager::GetInst()->AddAnimationSequence2DFrame(AnimName, Vector2(xIdx, yIdx), Vector2(xIdx + standard, yIdx + standard));
				}
			}


			// Aim
			{
				const std::string& state = "Aim";

				for (size_t vecIdx = 0; vecIdx < vecDir.size(); vecIdx++) {
					char AnimName[40] = "";

					strcat_s(AnimName, state.c_str());
					strcat_s(AnimName, vecDir[vecIdx]);


					CResourceManager::GetInst()->CreateAnimationSequence2D(AnimName, "PlayerSprite", TEXT("IMG/player.png"));

					float xIdx = 0.f;
					float yIdx = standard * (float)vecIdx;
					CResourceManager::GetInst()->AddAnimationSequence2DFrame(AnimName, Vector2(xIdx, yIdx), Vector2(xIdx + standard, yIdx + standard));
				}
			}


			// Swim
			{
				const std::string& state = "Swim";
				float standardY = standard * 8.f;

				for (size_t vecIdx = 0; vecIdx < vecDir.size(); vecIdx++) {
					char AnimName[40] = "";

					strcat_s(AnimName, state.c_str());
					strcat_s(AnimName, vecDir[vecIdx]);


					CResourceManager::GetInst()->CreateAnimationSequence2D(AnimName, "PlayerSprite", TEXT("IMG/player.png"));

					for (int i = 0; i < 8; ++i)
					{
						float xIdx = i * standard;
						float yIdx = standardY + standard * (float)vecIdx;
						CResourceManager::GetInst()->AddAnimationSequence2DFrame(AnimName, Vector2(xIdx, yIdx), Vector2(xIdx + standard, yIdx + standard));
					}
				}
			} // Swim End


			// Idle(water)
			{
				const std::string& state = "IdleSwim";
				float standardY = standard * 8.f;

				for (size_t vecIdx = 0; vecIdx < vecDir.size(); vecIdx++) {
					char AnimName[40] = "";

					strcat_s(AnimName, state.c_str());
					strcat_s(AnimName, vecDir[vecIdx]);


					CResourceManager::GetInst()->CreateAnimationSequence2D(AnimName, "PlayerSprite", TEXT("IMG/player.png"));

					for (int i = 0; i < 5; ++i)
					{
						float xIdx = i * standard;
						float yIdx = standardY + standard * (float)vecIdx;
						CResourceManager::GetInst()->AddAnimationSequence2DFrame(AnimName, Vector2(xIdx, yIdx), Vector2(xIdx + standard, yIdx + standard));
					}
				}
			} // Idle(water) End


			// Climb
			{
				CResourceManager::GetInst()->CreateAnimationSequence2D("Climb", "PlayerSprite", TEXT("IMG/player.png"));

				for (int i = 15; i < 20; ++i)
				{
					float xIdx = i * standard;
					float yIdx = standard * 1.f;
					CResourceManager::GetInst()->AddAnimationSequence2DFrame("Climb", Vector2(xIdx, yIdx), Vector2(xIdx + standard, yIdx + standard));
				}


				CResourceManager::GetInst()->CreateAnimationSequence2D("IdleClimb", "PlayerSprite", TEXT("IMG/player.png"));

				float xIdx = standard * 15.f;
				float yIdx = standard * 1.f;
				CResourceManager::GetInst()->AddAnimationSequence2DFrame("IdleClimb", Vector2(xIdx, yIdx), Vector2(xIdx + standard, yIdx + standard));
			} // Climb End


			// Dead
			{
				const std::string& state = "Dead";

				float standardX = 11.f * standard;


				for (size_t vecIdx = 0; vecIdx < vecDir.size(); vecIdx++) {
					char AnimName[40] = "";

					strcat_s(AnimName, state.c_str());
					strcat_s(AnimName, vecDir[vecIdx]);


					CResourceManager::GetInst()->CreateAnimationSequence2D(AnimName, "PlayerSprite", TEXT("IMG/player.png"));


					for (int i = 0; i < 5; i++)
					{
						float xIdx = standardX - i * standard;
						if (i == 4) {
							xIdx = standardX + standard;
						}

						float yIdx = standard * (float)vecIdx;

						CResourceManager::GetInst()->AddAnimationSequence2DFrame(AnimName, Vector2(xIdx, yIdx), Vector2(xIdx + standard, yIdx + standard));
					}
				}
			}


			// Ascent
			{
				float standardY = 16.f * standard + 1;
				const std::vector<float> vecFloat = { 3.f, 4.f, 0.f, 7.f, 1.f, 6.f, 2.f, 5.f };

				CResourceManager::GetInst()->CreateAnimationSequence2D("AscentRoll_Mono", "PlayerSprite", TEXT("IMG/player.png"));

				for (float iter : vecFloat) {
					float xIdx = iter * standard;
					float yIdx = standardY;
					CResourceManager::GetInst()->AddAnimationSequence2DFrame("AscentRoll_Mono", Vector2(xIdx, yIdx), Vector2(xIdx + standard, yIdx + standard));
				}


				CResourceManager::GetInst()->CreateAnimationSequence2D("AscentRoll", "PlayerSprite", TEXT("IMG/player.png"));

				for (float iter : vecFloat) {
					float xIdx = iter * standard;
					float yIdx = standardY + 16.f;
					CResourceManager::GetInst()->AddAnimationSequence2DFrame("AscentRoll", Vector2(xIdx, yIdx), Vector2(xIdx + standard, yIdx + standard));
				}


				CResourceManager::GetInst()->CreateAnimationSequence2D("AscentFall_Mono", "PlayerSprite", TEXT("IMG/player.png"));
				float standardX = standard * 8.f;

				for (int i = 0; i < 2; i++) {
					float xIdx = standardX + i * standard + standard;
					float yIdx = standardY;
					CResourceManager::GetInst()->AddAnimationSequence2DFrame("AscentFall_Mono", Vector2(xIdx, yIdx), Vector2(xIdx + standard, yIdx + standard));
				}


				CResourceManager::GetInst()->CreateAnimationSequence2D("AscentFall", "PlayerSprite", TEXT("IMG/player.png"));

				for (int i = 0; i < 3; i++) {
					float xIdx = standardX + i * standard;
					float yIdx = standardY + 16.f;
					CResourceManager::GetInst()->AddAnimationSequence2DFrame("AscentFall", Vector2(xIdx, yIdx), Vector2(xIdx + standard, yIdx + standard));
				}
			}

		}

		{} // VS 에러처리용 코드블럭
	}

	{} // VS 에러처리용 코드블럭

	// Monster Animation
	{
		// Yeti
		{
			{
				// Idle + Rest Animation
				{
					float standard = 96.f;

					CResourceManager::GetInst()->CreateAnimationSequence2D("Yeti_Idle", "YetiSprite", TEXT("IMG/BOSS/YETI/yeti.png"));

					for (int i = 0; i < 3; ++i)
					{
						float xIdx = i * standard;
						float yIdx = standard * 2.f;
						CResourceManager::GetInst()->AddAnimationSequence2DFrame("Yeti_Idle", Vector2(xIdx, yIdx), Vector2(xIdx + standard, yIdx + standard));
					}


					CResourceManager::GetInst()->CreateAnimationSequence2D("Yeti_RestRight", "YetiSprite", TEXT("IMG/BOSS/YETI/yeti.png"));

					for (int i = 0; i < 3; ++i)
					{
						float xIdx = i * standard;
						float yIdx = standard * 0.f;
						CResourceManager::GetInst()->AddAnimationSequence2DFrame("Yeti_RestRight", Vector2(xIdx, yIdx), Vector2(xIdx + standard, yIdx + standard));
					}


					CResourceManager::GetInst()->CreateAnimationSequence2D("Yeti_RestLeft", "YetiSprite", TEXT("IMG/BOSS/YETI/yeti.png"));

					for (int i = 0; i < 3; ++i)
					{
						float xIdx = i * standard;
						float yIdx = standard * 0.f;
						CResourceManager::GetInst()->AddAnimationSequence2DFrame("Yeti_RestLeft", Vector2(xIdx + standard, yIdx), Vector2(xIdx, yIdx + standard));
					}


					CResourceManager::GetInst()->CreateAnimationSequence2D("Yeti_RestUp", "YetiSprite", TEXT("IMG/BOSS/YETI/yeti.png"));

					for (int i = 0; i < 3; ++i)
					{
						float xIdx = i * standard;
						float yIdx = standard * 1.f;
						CResourceManager::GetInst()->AddAnimationSequence2DFrame("Yeti_RestUp", Vector2(xIdx, yIdx), Vector2(xIdx + standard, yIdx + standard));
					}


					CResourceManager::GetInst()->CreateAnimationSequence2D("Yeti_RestDown", "YetiSprite", TEXT("IMG/BOSS/YETI/yeti.png"));

					for (int i = 0; i < 3; ++i)
					{
						float xIdx = i * standard;
						float yIdx = standard * 2.f;
						CResourceManager::GetInst()->AddAnimationSequence2DFrame("Yeti_RestDown", Vector2(xIdx, yIdx), Vector2(xIdx + standard, yIdx + standard));
					}


					CResourceManager::GetInst()->CreateAnimationSequence2D("Yeti_RestRightUp", "YetiSprite", TEXT("IMG/BOSS/YETI/yeti.png"));

					for (int i = 0; i < 3; ++i)
					{
						float xIdx = i * standard;
						float yIdx = standard * 3.f;
						CResourceManager::GetInst()->AddAnimationSequence2DFrame("Yeti_RestRightUp", Vector2(xIdx, yIdx), Vector2(xIdx + standard, yIdx + standard));
					}


					CResourceManager::GetInst()->CreateAnimationSequence2D("Yeti_RestLeftUp", "YetiSprite", TEXT("IMG/BOSS/YETI/yeti.png"));

					for (int i = 0; i < 3; ++i)
					{
						float xIdx = i * standard;
						float yIdx = standard * 3.f;
						CResourceManager::GetInst()->AddAnimationSequence2DFrame("Yeti_RestLeftUp", Vector2(xIdx + standard, yIdx), Vector2(xIdx, yIdx + standard));
					}


					CResourceManager::GetInst()->CreateAnimationSequence2D("Yeti_RestRightDown", "YetiSprite", TEXT("IMG/BOSS/YETI/yeti.png"));

					for (int i = 0; i < 3; ++i)
					{
						float xIdx = i * standard;
						float yIdx = standard * 4.f;
						CResourceManager::GetInst()->AddAnimationSequence2DFrame("Yeti_RestRightDown", Vector2(xIdx, yIdx), Vector2(xIdx + standard, yIdx + standard));
					}


					CResourceManager::GetInst()->CreateAnimationSequence2D("Yeti_RestLeftDown", "YetiSprite", TEXT("IMG/BOSS/YETI/yeti.png"));

					for (int i = 0; i < 3; ++i)
					{
						float xIdx = i * standard;
						float yIdx = standard * 4.f;
						CResourceManager::GetInst()->AddAnimationSequence2DFrame("Yeti_RestLeftDown", Vector2(xIdx + standard, yIdx), Vector2(xIdx, yIdx + standard));
					}
				}


				// Roll Animation
				{
					float standard = 96.f;

					CResourceManager::GetInst()->CreateAnimationSequence2D("Yeti_RollRight", "YetiSprite", TEXT("IMG/BOSS/YETI/yeti.png"));

					for (int i = 0; i < 3; ++i)
					{
						float xIdx = (i + 6) * standard;
						float yIdx = standard * 0.f;
						CResourceManager::GetInst()->AddAnimationSequence2DFrame("Yeti_RollRight", Vector2(xIdx, yIdx), Vector2(xIdx + standard, yIdx + standard));
					}


					CResourceManager::GetInst()->CreateAnimationSequence2D("Yeti_RollLeft", "YetiSprite", TEXT("IMG/BOSS/YETI/yeti.png"));

					for (int i = 0; i < 3; ++i)
					{
						float xIdx = (i + 6) * standard;
						float yIdx = standard * 0.f;
						CResourceManager::GetInst()->AddAnimationSequence2DFrame("Yeti_RollLeft", Vector2(xIdx + standard, yIdx), Vector2(xIdx, yIdx + standard));
					}


					CResourceManager::GetInst()->CreateAnimationSequence2D("Yeti_RollUp", "YetiSprite", TEXT("IMG/BOSS/YETI/yeti.png"));

					for (int i = 0; i < 3; ++i)
					{
						float xIdx = (i + 6) * standard;
						float yIdx = standard * 1.f;
						CResourceManager::GetInst()->AddAnimationSequence2DFrame("Yeti_RollUp", Vector2(xIdx, yIdx), Vector2(xIdx + standard, yIdx + standard));
					}


					CResourceManager::GetInst()->CreateAnimationSequence2D("Yeti_RollDown", "YetiSprite", TEXT("IMG/BOSS/YETI/yeti.png"));

					for (int i = 0; i < 3; ++i)
					{
						float xIdx = (i + 6) * standard;
						float yIdx = standard * 2.f;
						CResourceManager::GetInst()->AddAnimationSequence2DFrame("Yeti_RollDown", Vector2(xIdx, yIdx), Vector2(xIdx + standard, yIdx + standard));
					}


					CResourceManager::GetInst()->CreateAnimationSequence2D("Yeti_RollRightUp", "YetiSprite", TEXT("IMG/BOSS/YETI/yeti.png"));

					for (int i = 0; i < 3; ++i)
					{
						float xIdx = (i + 6) * standard;
						float yIdx = standard * 3.f;
						CResourceManager::GetInst()->AddAnimationSequence2DFrame("Yeti_RollRightUp", Vector2(xIdx, yIdx), Vector2(xIdx + standard, yIdx + standard));
					}


					CResourceManager::GetInst()->CreateAnimationSequence2D("Yeti_RollRightDown", "YetiSprite", TEXT("IMG/BOSS/YETI/yeti.png"));

					for (int i = 0; i < 3; ++i)
					{
						float xIdx = (i + 6) * standard;
						float yIdx = standard * 4.f;
						CResourceManager::GetInst()->AddAnimationSequence2DFrame("Yeti_RollRightDown", Vector2(xIdx, yIdx), Vector2(xIdx + standard, yIdx + standard));
					}


					CResourceManager::GetInst()->CreateAnimationSequence2D("Yeti_RollLeftUp", "YetiSprite", TEXT("IMG/BOSS/YETI/yeti.png"));

					for (int i = 0; i < 3; ++i)
					{
						float xIdx = (i + 6) * standard;
						float yIdx = standard * 3.f;
						CResourceManager::GetInst()->AddAnimationSequence2DFrame("Yeti_RollLeftUp", Vector2(xIdx + standard, yIdx), Vector2(xIdx, yIdx + standard));
					}


					CResourceManager::GetInst()->CreateAnimationSequence2D("Yeti_RollLeftDown", "YetiSprite", TEXT("IMG/BOSS/YETI/yeti.png"));

					for (int i = 0; i < 3; ++i)
					{
						float xIdx = (i + 6) * standard;
						float yIdx = standard * 4.f;
						CResourceManager::GetInst()->AddAnimationSequence2DFrame("Yeti_RollLeftDown", Vector2(xIdx + standard, yIdx), Vector2(xIdx, yIdx + standard));
					}

				}


				// Roll After Animation
				{
					float standard = 96.f;

					CResourceManager::GetInst()->CreateAnimationSequence2D("Yeti_RollAfterRight", "YetiSprite", TEXT("IMG/BOSS/YETI/yeti.png"));

					for (int i = 0; i < 4; ++i)
					{
						float xIdx = (i + 6) * standard;
						float yIdx = standard * 0.f;
						CResourceManager::GetInst()->AddAnimationSequence2DFrame("Yeti_RollAfterRight", Vector2(xIdx, yIdx), Vector2(xIdx + standard, yIdx + standard));
					}


					CResourceManager::GetInst()->CreateAnimationSequence2D("Yeti_RollAfterLeft", "YetiSprite", TEXT("IMG/BOSS/YETI/yeti.png"));

					for (int i = 0; i < 4; ++i)
					{
						float xIdx = (i + 6) * standard;
						float yIdx = standard * 0.f;
						CResourceManager::GetInst()->AddAnimationSequence2DFrame("Yeti_RollAfterLeft", Vector2(xIdx + standard, yIdx), Vector2(xIdx, yIdx + standard));
					}


					CResourceManager::GetInst()->CreateAnimationSequence2D("Yeti_RollAfterUp", "YetiSprite", TEXT("IMG/BOSS/YETI/yeti.png"));

					for (int i = 0; i < 4; ++i)
					{
						float xIdx = (i + 6) * standard;
						float yIdx = standard * 1.f;
						CResourceManager::GetInst()->AddAnimationSequence2DFrame("Yeti_RollAfterUp", Vector2(xIdx, yIdx), Vector2(xIdx + standard, yIdx + standard));
					}


					CResourceManager::GetInst()->CreateAnimationSequence2D("Yeti_RollAfterDown", "YetiSprite", TEXT("IMG/BOSS/YETI/yeti.png"));

					for (int i = 0; i < 4; ++i)
					{
						float xIdx = (i + 6) * standard;
						float yIdx = standard * 2.f;
						CResourceManager::GetInst()->AddAnimationSequence2DFrame("Yeti_RollAfterDown", Vector2(xIdx, yIdx), Vector2(xIdx + standard, yIdx + standard));
					}


					CResourceManager::GetInst()->CreateAnimationSequence2D("Yeti_RollAfterRightUp", "YetiSprite", TEXT("IMG/BOSS/YETI/yeti.png"));

					for (int i = 0; i < 4; ++i)
					{
						float xIdx = (i + 6) * standard;
						float yIdx = standard * 3.f;
						CResourceManager::GetInst()->AddAnimationSequence2DFrame("Yeti_RollAfterRightUp", Vector2(xIdx, yIdx), Vector2(xIdx + standard, yIdx + standard));
					}


					CResourceManager::GetInst()->CreateAnimationSequence2D("Yeti_RollAfterRightDown", "YetiSprite", TEXT("IMG/BOSS/YETI/yeti.png"));

					for (int i = 0; i < 4; ++i)
					{
						float xIdx = (i + 6) * standard;
						float yIdx = standard * 4.f;
						CResourceManager::GetInst()->AddAnimationSequence2DFrame("Yeti_RollAfterRightDown", Vector2(xIdx, yIdx), Vector2(xIdx + standard, yIdx + standard));
					}


					CResourceManager::GetInst()->CreateAnimationSequence2D("Yeti_RollAfterLeftUp", "YetiSprite", TEXT("IMG/BOSS/YETI/yeti.png"));

					for (int i = 0; i < 4; ++i)
					{
						float xIdx = (i + 6) * standard;
						float yIdx = standard * 3.f;
						CResourceManager::GetInst()->AddAnimationSequence2DFrame("Yeti_RollAfterLeftUp", Vector2(xIdx + standard, yIdx), Vector2(xIdx, yIdx + standard));
					}


					CResourceManager::GetInst()->CreateAnimationSequence2D("Yeti_RollAfterLeftDown", "YetiSprite", TEXT("IMG/BOSS/YETI/yeti.png"));

					for (int i = 0; i < 4; ++i)
					{
						float xIdx = (i + 6) * standard;
						float yIdx = standard * 4.f;
						CResourceManager::GetInst()->AddAnimationSequence2DFrame("Yeti_RollAfterLeftDown", Vector2(xIdx + standard, yIdx), Vector2(xIdx, yIdx + standard));
					}

				}


				// Shoot Animation
				{
					float standard = 96.f;
					float standardX = standard * 3.f;

					CResourceManager::GetInst()->CreateAnimationSequence2D("Yeti_ShootLeft", "YetiSprite", TEXT("IMG/BOSS/YETI/yeti.png"));

					for (int i = 0; i < 3; ++i)
					{
						float xIdx = i * standard + standardX;
						float yIdx = standard * 0.f;
						CResourceManager::GetInst()->AddAnimationSequence2DFrame("Yeti_ShootLeft", Vector2(xIdx + standard, yIdx), Vector2(xIdx, yIdx + standard));
					}


					CResourceManager::GetInst()->CreateAnimationSequence2D("Yeti_ShootRight", "YetiSprite", TEXT("IMG/BOSS/YETI/yeti.png"));

					for (int i = 0; i < 3; ++i)
					{
						float xIdx = i * standard + standardX;
						float yIdx = standard * 0.f;
						CResourceManager::GetInst()->AddAnimationSequence2DFrame("Yeti_ShootRight", Vector2(xIdx, yIdx), Vector2(xIdx + standard, yIdx + standard));
					}


					CResourceManager::GetInst()->CreateAnimationSequence2D("Yeti_ShootUp", "YetiSprite", TEXT("IMG/BOSS/YETI/yeti.png"));

					for (int i = 0; i < 3; ++i)
					{
						float xIdx = i * standard + standardX;
						float yIdx = standard * 1.f;
						CResourceManager::GetInst()->AddAnimationSequence2DFrame("Yeti_ShootUp", Vector2(xIdx, yIdx), Vector2(xIdx + standard, yIdx + standard));
					}


					CResourceManager::GetInst()->CreateAnimationSequence2D("Yeti_ShootDown", "YetiSprite", TEXT("IMG/BOSS/YETI/yeti.png"));

					for (int i = 0; i < 3; ++i)
					{
						float xIdx = i * standard + standardX;
						float yIdx = standard * 2.f;
						CResourceManager::GetInst()->AddAnimationSequence2DFrame("Yeti_ShootDown", Vector2(xIdx, yIdx), Vector2(xIdx + standard, yIdx + standard));
					}


					CResourceManager::GetInst()->CreateAnimationSequence2D("Yeti_ShootLeftUp", "YetiSprite", TEXT("IMG/BOSS/YETI/yeti.png"));

					for (int i = 0; i < 3; ++i)
					{
						float xIdx = i * standard + standardX;
						float yIdx = standard * 3.f;
						CResourceManager::GetInst()->AddAnimationSequence2DFrame("Yeti_ShootLeftUp", Vector2(xIdx + standard, yIdx), Vector2(xIdx, yIdx + standard));
					}


					CResourceManager::GetInst()->CreateAnimationSequence2D("Yeti_ShootRightUp", "YetiSprite", TEXT("IMG/BOSS/YETI/yeti.png"));

					for (int i = 0; i < 3; ++i)
					{
						float xIdx = i * standard + standardX;
						float yIdx = standard * 3.f;
						CResourceManager::GetInst()->AddAnimationSequence2DFrame("Yeti_ShootRightUp", Vector2(xIdx, yIdx), Vector2(xIdx + standard, yIdx + standard));
					}


					CResourceManager::GetInst()->CreateAnimationSequence2D("Yeti_ShootLeftDown", "YetiSprite", TEXT("IMG/BOSS/YETI/yeti.png"));

					for (int i = 0; i < 3; ++i)
					{
						float xIdx = i * standard + standardX;
						float yIdx = standard * 4.f;
						CResourceManager::GetInst()->AddAnimationSequence2DFrame("Yeti_ShootLeftDown", Vector2(xIdx + standard, yIdx), Vector2(xIdx, yIdx + standard));
					}


					CResourceManager::GetInst()->CreateAnimationSequence2D("Yeti_ShootRightDown", "YetiSprite", TEXT("IMG/BOSS/YETI/yeti.png"));

for (int i = 0; i < 3; ++i)
{
	float xIdx = i * standard + standardX;
	float yIdx = standard * 4.f;
	CResourceManager::GetInst()->AddAnimationSequence2DFrame("Yeti_ShootRightDown", Vector2(xIdx, yIdx), Vector2(xIdx + standard, yIdx + standard));
}
				}


				// Dead
				{
				float standard = 96.f;
				float standardX = standard * 4.f;
				float standardY = standard * 5.f;

				CResourceManager::GetInst()->CreateAnimationSequence2D("Yeti_DeadLeft", "YetiSprite", TEXT("IMG/BOSS/YETI/yeti.png"));

				for (int i = 0; i < 3; ++i)
				{
					float xIdx = i * standard + standardX;
					float yIdx = standard * 0.f + standardY;
					CResourceManager::GetInst()->AddAnimationSequence2DFrame("Yeti_DeadLeft", Vector2(xIdx + standard, yIdx), Vector2(xIdx, yIdx + standard));
				}


				CResourceManager::GetInst()->CreateAnimationSequence2D("Yeti_DeadRight", "YetiSprite", TEXT("IMG/BOSS/YETI/yeti.png"));

				for (int i = 0; i < 3; ++i)
				{
					float xIdx = i * standard + standardX;
					float yIdx = standard * 0.f;
					CResourceManager::GetInst()->AddAnimationSequence2DFrame("Yeti_DeadRight", Vector2(xIdx, yIdx), Vector2(xIdx + standard, yIdx + standard));
				}


				CResourceManager::GetInst()->CreateAnimationSequence2D("Yeti_DeadUp", "YetiSprite", TEXT("IMG/BOSS/YETI/yeti.png"));

				for (int i = 0; i < 3; ++i)
				{
					float xIdx = i * standard + standardX;
					float yIdx = standard * 1.f + standardY;
					CResourceManager::GetInst()->AddAnimationSequence2DFrame("Yeti_DeadUp", Vector2(xIdx, yIdx), Vector2(xIdx + standard, yIdx + standard));
				}


				CResourceManager::GetInst()->CreateAnimationSequence2D("Yeti_DeadDown", "YetiSprite", TEXT("IMG/BOSS/YETI/yeti.png"));

				for (int i = 0; i < 3; ++i)
				{
					float xIdx = i * standard + standardX;
					float yIdx = standard * 2.f + standardY;
					CResourceManager::GetInst()->AddAnimationSequence2DFrame("Yeti_DeadDown", Vector2(xIdx, yIdx), Vector2(xIdx + standard, yIdx + standard));
				}


				CResourceManager::GetInst()->CreateAnimationSequence2D("Yeti_DeadLeftUp", "YetiSprite", TEXT("IMG/BOSS/YETI/yeti.png"));

				for (int i = 0; i < 3; ++i)
				{
					float xIdx = i * standard + standardX;
					float yIdx = standard * 3.f + standardY;
					CResourceManager::GetInst()->AddAnimationSequence2DFrame("Yeti_DeadLeft", Vector2(xIdx + standard, yIdx), Vector2(xIdx, yIdx + standard));
				}


				CResourceManager::GetInst()->CreateAnimationSequence2D("Yeti_DeadRightUp", "YetiSprite", TEXT("IMG/BOSS/YETI/yeti.png"));

				for (int i = 0; i < 3; ++i)
				{
					float xIdx = i * standard + standardX;
					float yIdx = standard * 3.f;
					CResourceManager::GetInst()->AddAnimationSequence2DFrame("Yeti_DeadRight", Vector2(xIdx, yIdx), Vector2(xIdx + standard, yIdx + standard));
				}


				CResourceManager::GetInst()->CreateAnimationSequence2D("Yeti_DeadLeftDown", "YetiSprite", TEXT("IMG/BOSS/YETI/yeti.png"));

				for (int i = 0; i < 3; ++i)
				{
					float xIdx = i * standard + standardX;
					float yIdx = standard * 4.f + standardY;
					CResourceManager::GetInst()->AddAnimationSequence2DFrame("Yeti_DeadLeftDown", Vector2(xIdx + standard, yIdx), Vector2(xIdx, yIdx + standard));
				}


				CResourceManager::GetInst()->CreateAnimationSequence2D("Yeti_DeadRightDown", "YetiSprite", TEXT("IMG/BOSS/YETI/yeti.png"));

				for (int i = 0; i < 3; ++i)
				{
					float xIdx = i * standard + standardX;
					float yIdx = standard * 4.f;
					CResourceManager::GetInst()->AddAnimationSequence2DFrame("Yeti_DeadRightDown", Vector2(xIdx, yIdx), Vector2(xIdx + standard, yIdx + standard));
				}


				}
			}
		}

		{} // VS 에러처리용 코드블럭

		// Knight
		{
			// Animation
			const std::string& KnightName = "Knight_";

			// Animation Create
			{
				float standard = 96.f;

				// Idle
				{
					const std::string& state = "Idle";

					for (size_t vecIdx = 0; vecIdx < vecDir.size(); vecIdx++) {
						char AnimName[40] = "";

						strcat_s(AnimName, KnightName.c_str());
						strcat_s(AnimName, state.c_str());
						strcat_s(AnimName, vecDir[vecIdx]);

						CResourceManager::GetInst()->CreateAnimationSequence2D(AnimName, "KnightSprite", TEXT("IMG/BOSS/KNIGHT/knight.png"));
						float xIdx = 0.f;
						float yIdx = standard * (float)vecIdx;
						CResourceManager::GetInst()->AddAnimationSequence2DFrame(AnimName, Vector2(xIdx, yIdx), Vector2(xIdx + standard, yIdx + standard));
					}
				} // Idle End


				// KneelDown
				{
					float standardX = standard * 1.f;
					const std::string& state = "KneelDown";

					for (size_t vecIdx = 0; vecIdx < vecDir.size(); vecIdx++) {
						char AnimName[40] = "";

						strcat_s(AnimName, KnightName.c_str());
						strcat_s(AnimName, state.c_str());
						strcat_s(AnimName, vecDir[vecIdx]);

						CResourceManager::GetInst()->CreateAnimationSequence2D(AnimName, "KnightSprite", TEXT("IMG/BOSS/KNIGHT/knight.png"));
						float xIdx = standardX;
						float yIdx = standard * (float)vecIdx;
						CResourceManager::GetInst()->AddAnimationSequence2DFrame(AnimName, Vector2(xIdx, yIdx), Vector2(xIdx + standard, yIdx + standard));
					}
				} // KneelDown End


				// KneelDown_Skull
				{
					float standardX = standard * 5.f;
					const std::string& state = "KneelDown_Skull";

					for (size_t vecIdx = 0; vecIdx < vecDir.size(); vecIdx++) {
						char AnimName[40] = "";

						strcat_s(AnimName, KnightName.c_str());
						strcat_s(AnimName, state.c_str());
						strcat_s(AnimName, vecDir[vecIdx]);

						CResourceManager::GetInst()->CreateAnimationSequence2D(AnimName, "KnightSprite", TEXT("IMG/BOSS/KNIGHT/knight.png"));
						float xIdx = standardX;
						float yIdx = standard * (float)vecIdx;
						CResourceManager::GetInst()->AddAnimationSequence2DFrame(AnimName, Vector2(xIdx, yIdx), Vector2(xIdx + standard, yIdx + standard));
					}
				} // KneelDown End


				// AimBigArrow 
				{
					const std::string& state = "AimBigArrow";

					char AnimName[40] = "";

					strcat_s(AnimName, KnightName.c_str());
					strcat_s(AnimName, state.c_str());

					CResourceManager::GetInst()->CreateAnimationSequence2D(AnimName, "KnightSprite", TEXT("IMG/BOSS/KNIGHT/knight.png"));
					float xIdx = standard * 18;
					float yIdx = standard * 3;
					CResourceManager::GetInst()->AddAnimationSequence2DFrame(AnimName, Vector2(xIdx, yIdx), Vector2(xIdx + standard, yIdx + standard));
				} // Aim Big Arrow End


				// AimArrow 
				{
					const std::string& state = "AimArrow";
					float standardX = standard * 3.f;

					for (size_t vecIdx = 0; vecIdx < vecDir.size(); vecIdx++) {
						char AnimName[40] = "";

						strcat_s(AnimName, KnightName.c_str());
						strcat_s(AnimName, state.c_str());
						strcat_s(AnimName, vecDir[vecIdx]);

						CResourceManager::GetInst()->CreateAnimationSequence2D(AnimName, "KnightSprite", TEXT("IMG/BOSS/KNIGHT/knight.png"));

						for (int i = 0; i < 2; i++) {
							float xIdx = standardX + standard * i;
							float yIdx = standard * (float)vecIdx;
							CResourceManager::GetInst()->AddAnimationSequence2DFrame(AnimName, Vector2(xIdx, yIdx), Vector2(xIdx + standard, yIdx + standard));
						}
					}
				} // Aim Arrow End


				// ShootArrow 
				{
					const std::string& state = "ShootArrow";
					float standardX = standard * 6.f;

					for (size_t vecIdx = 0; vecIdx < vecDir.size(); vecIdx++) {
						char AnimName[40] = "";

						strcat_s(AnimName, KnightName.c_str());
						strcat_s(AnimName, state.c_str());
						strcat_s(AnimName, vecDir[vecIdx]);

						CResourceManager::GetInst()->CreateAnimationSequence2D(AnimName, "KnightSprite", TEXT("IMG/BOSS/KNIGHT/knight.png"));

						for (int i = 0; i < 2; i++) {
							float xIdx = standardX + standard * i;
							float yIdx = standard * (float)vecIdx;
							CResourceManager::GetInst()->AddAnimationSequence2DFrame(AnimName, Vector2(xIdx, yIdx), Vector2(xIdx + standard, yIdx + standard));
						}
					}
				} // Shoot Arrow End


				// Move
				{
					const std::string& state = "Move";
					float standardX = standard * 14.f;

					for (size_t vecIdx = 0; vecIdx < vecDir.size(); vecIdx++) {
						char AnimName[40] = "";

						strcat_s(AnimName, KnightName.c_str());
						strcat_s(AnimName, state.c_str());
						strcat_s(AnimName, vecDir[vecIdx]);

						CResourceManager::GetInst()->CreateAnimationSequence2D(AnimName, "KnightSprite", TEXT("IMG/BOSS/KNIGHT/knight.png"));

						for (int i = 0; i < 4; i++) {
							float xIdx = standardX + standard * i;
							float yIdx = standard * (float)vecIdx;
							CResourceManager::GetInst()->AddAnimationSequence2DFrame(AnimName, Vector2(xIdx, yIdx), Vector2(xIdx + standard, yIdx + standard));
						}
					}
				} // Move End


				// Dead_Skull
				{
					float standardX = standard * 8.f;
					const std::string& state = "Dead_Skull";

					for (size_t vecIdx = 0; vecIdx < vecDir.size(); vecIdx++) {
						char AnimName[40] = "";

						strcat_s(AnimName, KnightName.c_str());
						strcat_s(AnimName, state.c_str());
						strcat_s(AnimName, vecDir[vecIdx]);

						CResourceManager::GetInst()->CreateAnimationSequence2D(AnimName, "KnightSprite", TEXT("IMG/BOSS/KNIGHT/knight.png"));
						float xIdx = standardX;
						float yIdx = standard * (float)vecIdx;
						CResourceManager::GetInst()->AddAnimationSequence2DFrame(AnimName, Vector2(xIdx, yIdx), Vector2(xIdx + standard, yIdx + standard));
					}
				} // Dead_Skull End

			}

		}

		{} // VS 에러처리용 코드블럭

		// Colossus
		{
			const std::string& Name = "Colossus_";

			// Animation Create
			{
				// Face
				{
					float standardX = 64.f;
					float standardY = 70.f;

					const std::string& state = "FaceRoar";

					char AnimName[40] = "";

					strcat_s(AnimName, Name.c_str());
					strcat_s(AnimName, state.c_str());

					CResourceManager::GetInst()->CreateAnimationSequence2D(AnimName, "ColossusSprite", TEXT("IMG/BOSS/COLOSSUS/colossus.png"));


					float xIdx = 0.f;
					float yIdx = 0.f;
					CResourceManager::GetInst()->AddAnimationSequence2DFrame(AnimName, Vector2(xIdx, yIdx), Vector2(xIdx + standardX, yIdx + standardY));

					xIdx += standardX;
					CResourceManager::GetInst()->AddAnimationSequence2DFrame(AnimName, Vector2(xIdx, yIdx), Vector2(xIdx + standardX, yIdx + standardY));

					xIdx += standardX;
					CResourceManager::GetInst()->AddAnimationSequence2DFrame(AnimName, Vector2(xIdx, yIdx), Vector2(xIdx + standardX, yIdx + standardY));
					CResourceManager::GetInst()->AddAnimationSequence2DFrame(AnimName, Vector2(xIdx, yIdx), Vector2(xIdx + standardX, yIdx + standardY));
					CResourceManager::GetInst()->AddAnimationSequence2DFrame(AnimName, Vector2(xIdx, yIdx), Vector2(xIdx + standardX, yIdx + standardY));
					CResourceManager::GetInst()->AddAnimationSequence2DFrame(AnimName, Vector2(xIdx, yIdx), Vector2(xIdx + standardX, yIdx + standardY));
					CResourceManager::GetInst()->AddAnimationSequence2DFrame(AnimName, Vector2(xIdx, yIdx), Vector2(xIdx + standardX, yIdx + standardY));

					xIdx += standardX;
					CResourceManager::GetInst()->AddAnimationSequence2DFrame(AnimName, Vector2(xIdx, yIdx), Vector2(xIdx + standardX, yIdx + standardY));
					CResourceManager::GetInst()->AddAnimationSequence2DFrame(AnimName, Vector2(xIdx, yIdx), Vector2(xIdx + standardX, yIdx + standardY));

					xIdx += standardX;
					CResourceManager::GetInst()->AddAnimationSequence2DFrame(AnimName, Vector2(xIdx, yIdx), Vector2(xIdx + standardX, yIdx + standardY));

					//for (int i = 0; i < 5; i++) {
					//	float xIdx = i * standardX;
					//	float yIdx = 0.f;
					//	CResourceManager::GetInst()->AddAnimationSequence2DFrame(AnimName, Vector2(xIdx, yIdx), Vector2(xIdx + standardX, yIdx + standardY));
					//}


					CResourceManager::GetInst()->CreateAnimationSequence2D("Colossus_FaceIdle", "ColossusSprite", TEXT("IMG/BOSS/COLOSSUS/colossus.png"));
					CResourceManager::GetInst()->AddAnimationSequence2DFrame("Colossus_FaceIdle", Vector2(0.f, 0.f), Vector2(0.f + standardX, 0.f + standardY));
				} // Face End


				// Body
				{
				const std::string& state = "Body";

				char AnimName[40] = "";

				strcat_s(AnimName, Name.c_str());
				strcat_s(AnimName, state.c_str());

				CResourceManager::GetInst()->CreateAnimationSequence2D(AnimName, "ColossusSprite", TEXT("IMG/BOSS/COLOSSUS/colossus.png"));

				float xIdx = 0.f;
				float yIdx = 120.f;
				float standardX = 96.f;
				//float standardY = 110.f; // Full Body
				float standardY = 70.f;
				CResourceManager::GetInst()->AddAnimationSequence2DFrame(AnimName, Vector2(xIdx, yIdx), Vector2(xIdx + standardX, yIdx + standardY));
				} // Body End


				// BodyPaint
				{
					const std::string& state = "BodyPaint";

					char AnimName[40] = "";

					strcat_s(AnimName, Name.c_str());
					strcat_s(AnimName, state.c_str());

					CResourceManager::GetInst()->CreateAnimationSequence2D(AnimName, "ColossusSprite", TEXT("IMG/BOSS/COLOSSUS/colossus.png"));

					float xIdx = 288.f;
					float yIdx = 120.f;
					float standardX = 96.f;
					float standardY = 110.f;
					CResourceManager::GetInst()->AddAnimationSequence2DFrame(AnimName, Vector2(xIdx, yIdx), Vector2(xIdx + standardX, yIdx + standardY));
				} // BodyPaint End


				// SholderLeft
				{
					const std::string& state = "SholderLeft";

					char AnimName[40] = "";

					strcat_s(AnimName, Name.c_str());
					strcat_s(AnimName, state.c_str());

					float xIdx = 150.f;
					float yIdx = 100.f;
					float standardX = 42.f;
					float standardY = 42.f;

					const std::string& Left = "Right";


					CResourceManager::GetInst()->CreateAnimationSequence2D(AnimName, "ColossusSprite", TEXT("IMG/BOSS/COLOSSUS/colossus.png"));

					CResourceManager::GetInst()->AddAnimationSequence2DFrame(AnimName, Vector2(xIdx, yIdx), Vector2(xIdx + standardX, yIdx + standardY));
				} // SholderLeft End


				// SholderRight
				{
					const std::string& state = "SholderRight";

					char AnimName[40] = "";

					strcat_s(AnimName, Name.c_str());
					strcat_s(AnimName, state.c_str());

					float xIdx = 192.f;
					float yIdx = 100.f;
					float standardX = 42.f;
					float standardY = 42.f;

					CResourceManager::GetInst()->CreateAnimationSequence2D(AnimName, "ColossusSprite", TEXT("IMG/BOSS/COLOSSUS/colossus.png"));

					CResourceManager::GetInst()->AddAnimationSequence2DFrame(AnimName, Vector2(xIdx, yIdx), Vector2(xIdx + standardX, yIdx + standardY));
				} // SholderRight End
			}

			{} // VS 에러처리용 코드블럭

			// Colossus_LeftHand
			{
				const std::string& Name2 = "LeftHand_";
				float StartX = 256.f;
				float StandardX = 64.f;
				float StandardY = 64.f;

				// Idle
				{
					const std::string& state = "Idle";
					char AnimName[40] = "";

					strcat_s(AnimName, Name.c_str());
					strcat_s(AnimName, Name2.c_str());
					strcat_s(AnimName, state.c_str());

					float xIdx = StartX;
					float yIdx = 256.f;

					CResourceManager::GetInst()->CreateAnimationSequence2D(AnimName, "ColossusSprite", TEXT("IMG/BOSS/COLOSSUS/colossus.png"));

					// 1
					CResourceManager::GetInst()->AddAnimationSequence2DFrame(AnimName, Vector2(xIdx, yIdx), Vector2(xIdx + StandardX, yIdx + StandardY));
				}

				// Up
				{
					const std::string& state = "Up";
					char AnimName[40] = "";

					strcat_s(AnimName, Name.c_str());
					strcat_s(AnimName, Name2.c_str());
					strcat_s(AnimName, state.c_str());

					float xIdx = StartX + StandardX;
					float yIdx = 256.f;

					CResourceManager::GetInst()->CreateAnimationSequence2D(AnimName, "ColossusSprite", TEXT("IMG/BOSS/COLOSSUS/colossus.png"));

					// 2
					CResourceManager::GetInst()->AddAnimationSequence2DFrame(AnimName, Vector2(xIdx, yIdx), Vector2(xIdx + StandardX, yIdx + StandardY));
				}

				
				// MoveFront
				{
					const std::string& state = "MoveFront";
					char AnimName[40] = "";

					strcat_s(AnimName, Name.c_str());
					strcat_s(AnimName, Name2.c_str());
					strcat_s(AnimName, state.c_str());

					float xIdx = StartX + StandardX * 3;
					float yIdx = 256.f;

					CResourceManager::GetInst()->CreateAnimationSequence2D(AnimName, "ColossusSprite", TEXT("IMG/BOSS/COLOSSUS/colossus.png"));

					// 4
					CResourceManager::GetInst()->AddAnimationSequence2DFrame(AnimName, Vector2(xIdx, yIdx), Vector2(xIdx + StandardX, yIdx + StandardY));
				}


				// MoveBack
				{
					const std::string& state = "MoveBack";
					char AnimName[40] = "";

					strcat_s(AnimName, Name.c_str());
					strcat_s(AnimName, Name2.c_str());
					strcat_s(AnimName, state.c_str());

					float xIdx = StartX + StandardX;
					float yIdx = 256.f;

					CResourceManager::GetInst()->CreateAnimationSequence2D(AnimName, "ColossusSprite", TEXT("IMG/BOSS/COLOSSUS/colossus.png"));

					// 2
					CResourceManager::GetInst()->AddAnimationSequence2DFrame(AnimName, Vector2(xIdx, yIdx), Vector2(xIdx + StandardX, yIdx + StandardY));
				}

				// Guard
				{
					const std::string& state = "Guard";
					char AnimName[40] = "";

					strcat_s(AnimName, Name.c_str());
					strcat_s(AnimName, Name2.c_str());
					strcat_s(AnimName, state.c_str());

					float xIdx = StartX + StandardX * 2;
					float yIdx = 256.f;

					CResourceManager::GetInst()->CreateAnimationSequence2D(AnimName, "ColossusSprite", TEXT("IMG/BOSS/COLOSSUS/colossus.png"));

					// 3
					CResourceManager::GetInst()->AddAnimationSequence2DFrame(AnimName, Vector2(xIdx, yIdx), Vector2(xIdx + StandardX, yIdx + StandardY));
				}

				// Dead
				{
					const std::string& state = "Dead";
					char AnimName[40] = "";

					strcat_s(AnimName, Name.c_str());
					strcat_s(AnimName, Name2.c_str());
					strcat_s(AnimName, state.c_str());

					float xIdx = 400.f;
					float yIdx = 192.f;

					CResourceManager::GetInst()->CreateAnimationSequence2D(AnimName, "ColossusSprite", TEXT("IMG/BOSS/COLOSSUS/colossus.png"));
					CResourceManager::GetInst()->AddAnimationSequence2DFrame(AnimName, Vector2(xIdx, yIdx), Vector2(xIdx + StandardX, yIdx + StandardY));
				}
			}

			{} // VS 에러처리용 코드블럭

			// Colossus_RightHand
			{
				const std::string& Name2 = "RightHand_";
				float StartX = 256.f;
				float StandardX = 64.f;
				float StandardY = 64.f;

				// Idle
				{
					const std::string& state = "Idle";
					char AnimName[40] = "";

					strcat_s(AnimName, Name.c_str());
					strcat_s(AnimName, Name2.c_str());
					strcat_s(AnimName, state.c_str());

					float xIdx = StartX;
					float yIdx = 256.f;

					CResourceManager::GetInst()->CreateAnimationSequence2D(AnimName, "ColossusSprite", TEXT("IMG/BOSS/COLOSSUS/colossus.png"));

					// 1
					CResourceManager::GetInst()->AddAnimationSequence2DFrame(AnimName, Vector2(xIdx + StandardX, yIdx), Vector2(xIdx, yIdx + StandardY));
				}


				// Up
				{
					const std::string& state = "Up";
					char AnimName[40] = "";

					strcat_s(AnimName, Name.c_str());
					strcat_s(AnimName, Name2.c_str());
					strcat_s(AnimName, state.c_str());

					float xIdx = StartX + StandardX;
					float yIdx = 256.f;

					CResourceManager::GetInst()->CreateAnimationSequence2D(AnimName, "ColossusSprite", TEXT("IMG/BOSS/COLOSSUS/colossus.png"));

					// 2
					CResourceManager::GetInst()->AddAnimationSequence2DFrame(AnimName, Vector2(xIdx + StandardX, yIdx), Vector2(xIdx, yIdx + StandardY));
				}


				// MoveFront
				{
					const std::string& state = "MoveFront";
					char AnimName[40] = "";

					strcat_s(AnimName, Name.c_str());
					strcat_s(AnimName, Name2.c_str());
					strcat_s(AnimName, state.c_str());

					float xIdx = StartX + StandardX * 3;
					float yIdx = 256.f;

					CResourceManager::GetInst()->CreateAnimationSequence2D(AnimName, "ColossusSprite", TEXT("IMG/BOSS/COLOSSUS/colossus.png"));

					// 4
					CResourceManager::GetInst()->AddAnimationSequence2DFrame(AnimName, Vector2(xIdx + StandardX, yIdx), Vector2(xIdx, yIdx + StandardY));
				}


				// MoveBack
				{
					const std::string& state = "MoveBack";
					char AnimName[40] = "";

					strcat_s(AnimName, Name.c_str());
					strcat_s(AnimName, Name2.c_str());
					strcat_s(AnimName, state.c_str());

					float xIdx = StartX + StandardX;
					float yIdx = 256.f;

					CResourceManager::GetInst()->CreateAnimationSequence2D(AnimName, "ColossusSprite", TEXT("IMG/BOSS/COLOSSUS/colossus.png"));

					// 2
					CResourceManager::GetInst()->AddAnimationSequence2DFrame(AnimName, Vector2(xIdx + StandardX, yIdx), Vector2(xIdx, yIdx + StandardY));
				}

				// Guard
				{
					const std::string& state = "Guard";
					char AnimName[40] = "";

					strcat_s(AnimName, Name.c_str());
					strcat_s(AnimName, Name2.c_str());
					strcat_s(AnimName, state.c_str());

					float xIdx = StartX + StandardX * 2;
					float yIdx = 256.f;

					CResourceManager::GetInst()->CreateAnimationSequence2D(AnimName, "ColossusSprite", TEXT("IMG/BOSS/COLOSSUS/colossus.png"));

					// 3
					CResourceManager::GetInst()->AddAnimationSequence2DFrame(AnimName, Vector2(xIdx + StandardX, yIdx), Vector2(xIdx, yIdx + StandardY));
				}

				// Dead
				{
					const std::string& state = "Dead";
					char AnimName[40] = "";

					strcat_s(AnimName, Name.c_str());
					strcat_s(AnimName, Name2.c_str());
					strcat_s(AnimName, state.c_str());

					float xIdx = 400.f;
					float yIdx = 192.f;

					CResourceManager::GetInst()->CreateAnimationSequence2D(AnimName, "ColossusSprite", TEXT("IMG/BOSS/COLOSSUS/colossus.png"));
					CResourceManager::GetInst()->AddAnimationSequence2DFrame(AnimName, Vector2(xIdx + StandardX, yIdx), Vector2(xIdx, yIdx + StandardY));
				}
			}

			{} // VS 에러처리용 코드블럭

			// Colossus_LeftHandShadow
			{
				const std::string& Name2 = "LeftHandShadow_";
				float StartX = 256.f;
				float StartY = 320.f;
				float StandardX = 64.f;
				float StandardY = 32.f;

				// Idle
				{
					const std::string& state = "Idle";
					char AnimName[40] = "";

					strcat_s(AnimName, Name.c_str());
					strcat_s(AnimName, Name2.c_str());
					strcat_s(AnimName, state.c_str());

					float xIdx = StartX;
					float yIdx = StartY;

					CResourceManager::GetInst()->CreateAnimationSequence2D(AnimName, "ColossusSprite", TEXT("IMG/BOSS/COLOSSUS/colossus.png"));

					// 1
					CResourceManager::GetInst()->AddAnimationSequence2DFrame(AnimName, Vector2(xIdx, yIdx), Vector2(xIdx + StandardX, yIdx + StandardY));
				}



				// UpFront
				{
					const std::string& state = "UpFront";
					char AnimName[40] = "";

					strcat_s(AnimName, Name.c_str());
					strcat_s(AnimName, Name2.c_str());
					strcat_s(AnimName, state.c_str());

					float xIdx = StartX + StandardX * 2;
					float yIdx = StartY;

					CResourceManager::GetInst()->CreateAnimationSequence2D(AnimName, "ColossusSprite", TEXT("IMG/BOSS/COLOSSUS/colossus.png"));

					// 3, 4, 2
					CResourceManager::GetInst()->AddAnimationSequence2DFrame(AnimName, Vector2(xIdx, yIdx), Vector2(xIdx + StandardX, yIdx + StandardY));

					xIdx += StandardX;
					CResourceManager::GetInst()->AddAnimationSequence2DFrame(AnimName, Vector2(xIdx, yIdx), Vector2(xIdx + StandardX, yIdx + StandardY));

					xIdx -= StandardX * 2;
					CResourceManager::GetInst()->AddAnimationSequence2DFrame(AnimName, Vector2(xIdx, yIdx), Vector2(xIdx + StandardX, yIdx + StandardY));
				}


				// UpSide
				{
					const std::string& state = "UpSide";
					char AnimName[40] = "";

					strcat_s(AnimName, Name.c_str());
					strcat_s(AnimName, Name2.c_str());
					strcat_s(AnimName, state.c_str());

					float xIdx = StartX;
					float yIdx = StartY;

					CResourceManager::GetInst()->CreateAnimationSequence2D(AnimName, "ColossusSprite", TEXT("IMG/BOSS/COLOSSUS/colossus.png"));

					// 1, 4, 2
					CResourceManager::GetInst()->AddAnimationSequence2DFrame(AnimName, Vector2(xIdx, yIdx), Vector2(xIdx + StandardX, yIdx + StandardY));

					xIdx += StandardX * 3.f;
					CResourceManager::GetInst()->AddAnimationSequence2DFrame(AnimName, Vector2(xIdx, yIdx), Vector2(xIdx + StandardX, yIdx + StandardY));

					xIdx -= StandardX * 2.f;
					CResourceManager::GetInst()->AddAnimationSequence2DFrame(AnimName, Vector2(xIdx, yIdx), Vector2(xIdx + StandardX, yIdx + StandardY));
				}

				// MoveFront
				{
					const std::string& state = "MoveFront";
					char AnimName[40] = "";

					strcat_s(AnimName, Name.c_str());
					strcat_s(AnimName, Name2.c_str());
					strcat_s(AnimName, state.c_str());

					float xIdx = StartX + StandardX;
					float yIdx = StartY + StandardY;

					CResourceManager::GetInst()->CreateAnimationSequence2D(AnimName, "ColossusSprite", TEXT("IMG/BOSS/COLOSSUS/colossus.png"));

					// 2, 3, 1
					CResourceManager::GetInst()->AddAnimationSequence2DFrame(AnimName, Vector2(xIdx, yIdx), Vector2(xIdx + StandardX, yIdx + StandardY));

					xIdx += StandardX * 2;
					CResourceManager::GetInst()->AddAnimationSequence2DFrame(AnimName, Vector2(xIdx, yIdx), Vector2(xIdx + StandardX, yIdx + StandardY));

					xIdx -= StandardX * 3;
					CResourceManager::GetInst()->AddAnimationSequence2DFrame(AnimName, Vector2(xIdx, yIdx), Vector2(xIdx + StandardX, yIdx + StandardY));
				}

				// MoveSide
				{
					const std::string& state = "MoveSide";
					char AnimName[40] = "";

					strcat_s(AnimName, Name.c_str());
					strcat_s(AnimName, Name2.c_str());
					strcat_s(AnimName, state.c_str());

					float xIdx = StartX + StandardX;
					float yIdx = StartY + StandardY;

					CResourceManager::GetInst()->CreateAnimationSequence2D(AnimName, "ColossusSprite", TEXT("IMG/BOSS/COLOSSUS/colossus.png"));

					// 2, 4, 1
					CResourceManager::GetInst()->AddAnimationSequence2DFrame(AnimName, Vector2(xIdx, yIdx), Vector2(xIdx + StandardX, yIdx + StandardY));

					xIdx += StandardX * 2.f;
					CResourceManager::GetInst()->AddAnimationSequence2DFrame(AnimName, Vector2(xIdx, yIdx), Vector2(xIdx + StandardX, yIdx + StandardY));

					xIdx -= StandardX * 3.f;
					CResourceManager::GetInst()->AddAnimationSequence2DFrame(AnimName, Vector2(xIdx, yIdx), Vector2(xIdx + StandardX, yIdx + StandardY));
				}

				// MoveBack
				{
					const std::string& state = "MoveBack";
					char AnimName[40] = "";

					strcat_s(AnimName, Name.c_str());
					strcat_s(AnimName, Name2.c_str());
					strcat_s(AnimName, state.c_str());

					float xIdx = StartX;
					float yIdx = StartY + StandardY;

					CResourceManager::GetInst()->CreateAnimationSequence2D(AnimName, "ColossusSprite", TEXT("IMG/BOSS/COLOSSUS/colossus.png"));

					// 1, 4, 2
					CResourceManager::GetInst()->AddAnimationSequence2DFrame(AnimName, Vector2(xIdx, yIdx), Vector2(xIdx + StandardX, yIdx + StandardY));

					xIdx += StandardX * 3.f;
					CResourceManager::GetInst()->AddAnimationSequence2DFrame(AnimName, Vector2(xIdx, yIdx), Vector2(xIdx + StandardX, yIdx + StandardY));

					xIdx -= StandardX * 2.f;
					CResourceManager::GetInst()->AddAnimationSequence2DFrame(AnimName, Vector2(xIdx, yIdx), Vector2(xIdx + StandardX, yIdx + StandardY));
				}

				// Guard
				{
					const std::string& state = "Guard";
					char AnimName[40] = "";

					strcat_s(AnimName, Name.c_str());
					strcat_s(AnimName, Name2.c_str());
					strcat_s(AnimName, state.c_str());

					float xIdx = StartX + StandardX * 3.f;
					float yIdx = StartY + StandardY;

					CResourceManager::GetInst()->CreateAnimationSequence2D(AnimName, "ColossusSprite", TEXT("IMG/BOSS/COLOSSUS/colossus.png"));

					// 4, 3
					CResourceManager::GetInst()->AddAnimationSequence2DFrame(AnimName, Vector2(xIdx, yIdx), Vector2(xIdx + StandardX, yIdx + StandardY));

					xIdx -= StandardX;
					CResourceManager::GetInst()->AddAnimationSequence2DFrame(AnimName, Vector2(xIdx, yIdx), Vector2(xIdx + StandardX, yIdx + StandardY));
				}

				// Dead
				{
					const std::string& state = "Dead";
					char AnimName[40] = "";

					strcat_s(AnimName, Name.c_str());
					strcat_s(AnimName, Name2.c_str());
					strcat_s(AnimName, state.c_str());

					float xIdx = StartX + StandardX * 2.f;
					float yIdx = StartY + StandardY;

					CResourceManager::GetInst()->CreateAnimationSequence2D(AnimName, "ColossusSprite", TEXT("IMG/BOSS/COLOSSUS/colossus.png"));
					CResourceManager::GetInst()->AddAnimationSequence2DFrame(AnimName, Vector2(xIdx, yIdx), Vector2(xIdx + StandardX, yIdx + StandardY));
				}
			}

			{} // VS 에러처리용 코드블럭

			// Colossus_RightHandShadow
			{
				const std::string& Name2 = "RightHandShadow_";
				float StartX = 256.f;
				float StartY = 320.f;
				float StandardX = 64.f;
				float StandardY = 32.f;

				// Idle
				{
					const std::string& state = "Idle";
					char AnimName[40] = "";

					strcat_s(AnimName, Name.c_str());
					strcat_s(AnimName, Name2.c_str());
					strcat_s(AnimName, state.c_str());

					float xIdx = StartX;
					float yIdx = StartY;

					CResourceManager::GetInst()->CreateAnimationSequence2D(AnimName, "ColossusSprite", TEXT("IMG/BOSS/COLOSSUS/colossus.png"));

					// 1
					CResourceManager::GetInst()->AddAnimationSequence2DFrame(AnimName, Vector2(xIdx + StandardX, yIdx), Vector2(xIdx, yIdx + StandardY));
				}



				// UpFront
				{
					const std::string& state = "UpFront";
					char AnimName[40] = "";

					strcat_s(AnimName, Name.c_str());
					strcat_s(AnimName, Name2.c_str());
					strcat_s(AnimName, state.c_str());

					float xIdx = StartX + StandardX * 2;
					float yIdx = StartY;

					CResourceManager::GetInst()->CreateAnimationSequence2D(AnimName, "ColossusSprite", TEXT("IMG/BOSS/COLOSSUS/colossus.png"));

					// 3, 4, 2
					CResourceManager::GetInst()->AddAnimationSequence2DFrame(AnimName, Vector2(xIdx + StandardX, yIdx), Vector2(xIdx, yIdx + StandardY));

					xIdx += StandardX;
					CResourceManager::GetInst()->AddAnimationSequence2DFrame(AnimName, Vector2(xIdx + StandardX, yIdx), Vector2(xIdx, yIdx + StandardY));

					xIdx -= StandardX * 2;
					CResourceManager::GetInst()->AddAnimationSequence2DFrame(AnimName, Vector2(xIdx + StandardX, yIdx), Vector2(xIdx, yIdx + StandardY));
				}


				// UpSide
				{
					const std::string& state = "UpSide";
					char AnimName[40] = "";

					strcat_s(AnimName, Name.c_str());
					strcat_s(AnimName, Name2.c_str());
					strcat_s(AnimName, state.c_str());

					float xIdx = StartX;
					float yIdx = StartY;

					CResourceManager::GetInst()->CreateAnimationSequence2D(AnimName, "ColossusSprite", TEXT("IMG/BOSS/COLOSSUS/colossus.png"));

					// 1, 4, 2
					CResourceManager::GetInst()->AddAnimationSequence2DFrame(AnimName, Vector2(xIdx + StandardX, yIdx), Vector2(xIdx, yIdx + StandardY));

					xIdx += StandardX * 3.f;
					CResourceManager::GetInst()->AddAnimationSequence2DFrame(AnimName, Vector2(xIdx + StandardX, yIdx), Vector2(xIdx, yIdx + StandardY));

					xIdx -= StandardX * 2.f;
					CResourceManager::GetInst()->AddAnimationSequence2DFrame(AnimName, Vector2(xIdx + StandardX, yIdx), Vector2(xIdx, yIdx + StandardY));
				}

				// MoveFront
				{
					const std::string& state = "MoveFront";
					char AnimName[40] = "";

					strcat_s(AnimName, Name.c_str());
					strcat_s(AnimName, Name2.c_str());
					strcat_s(AnimName, state.c_str());

					float xIdx = StartX + StandardX;
					float yIdx = StartY + StandardY;

					CResourceManager::GetInst()->CreateAnimationSequence2D(AnimName, "ColossusSprite", TEXT("IMG/BOSS/COLOSSUS/colossus.png"));

					// 2, 3, 1
					CResourceManager::GetInst()->AddAnimationSequence2DFrame(AnimName, Vector2(xIdx + StandardX, yIdx), Vector2(xIdx, yIdx + StandardY));

					xIdx += StandardX * 2;
					CResourceManager::GetInst()->AddAnimationSequence2DFrame(AnimName, Vector2(xIdx + StandardX, yIdx), Vector2(xIdx, yIdx + StandardY));

					xIdx -= StandardX * 3;
					CResourceManager::GetInst()->AddAnimationSequence2DFrame(AnimName, Vector2(xIdx + StandardX, yIdx), Vector2(xIdx, yIdx + StandardY));
				}

				// MoveSide
				{
					const std::string& state = "MoveSide";
					char AnimName[40] = "";

					strcat_s(AnimName, Name.c_str());
					strcat_s(AnimName, Name2.c_str());
					strcat_s(AnimName, state.c_str());

					float xIdx = StartX + StandardX;
					float yIdx = StartY + StandardY;

					CResourceManager::GetInst()->CreateAnimationSequence2D(AnimName, "ColossusSprite", TEXT("IMG/BOSS/COLOSSUS/colossus.png"));

					// 2, 4, 1
					CResourceManager::GetInst()->AddAnimationSequence2DFrame(AnimName, Vector2(xIdx + StandardX, yIdx), Vector2(xIdx, yIdx + StandardY));

					xIdx += StandardX * 2.f;
					CResourceManager::GetInst()->AddAnimationSequence2DFrame(AnimName, Vector2(xIdx + StandardX, yIdx), Vector2(xIdx, yIdx + StandardY));

					xIdx -= StandardX * 3.f;
					CResourceManager::GetInst()->AddAnimationSequence2DFrame(AnimName, Vector2(xIdx + StandardX, yIdx), Vector2(xIdx, yIdx + StandardY));
				}

				// MoveBack
				{
					const std::string& state = "MoveBack";
					char AnimName[40] = "";

					strcat_s(AnimName, Name.c_str());
					strcat_s(AnimName, Name2.c_str());
					strcat_s(AnimName, state.c_str());

					float xIdx = StartX;
					float yIdx = StartY + StandardY;

					CResourceManager::GetInst()->CreateAnimationSequence2D(AnimName, "ColossusSprite", TEXT("IMG/BOSS/COLOSSUS/colossus.png"));

					// 1, 4, 2
					CResourceManager::GetInst()->AddAnimationSequence2DFrame(AnimName, Vector2(xIdx + StandardX, yIdx), Vector2(xIdx, yIdx + StandardY));

					xIdx += StandardX * 3.f;
					CResourceManager::GetInst()->AddAnimationSequence2DFrame(AnimName, Vector2(xIdx + StandardX, yIdx), Vector2(xIdx, yIdx + StandardY));

					xIdx -= StandardX * 2.f;
					CResourceManager::GetInst()->AddAnimationSequence2DFrame(AnimName, Vector2(xIdx + StandardX, yIdx), Vector2(xIdx, yIdx + StandardY));
				}

				// Guard
				{
					const std::string& state = "Guard";
					char AnimName[40] = "";

					strcat_s(AnimName, Name.c_str());
					strcat_s(AnimName, Name2.c_str());
					strcat_s(AnimName, state.c_str());

					float xIdx = StartX + StandardX * 3.f;
					float yIdx = StartY + StandardY;

					CResourceManager::GetInst()->CreateAnimationSequence2D(AnimName, "ColossusSprite", TEXT("IMG/BOSS/COLOSSUS/colossus.png"));

					// 4, 3
					CResourceManager::GetInst()->AddAnimationSequence2DFrame(AnimName, Vector2(xIdx + StandardX, yIdx), Vector2(xIdx, yIdx + StandardY));

					xIdx -= StandardX;
					CResourceManager::GetInst()->AddAnimationSequence2DFrame(AnimName, Vector2(xIdx + StandardX, yIdx), Vector2(xIdx, yIdx + StandardY));
				}

				// Dead
				{
					const std::string& state = "Dead";
					char AnimName[40] = "";

					strcat_s(AnimName, Name.c_str());
					strcat_s(AnimName, Name2.c_str());
					strcat_s(AnimName, state.c_str());

					float xIdx = StartX + StandardX * 2.f;
					float yIdx = StartY + StandardY;

					CResourceManager::GetInst()->CreateAnimationSequence2D(AnimName, "ColossusSprite", TEXT("IMG/BOSS/COLOSSUS/colossus.png"));
					CResourceManager::GetInst()->AddAnimationSequence2DFrame(AnimName, Vector2(xIdx + StandardX, yIdx), Vector2(xIdx, yIdx + StandardY));
				}
			}
		}

		{} // VS 에러처리용 코드블럭

		// Slime
		{
			
		}

	}



}

void CDefaultSetting::SetInput()
{
    // Title key
    CInput::GetInst()->AddBindKey("TitleKeyUp", VK_UP);
    CInput::GetInst()->AddBindKey("TitleKeyDown", VK_DOWN);
    CInput::GetInst()->AddBindKey("TitleKeyEnter", 'X');
    CInput::GetInst()->AddBindKey("TitleKeyBack", 'C');

    
    // Player Key
    CInput::GetInst()->AddBindKey("MoveLeft", 'A');
    CInput::GetInst()->AddBindKey("MoveRight", 'D');
    CInput::GetInst()->AddBindKey("MoveUp", 'W');
    CInput::GetInst()->AddBindKey("MoveDown", 'S');

    CInput::GetInst()->AddBindKey("Dash", VK_SPACE);

    CInput::GetInst()->AddBindKey("ArrowAim", VK_LBUTTON);
    CInput::GetInst()->AddBindKey("ArrowShoot", VK_LBUTTON);
    CInput::GetInst()->AddBindKey("ArrowCall", VK_RBUTTON);
    CInput::GetInst()->AddBindKey("ArrowCallStop", VK_RBUTTON);


    // DefaultUI Key
    CInput::GetInst()->AddBindKey("UILeft", 'A');
    CInput::GetInst()->AddBindKey("UIRight", 'D');
    CInput::GetInst()->AddBindKey("UIEnter", 'X');
    CInput::GetInst()->AddBindKey("UIBack", 'C');
    CInput::GetInst()->AddBindKey("UIEsc", VK_ESCAPE);



    // Debug Key
    CInput::GetInst()->AddBindKey("DebugModeF1", VK_F1);
    CInput::GetInst()->AddBindKey("DebugModeF2", VK_F2);
    CInput::GetInst()->AddBindKey("DebugModeF3", VK_F3);
    CInput::GetInst()->AddBindKey("DebugModeF4", VK_F4);
    CInput::GetInst()->AddBindKey("DebugModeF5", VK_F5);


    // ETC
    CInput::GetInst()->AddBindKey("Fire", VK_SPACE);
    CInput::GetInst()->AddBindKey("MoveClick", VK_RBUTTON);
}

void CDefaultSetting::SetCollision()
{
    // Create Channel
    {
        CCollisionManager::GetInst()->CreateChannel("Player", ECollision_Interaction::Collision);

        CCollisionManager::GetInst()->CreateChannel("ArrowHead", ECollision_Interaction::Collision);
        CCollisionManager::GetInst()->CreateChannel("ArrowBody", ECollision_Interaction::Collision);

        CCollisionManager::GetInst()->CreateChannel("Monster", ECollision_Interaction::Collision);
        CCollisionManager::GetInst()->CreateChannel("MonsterWeak", ECollision_Interaction::Collision);
        CCollisionManager::GetInst()->CreateChannel("MonsterAttack_Snow", ECollision_Interaction::Collision);
        CCollisionManager::GetInst()->CreateChannel("MonsterAttack_Icicle", ECollision_Interaction::Collision);

        CCollisionManager::GetInst()->CreateChannel("Portal", ECollision_Interaction::Collision);
        CCollisionManager::GetInst()->CreateChannel("Door", ECollision_Interaction::Collision);

        // ETC
        CCollisionManager::GetInst()->CreateChannel("PlayerAttack", ECollision_Interaction::Collision);
        CCollisionManager::GetInst()->CreateChannel("MonsterAttack", ECollision_Interaction::Collision);
    }


    // Create Profile
    {
        CCollisionManager::GetInst()->CreateProfile("Player", "Player", true);

        CCollisionManager::GetInst()->CreateProfile("ArrowHead", "ArrowHead", true);
        CCollisionManager::GetInst()->CreateProfile("ArrowBody", "ArrowBody", true);

        CCollisionManager::GetInst()->CreateProfile("Monster", "Monster", true);
        CCollisionManager::GetInst()->CreateProfile("MonsterWeak", "MonsterWeak", true);
        CCollisionManager::GetInst()->CreateProfile("MonsterAttack_Snow", "MonsterAttack_Snow", true);
        CCollisionManager::GetInst()->CreateProfile("MonsterAttack_Icicle", "MonsterAttack_Icicle", true);

        CCollisionManager::GetInst()->CreateProfile("Portal", "Portal", true);
        CCollisionManager::GetInst()->CreateProfile("Door", "Door", true);

        // ETC
        CCollisionManager::GetInst()->CreateProfile("PlayerAttack", "PlayerAttack", true);
        CCollisionManager::GetInst()->CreateProfile("MonsterAttack", "MonsterAttack", true);
    }



    // Set Collision Interaction
    {
        // Player
        {
            CCollisionManager::GetInst()->SetCollisionInteraction("Player", "Player", ECollision_Interaction::Ignore);
            CCollisionManager::GetInst()->SetCollisionInteraction("Player", "ArrowHead", ECollision_Interaction::Ignore);
            CCollisionManager::GetInst()->SetCollisionInteraction("Player", "MonsterWeak", ECollision_Interaction::Ignore);

            CCollisionManager::GetInst()->SetCollisionInteraction("ArrowHead", "ArrowHead", ECollision_Interaction::Ignore);
            CCollisionManager::GetInst()->SetCollisionInteraction("ArrowHead", "Player", ECollision_Interaction::Ignore);
            CCollisionManager::GetInst()->SetCollisionInteraction("ArrowHead", "ArrowBody", ECollision_Interaction::Ignore);
            CCollisionManager::GetInst()->SetCollisionInteraction("ArrowHead", "MonsterAttack_Snow", ECollision_Interaction::Ignore);
            CCollisionManager::GetInst()->SetCollisionInteraction("ArrowHead", "Portal", ECollision_Interaction::Ignore);
            CCollisionManager::GetInst()->SetCollisionInteraction("ArrowHead", "Door", ECollision_Interaction::Ignore);

            CCollisionManager::GetInst()->SetCollisionInteraction("ArrowBody", "ArrowBody", ECollision_Interaction::Ignore);
            CCollisionManager::GetInst()->SetCollisionInteraction("ArrowBody", "ArrowHead", ECollision_Interaction::Ignore);
            CCollisionManager::GetInst()->SetCollisionInteraction("ArrowBody", "Monster", ECollision_Interaction::Ignore);
            CCollisionManager::GetInst()->SetCollisionInteraction("ArrowBody", "MonsterWeak", ECollision_Interaction::Ignore);
            CCollisionManager::GetInst()->SetCollisionInteraction("ArrowBody", "MonsterAttack_Snow", ECollision_Interaction::Ignore);
            CCollisionManager::GetInst()->SetCollisionInteraction("ArrowBody", "MonsterAttack_Icicle", ECollision_Interaction::Ignore);
            CCollisionManager::GetInst()->SetCollisionInteraction("ArrowBody", "Portal", ECollision_Interaction::Ignore);
            CCollisionManager::GetInst()->SetCollisionInteraction("ArrowBody", "Door", ECollision_Interaction::Ignore);
        }

        // Monster(= Titan)
        {
            CCollisionManager::GetInst()->SetCollisionInteraction("Monster", "Monster", ECollision_Interaction::Ignore);
            CCollisionManager::GetInst()->SetCollisionInteraction("Monster", "ArrowBody", ECollision_Interaction::Ignore);
            CCollisionManager::GetInst()->SetCollisionInteraction("Monster", "MonsterWeak", ECollision_Interaction::Ignore);
            CCollisionManager::GetInst()->SetCollisionInteraction("Monster", "MonsterAttack_Snow", ECollision_Interaction::Ignore);
            CCollisionManager::GetInst()->SetCollisionInteraction("Monster", "Portal", ECollision_Interaction::Ignore);
            CCollisionManager::GetInst()->SetCollisionInteraction("Monster", "Door", ECollision_Interaction::Ignore);

            CCollisionManager::GetInst()->SetCollisionInteraction("MonsterWeak", "MonsterWeak", ECollision_Interaction::Ignore);
            CCollisionManager::GetInst()->SetCollisionInteraction("MonsterWeak", "Player", ECollision_Interaction::Ignore);
            CCollisionManager::GetInst()->SetCollisionInteraction("MonsterWeak", "ArrowBody", ECollision_Interaction::Ignore);
            CCollisionManager::GetInst()->SetCollisionInteraction("MonsterWeak", "Monster", ECollision_Interaction::Ignore);
            CCollisionManager::GetInst()->SetCollisionInteraction("MonsterWeak", "MonsterAttack_Snow", ECollision_Interaction::Ignore);
            CCollisionManager::GetInst()->SetCollisionInteraction("MonsterWeak", "MonsterAttack_Icicle", ECollision_Interaction::Ignore);
            CCollisionManager::GetInst()->SetCollisionInteraction("MonsterWeak", "Portal", ECollision_Interaction::Ignore);
            CCollisionManager::GetInst()->SetCollisionInteraction("MonsterWeak", "Door", ECollision_Interaction::Ignore);

            // Yeti
            {
                CCollisionManager::GetInst()->SetCollisionInteraction("MonsterAttack_Snow", "MonsterAttack_Snow", ECollision_Interaction::Ignore);
                CCollisionManager::GetInst()->SetCollisionInteraction("MonsterAttack_Snow", "ArrowHead", ECollision_Interaction::Ignore);
                CCollisionManager::GetInst()->SetCollisionInteraction("MonsterAttack_Snow", "ArrowBody", ECollision_Interaction::Ignore);
                CCollisionManager::GetInst()->SetCollisionInteraction("MonsterAttack_Snow", "Monster", ECollision_Interaction::Ignore);
                CCollisionManager::GetInst()->SetCollisionInteraction("MonsterAttack_Snow", "MonsterWeak", ECollision_Interaction::Ignore);
                CCollisionManager::GetInst()->SetCollisionInteraction("MonsterAttack_Snow", "Portal", ECollision_Interaction::Ignore);
                CCollisionManager::GetInst()->SetCollisionInteraction("MonsterAttack_Snow", "Door", ECollision_Interaction::Ignore);

                CCollisionManager::GetInst()->SetCollisionInteraction("MonsterAttack_Icicle", "MonsterAttack_Icicle", ECollision_Interaction::Ignore);
                CCollisionManager::GetInst()->SetCollisionInteraction("MonsterAttack_Icicle", "ArrowBody", ECollision_Interaction::Ignore);
                CCollisionManager::GetInst()->SetCollisionInteraction("MonsterAttack_Icicle", "MonsterWeak", ECollision_Interaction::Ignore);
                CCollisionManager::GetInst()->SetCollisionInteraction("MonsterAttack_Icicle", "Portal", ECollision_Interaction::Ignore);
                CCollisionManager::GetInst()->SetCollisionInteraction("MonsterAttack_Icicle", "Door", ECollision_Interaction::Ignore);
            }

        }

        // Object
        {
            CCollisionManager::GetInst()->SetCollisionInteraction("Portal", "Portal", ECollision_Interaction::Ignore);
            CCollisionManager::GetInst()->SetCollisionInteraction("Portal", "ArrowHead", ECollision_Interaction::Ignore);
            CCollisionManager::GetInst()->SetCollisionInteraction("Portal", "ArrowBody", ECollision_Interaction::Ignore);
            CCollisionManager::GetInst()->SetCollisionInteraction("Portal", "Monster", ECollision_Interaction::Ignore);
            CCollisionManager::GetInst()->SetCollisionInteraction("Portal", "MonsterWeak", ECollision_Interaction::Ignore);
            CCollisionManager::GetInst()->SetCollisionInteraction("Portal", "MonsterAttack_Snow", ECollision_Interaction::Ignore);
            CCollisionManager::GetInst()->SetCollisionInteraction("Portal", "MonsterAttack_Icicle", ECollision_Interaction::Ignore);
            CCollisionManager::GetInst()->SetCollisionInteraction("Portal", "Door", ECollision_Interaction::Ignore);


            CCollisionManager::GetInst()->SetCollisionInteraction("Door", "Door", ECollision_Interaction::Ignore);
            CCollisionManager::GetInst()->SetCollisionInteraction("Door", "ArrowHead", ECollision_Interaction::Ignore);
            CCollisionManager::GetInst()->SetCollisionInteraction("Door", "ArrowBody", ECollision_Interaction::Ignore);
            CCollisionManager::GetInst()->SetCollisionInteraction("Door", "Monster", ECollision_Interaction::Ignore);
            CCollisionManager::GetInst()->SetCollisionInteraction("Door", "MonsterWeak", ECollision_Interaction::Ignore);
            CCollisionManager::GetInst()->SetCollisionInteraction("Door", "MonsterAttack_Snow", ECollision_Interaction::Ignore);
            CCollisionManager::GetInst()->SetCollisionInteraction("Door", "MonsterAttack_Icicle", ECollision_Interaction::Ignore);
            CCollisionManager::GetInst()->SetCollisionInteraction("Door", "Portal", ECollision_Interaction::Ignore);
        }


        // ETC (수업)
        {
            // ETC
            CCollisionManager::GetInst()->SetCollisionInteraction("Player", "PlayerAttack", ECollision_Interaction::Ignore);

            CCollisionManager::GetInst()->SetCollisionInteraction("PlayerAttack", "Player", ECollision_Interaction::Ignore);
            CCollisionManager::GetInst()->SetCollisionInteraction("PlayerAttack", "MonsterAttack", ECollision_Interaction::Ignore);
            CCollisionManager::GetInst()->SetCollisionInteraction("PlayerAttack", "PlayerAttack", ECollision_Interaction::Ignore);

            CCollisionManager::GetInst()->SetCollisionInteraction("Monster", "MonsterAttack", ECollision_Interaction::Ignore);

            CCollisionManager::GetInst()->SetCollisionInteraction("MonsterAttack", "Monster", ECollision_Interaction::Ignore);
            CCollisionManager::GetInst()->SetCollisionInteraction("MonsterAttack", "MonsterAttack", ECollision_Interaction::Ignore);
            CCollisionManager::GetInst()->SetCollisionInteraction("MonsterAttack", "PlayerAttack", ECollision_Interaction::Ignore);
        }
    }


}
