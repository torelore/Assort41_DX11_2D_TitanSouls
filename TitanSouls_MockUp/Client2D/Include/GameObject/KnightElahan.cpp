#include "KnightElahan.h"

#include "Animation/Animation2D.h"

#include "Component/SpriteComponent.h"
#include "Component/ColliderSphere2D.h"
#include "Component/ColliderOBB2D.h"

#include "Resource/ResourceManager.h"

#include "Scene/Scene.h"

#include "Component/TileMapComponent.h"

#include "Arrow.h"
#include "Player.h"

//debug
#include <atlstr.h>

CKnightElahan::CKnightElahan()
{
	SetTypeID<CKnightElahan>();

	m_ObjectTypeName = "Titan_Knight";

	m_timer = 0.f;

	m_IsAttacked = false;
	m_IsAttackStart = false;
	m_IsDead = false;

	m_Direction = EDirection::Up;
}

CKnightElahan::CKnightElahan(const CKnightElahan& Obj) :
	CGameObject(Obj)
{
	m_Sprite = (CSpriteComponent*)FindComponent("Knight_Sprite");
	m_Body = (CColliderOBB2D*)FindComponent("Knight_Body");
}

CKnightElahan::~CKnightElahan()
{
}

void CKnightElahan::Start()
{
	CGameObject::Start();
}

bool CKnightElahan::Init()
{
	CGameObject::Init();

	m_Sprite = CreateComponent<CSpriteComponent>("Knight_Sprite");
	m_Body = CreateComponent<CColliderOBB2D>("Knight_Body");

	SetRootComponent(m_Body);

	m_Body->SetCollisionProfile("Monster");
	m_Body->AddChild(m_Sprite);

	m_Body->SetCollisionCallback<CKnightElahan>(ECollision_Result::Collision, this, &CKnightElahan::CollisionKnightBody);
	m_Body->SetBoxHalfSize(40.f, 40.f);
	m_Body->SetRelativePositionY(m_Body->GetRelativePos().y + 40.f);


	m_Sprite->SetRelativeScale(248.f, 248.f);
	m_Sprite->SetPivot(0.5f, 0.5f);
	m_Sprite->SetInheritRotZ(true);




	// Animation
	std::vector<const char*> vecDir = { "Right", "Up", "Left", "Down", "RightDown", "LeftDown", "LeftUp", "RightUp" };
	const std::string& KnightName = "Knight_";


	// Animation Create
	/*
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
	*/
	
	{} // VS 버그 처리용 코드블럭

	// CAnimation Setting
	{
		CAnimation2D* Anim = m_Sprite->SetAnimation<CAnimation2D>("KnightAnim");


		// Idle
		{
			Anim->AddAnimation("Knight_IdleRight", "Knight_IdleRight");
			Anim->SetLoop("Knight_IdleRight", true);

			Anim->AddAnimation("Knight_IdleUp", "Knight_IdleUp");
			Anim->SetLoop("Knight_IdleUp", true);

			Anim->AddAnimation("Knight_IdleLeft", "Knight_IdleLeft");
			Anim->SetLoop("Knight_IdleLeft", true);

			Anim->AddAnimation("Knight_IdleDown", "Knight_IdleDown");
			Anim->SetLoop("Knight_IdleDown", true);

			Anim->AddAnimation("Knight_IdleRightDown", "Knight_IdleRightDown");
			Anim->SetLoop("Knight_IdleRightDown", true);

			Anim->AddAnimation("Knight_IdleLeftDown", "Knight_IdleLeftDown");
			Anim->SetLoop("Knight_IdleLeftDown", true);

			Anim->AddAnimation("Knight_IdleLeftUp", "Knight_IdleLeftUp");
			Anim->SetLoop("Knight_IdleLeftUp", true);

			Anim->AddAnimation("Knight_IdleRightUp", "Knight_IdleRightUp");
			Anim->SetLoop("Knight_IdleRightUp", true);
		} // Idle End


		// KneelDown
		{
			const std::string& state = "KneelDown";

			for (size_t vecIdx = 0; vecIdx < vecDir.size(); vecIdx++) {
				char AnimName[40] = "";

				strcat_s(AnimName, KnightName.c_str());
				strcat_s(AnimName, state.c_str());
				strcat_s(AnimName, vecDir[vecIdx]);

				Anim->AddAnimation(AnimName, AnimName);
				Anim->SetLoop(AnimName, true);
			}
		} // KneelDown End


		// KneelDown_Skull
		{
			const std::string& state = "KneelDown_Skull";

			for (size_t vecIdx = 0; vecIdx < vecDir.size(); vecIdx++) {
				char AnimName[40] = "";

				strcat_s(AnimName, KnightName.c_str());
				strcat_s(AnimName, state.c_str());
				strcat_s(AnimName, vecDir[vecIdx]);

				Anim->AddAnimation(AnimName, AnimName);
				Anim->SetLoop(AnimName, true);
			}
		} // KneelDown End


		// AimBigArrow 
		{
			const std::string& state = "AimBigArrow";

			char AnimName[40] = "";

			strcat_s(AnimName, KnightName.c_str());
			strcat_s(AnimName, state.c_str());

			Anim->AddAnimation(AnimName, AnimName);
			Anim->SetLoop(AnimName, true);
		} // Aim Big Arrow End


		// AimArrow 
		{
			const std::string& state = "AimArrow";

			for (size_t vecIdx = 0; vecIdx < vecDir.size(); vecIdx++) {
				char AnimName[40] = "";

				strcat_s(AnimName, KnightName.c_str());
				strcat_s(AnimName, state.c_str());
				strcat_s(AnimName, vecDir[vecIdx]);

				Anim->AddAnimation(AnimName, AnimName);
				Anim->SetLoop(AnimName, false);
			}
		} // Aim Arrow End


		// ShootArrow 
		{
			const std::string& state = "ShootArrow";

			for (size_t vecIdx = 0; vecIdx < vecDir.size(); vecIdx++) {
				char AnimName[40] = "";

				strcat_s(AnimName, KnightName.c_str());
				strcat_s(AnimName, state.c_str());
				strcat_s(AnimName, vecDir[vecIdx]);

				Anim->AddAnimation(AnimName, AnimName);
				Anim->SetLoop(AnimName, false);
			}
		} // Shoot Arrow End


		// Move
		{
			const std::string& state = "Move";

			for (size_t vecIdx = 0; vecIdx < vecDir.size(); vecIdx++) {
				char AnimName[40] = "";

				strcat_s(AnimName, KnightName.c_str());
				strcat_s(AnimName, state.c_str());
				strcat_s(AnimName, vecDir[vecIdx]);

				Anim->AddAnimation(AnimName, AnimName);
				Anim->SetLoop(AnimName, true);
			}
		} // Move End


		// Dead_Skull
		{
			const std::string& state = "Dead_Skull";

			for (size_t vecIdx = 0; vecIdx < vecDir.size(); vecIdx++) {
				char AnimName[40] = "";

				strcat_s(AnimName, KnightName.c_str());
				strcat_s(AnimName, state.c_str());
				strcat_s(AnimName, vecDir[vecIdx]);

				Anim->AddAnimation(AnimName, AnimName);
				Anim->SetLoop(AnimName, false);
			}
		} // Dead_Skull End


		Anim->SetCurrentAnimation("Knight_IdleUp");
	}


	return true;
}

void CKnightElahan::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

	m_timer += DeltaTime;

	// 죽었다면 업데이트 중지
	if (m_IsDead)
		return;

	// 공격당하기 전까지는 idle 상태로 대기.
	if (!m_IsAttacked)
		return;

	// 공격 당하고 첫 번째 공격을 했는지 체크하기 위한 플래그
	if (!m_IsAttackStart)
		return;

}

void CKnightElahan::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

CKnightElahan* CKnightElahan::Clone() const
{
	return new CKnightElahan(*this);
}

void CKnightElahan::Save(FILE* File)
{
	CGameObject::Save(File);
}

void CKnightElahan::Load(FILE* File)
{
	CGameObject::Load(File);
}

void CKnightElahan::CollisionKnightBody(const CollisionResult& result)
{
	if (m_IsDead)
		return;

	if (!m_IsAttacked) {
		m_IsAttacked = true;
		m_Scene->GetSceneInfo()->TitanStart();
	}


}
