#include "Sludgeheart.h"

#include "Animation/Animation2D.h"
#include "Component/SpriteComponent.h"
#include "Component/ColliderSphere2D.h"

#include "Resource/ResourceManager.h"

#include "Scene/Scene.h"

CSludgeheart::CSludgeheart()
{
	SetTypeID<CSludgeheart>();

	m_ObjectTypeName = "Titan_Sludgeheart";
}

CSludgeheart::CSludgeheart(const CSludgeheart& Obj) :
	CGameObject(Obj)
{
	m_Sprite = (CSpriteComponent*)FindComponent("Sludgeheart_Sprite");
	m_Body = (CColliderSphere2D*)FindComponent("Sludgeheart_Body");
}

CSludgeheart::~CSludgeheart()
{
}

void CSludgeheart::Start()
{
	CGameObject::Start();
}

bool CSludgeheart::Init()
{
	CGameObject::Init();

	m_Sprite = CreateComponent<CSpriteComponent>("Sludgeheart_Sprite");
	m_Body = CreateComponent<CColliderSphere2D>("Sludgeheart_Body");

	SetRootComponent(m_Body);

	m_Body->SetCollisionProfile("Monster");
	m_Body->AddChild(m_Sprite);
	m_Body->SetWorldPosition(500.f, 500.f);

	m_Body->SetCollisionCallback<CSludgeheart>(ECollision_Result::Collision, this, &CSludgeheart::CollisionSludgeheart);


	m_Sprite->SetRelativeScale(248.f, 248.f);
	m_Sprite->SetPivot(0.5f, 0.5f);
	m_Sprite->SetInheritRotZ(true);


	// Animation Create
	{
		// Idle + Rest Animation
		{
			CResourceManager::GetInst()->CreateAnimationSequence2D("Sludgeheart_Idle", "SludgeheartSprite", TEXT("IMG/BOSS/ACIDNERVE/acidnerve.png"));

			for (int i = 0; i < 3; ++i)
			{
				float standard = 96.f;

				float xIdx = i * standard;
				float yIdx = standard * 0.f;
				CResourceManager::GetInst()->AddAnimationSequence2DFrame("Yeti_IdleRight", Vector2(xIdx, yIdx), Vector2(xIdx + standard, yIdx + standard));
			}


			CResourceManager::GetInst()->CreateAnimationSequence2D("Yeti_IdleLeft", "YetiSprite", TEXT("IMG/BOSS/YETI/yeti.png"));

			for (int i = 0; i < 3; ++i)
			{
				float standard = 96.f;

				float xIdx = i * standard;
				float yIdx = standard * 0.f;
				CResourceManager::GetInst()->AddAnimationSequence2DFrame("Yeti_IdleLeft", Vector2(xIdx + standard, yIdx), Vector2(xIdx, yIdx + standard));
			}


			CResourceManager::GetInst()->CreateAnimationSequence2D("Yeti_IdleUp", "YetiSprite", TEXT("IMG/BOSS/YETI/yeti.png"));

			for (int i = 0; i < 3; ++i)
			{
				float standard = 96.f;

				float xIdx = i * standard;
				float yIdx = standard * 1.f;
				CResourceManager::GetInst()->AddAnimationSequence2DFrame("Yeti_IdleUp", Vector2(xIdx, yIdx), Vector2(xIdx + standard, yIdx + standard));
			}


			CResourceManager::GetInst()->CreateAnimationSequence2D("Yeti_IdleDown", "YetiSprite", TEXT("IMG/BOSS/YETI/yeti.png"));

			for (int i = 0; i < 3; ++i)
			{
				float standard = 96.f;

				float xIdx = i * standard;
				float yIdx = standard * 2.f;
				CResourceManager::GetInst()->AddAnimationSequence2DFrame("Yeti_IdleDown", Vector2(xIdx, yIdx), Vector2(xIdx + standard, yIdx + standard));
			}


			CResourceManager::GetInst()->CreateAnimationSequence2D("Yeti_IdleRightUp", "YetiSprite", TEXT("IMG/BOSS/YETI/yeti.png"));

			for (int i = 0; i < 3; ++i)
			{
				float standard = 96.f;

				float xIdx = i * standard;
				float yIdx = standard * 3.f;
				CResourceManager::GetInst()->AddAnimationSequence2DFrame("Yeti_IdleRightUp", Vector2(xIdx, yIdx), Vector2(xIdx + standard, yIdx + standard));
			}


			CResourceManager::GetInst()->CreateAnimationSequence2D("Yeti_IdleLeftUp", "YetiSprite", TEXT("IMG/BOSS/YETI/yeti.png"));

			for (int i = 0; i < 3; ++i)
			{
				float standard = 96.f;

				float xIdx = i * standard;
				float yIdx = standard * 3.f;
				CResourceManager::GetInst()->AddAnimationSequence2DFrame("Yeti_IdleLeftUp", Vector2(xIdx + standard, yIdx), Vector2(xIdx, yIdx + standard));
			}


			CResourceManager::GetInst()->CreateAnimationSequence2D("Yeti_IdleRightDown", "YetiSprite", TEXT("IMG/BOSS/YETI/yeti.png"));

			for (int i = 0; i < 3; ++i)
			{
				float standard = 96.f;

				float xIdx = i * standard;
				float yIdx = standard * 4.f;
				CResourceManager::GetInst()->AddAnimationSequence2DFrame("Yeti_IdleRightDown", Vector2(xIdx, yIdx), Vector2(xIdx + standard, yIdx + standard));
			}


			CResourceManager::GetInst()->CreateAnimationSequence2D("Yeti_IdleLeftDown", "YetiSprite", TEXT("IMG/BOSS/YETI/yeti.png"));

			for (int i = 0; i < 3; ++i)
			{
				float standard = 96.f;

				float xIdx = i * standard;
				float yIdx = standard * 4.f;
				CResourceManager::GetInst()->AddAnimationSequence2DFrame("Yeti_IdleLeftDown", Vector2(xIdx + standard, yIdx), Vector2(xIdx, yIdx + standard));
			}
		}


		// Roll Animation
		{
			CResourceManager::GetInst()->CreateAnimationSequence2D("RollRight", "YetiSprite", TEXT("IMG/BOSS/YETI/yeti.png"));

			for (int i = 0; i < 3; ++i)
			{
				float standard = 96.f;

				float xIdx = (i + 6) * standard;
				float yIdx = standard * 0.f;
				CResourceManager::GetInst()->AddAnimationSequence2DFrame("RollRight", Vector2(xIdx, yIdx), Vector2(xIdx + standard, yIdx + standard));
			}


			CResourceManager::GetInst()->CreateAnimationSequence2D("RollLeft", "YetiSprite", TEXT("IMG/BOSS/YETI/yeti.png"));

			for (int i = 0; i < 3; ++i)
			{
				float standard = 96.f;

				float xIdx = (i + 6) * standard;
				float yIdx = standard * 0.f;
				CResourceManager::GetInst()->AddAnimationSequence2DFrame("RollLeft", Vector2(xIdx + standard, yIdx), Vector2(xIdx, yIdx + standard));
			}


			CResourceManager::GetInst()->CreateAnimationSequence2D("RollUp", "YetiSprite", TEXT("IMG/BOSS/YETI/yeti.png"));

			for (int i = 0; i < 10; ++i)
			{
				float standard = 96.f;

				float xIdx = (i + 6) * standard;
				float yIdx = standard * 1.f;
				CResourceManager::GetInst()->AddAnimationSequence2DFrame("RollUp", Vector2(xIdx, yIdx), Vector2(xIdx + standard, yIdx + standard));
			}


			CResourceManager::GetInst()->CreateAnimationSequence2D("RollDown", "YetiSprite", TEXT("IMG/BOSS/YETI/yeti.png"));

			for (int i = 0; i < 10; ++i)
			{
				float standard = 96.f;

				float xIdx = (i + 6) * standard;
				float yIdx = standard * 2.f;
				CResourceManager::GetInst()->AddAnimationSequence2DFrame("RollDown", Vector2(xIdx, yIdx), Vector2(xIdx + standard, yIdx + standard));
			}


			CResourceManager::GetInst()->CreateAnimationSequence2D("RollRightUp", "YetiSprite", TEXT("IMG/BOSS/YETI/yeti.png"));

			for (int i = 0; i < 10; ++i)
			{
				float standard = 96.f;

				float xIdx = (i + 6) * standard;
				float yIdx = standard * 3.f;
				CResourceManager::GetInst()->AddAnimationSequence2DFrame("RollRightUp", Vector2(xIdx, yIdx), Vector2(xIdx + standard, yIdx + standard));
			}


			CResourceManager::GetInst()->CreateAnimationSequence2D("RollRightDown", "YetiSprite", TEXT("IMG/BOSS/YETI/yeti.png"));

			for (int i = 0; i < 10; ++i)
			{
				float standard = 96.f;

				float xIdx = (i + 6) * standard;
				float yIdx = standard * 4.f;
				CResourceManager::GetInst()->AddAnimationSequence2DFrame("RollRightDown", Vector2(xIdx, yIdx), Vector2(xIdx + standard, yIdx + standard));
			}

		}


		// Shoot Animation
		{

		}

	}

	// CAnimation Setting
	{
		CAnimation2D* Anim = m_Sprite->SetAnimation<CAnimation2D>("YetiAnim");

		// Idle + Rest
		{
			Anim->AddAnimation("Yeti_IdleRight", "Yeti_IdleRight");
			Anim->SetLoop("Yeti_IdleRight", true);

			Anim->AddAnimation("Yeti_IdleUp", "Yeti_IdleUp");
			Anim->SetLoop("Yeti_IdleUp", true);

			Anim->AddAnimation("Yeti_IdleDown", "Yeti_IdleDown");
			Anim->SetLoop("Yeti_IdleDown", true);

			Anim->AddAnimation("Yeti_IdleRightUp", "Yeti_IdleRightUp");
			Anim->SetLoop("Yeti_IdleRightUp", true);

			Anim->AddAnimation("Yeti_IdleRightDown", "Yeti_IdleRightDown");
			Anim->SetLoop("Yeti_IdleRightDown", true);

			Anim->AddAnimation("Yeti_IdleLeftUp", "Yeti_IdleLeftUp");
			Anim->SetLoop("Yeti_IdleLeftUp", true);

			Anim->AddAnimation("Yeti_IdleLeftDown", "Yeti_IdleLeftDown");
			Anim->SetLoop("Yeti_IdleLeftDown", true);

			Anim->AddAnimation("Yeti_IdleLeft", "Yeti_IdleLeft");
			Anim->SetLoop("Yeti_IdleLeft", true);
		}

		// Roll
		{
			Anim->AddAnimation("RollRight", "RollRight");
			Anim->SetLoop("RollRight", true);
			Anim->SetPlayTime("RollRight", 0.3f);

			Anim->AddAnimation("RollLeft", "RollLeft");
			Anim->SetLoop("RollLeft", true);
			Anim->SetPlayTime("RollLeft", 0.3f);

			Anim->AddAnimation("RollUp", "RollUp");
			Anim->SetLoop("RollUp", true);

			Anim->AddAnimation("RollDown", "RollDown");
			Anim->SetLoop("RollDown", true);

			Anim->AddAnimation("RollRightUp", "RollRightUp");
			Anim->SetLoop("RollRightUp", true);

			Anim->AddAnimation("RollRightDown", "RollRightDown");
			Anim->SetLoop("RollRightDown", true);
		}

		// Shoot
		{

		}


		Anim->SetCurrentAnimation("Yeti_IdleDown");
	}

	return true;
}

void CSludgeheart::Update(float DeltaTime)
{
}

void CSludgeheart::PostUpdate(float DeltaTime)
{
}

CSludgeheart* CSludgeheart::Clone() const
{
    return nullptr;
}

void CSludgeheart::Save(FILE* File)
{
}

void CSludgeheart::Load(FILE* File)
{
}

void CSludgeheart::CollisionSludgeheart(const CollisionResult& result)
{
}
