#include "YetiIcicle.h"

#include "Component/SpriteComponent.h"
#include "Component/ColliderOBB2D.h"

#include "Resource/ResourceManager.h"

#include "Scene/Scene.h"

#include "Yeti.h"
#include "YetiSnow.h"
#include "Player.h"

CYetiIcicle::CYetiIcicle()
{
	SetTypeID<CYetiIcicle>();

	m_ObjectTypeName = "Yeti_Icicle";
	m_Speed = 500.f;
	m_DestPos = Vector3(0.f, 0.f, 0.f);
	m_IsLand = false;
}

CYetiIcicle::CYetiIcicle(const CYetiIcicle& Obj) :
	CGameObject(Obj)
{
	m_Body = (CColliderOBB2D*)FindComponent("Yeti_IcicleBody");
	m_Sprite = (CSpriteComponent*)FindComponent("Yeti_IcicleSprite");
}

CYetiIcicle::~CYetiIcicle()
{
}

void CYetiIcicle::Start()
{
	CGameObject::Start();
}

bool CYetiIcicle::Init()
{
	CGameObject::Init();

	m_Body = CreateComponent<CColliderOBB2D>("Yeti_IcicleBody");
	m_Sprite = CreateComponent<CSpriteComponent>("Yeti_IcicleSprite");


	SetRootComponent(m_Body);


	m_Body->AddChild(m_Sprite);


	m_Body->SetCollisionCallback<CYetiIcicle>(ECollision_Result::Collision, this, &CYetiIcicle::CollisionYetiIcicle);
	m_Body->SetBoxHalfSize(20.f, 30.f);
	m_Body->SetCollisionProfile("MonsterAttack_Icicle");


	m_Sprite->SetTexture("Yeti_Icicle", TEXT("IMG/BOSS/YETI/yeti.png"));
	m_Sprite->SetPivot(0.5f, 0.5f);
	m_Sprite->SetRelativeScale(40.f, 60.f);
	m_Sprite->SetInheritRotZ(true);


	//Yeti Icicle
	{
		CResourceManager::GetInst()->CreateAnimationSequence2D("Yeti_Icicle", "Yeti_SnowSprite", TEXT("IMG/BOSS/YETI/yeti.png"));
		CResourceManager::GetInst()->AddAnimationSequence2DFrame("Yeti_Icicle", Vector2(134.f, 959.f), Vector2(154.f, 991.f));

		CResourceManager::GetInst()->CreateAnimationSequence2D("Yeti_IcicleEntered", "Yeti_SnowSprite", TEXT("IMG/BOSS/YETI/yeti.png"));
		CResourceManager::GetInst()->AddAnimationSequence2DFrame("Yeti_IcicleEntered", Vector2(294.f, 959.f), Vector2(314.f, 991.f));
	}


	CAnimation2D* Anim = m_Sprite->SetAnimation<CAnimation2D>("Yeti_Icicle");
	Anim->AddAnimation("Yeti_Icicle", "Yeti_Icicle");
	Anim->AddAnimation("Yeti_IcicleEntered", "Yeti_IcicleEntered");
	Anim->SetCurrentAnimation("Yeti_Icicle");

	return true;
}

void CYetiIcicle::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);


	if(m_DestPos.y <= GetWorldPos().y)
		m_Body->AddWorldPosition(m_Body->GetWorldAxis(AXIS_Y) * -m_Speed *DeltaTime);
	else {
		m_Sprite->GetAnimation()->SetCurrentAnimation("Yeti_IcicleEntered");
		m_IsLand = true;
	}
}

void CYetiIcicle::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

CYetiIcicle* CYetiIcicle::Clone() const
{
	return new CYetiIcicle(*this);
}

void CYetiIcicle::CollisionYetiIcicle(const CollisionResult& result)
{
	CCollider* dest = m_Body->GetColliderDest();

	if (dest == nullptr)
		return;

	if (!m_IsLand)
		return;

	const std::string& Name = dest->GetOwner()->GetName();

	// ÇÃ·¹ÀÌ¾î¿Í ºÎµúÇû´Ù¸é ÇÃ·¹ÀÌ¾î¸¦ »ç¸ÁÃ³¸®ÇÏ°í ¿ÀºêÁ§Æ® ÆÄ±«
	if (strcmp("Player", Name.c_str()) == 0) {
		CPlayer* Player = (CPlayer*)dest->GetOwner();

		Player->SetPlayerDead();
		Destroy();
	}
	// ´«°ú ºÎµúÇû´Ù¸é ¿ÀºêÁ§Æ® »óÈ£ ÆÄ±«
	if (strcmp("YetiSnowObject", Name.c_str()) == 0) {

		Destroy();
	}
	// ´«°ú ºÎµúÇû´Ù¸é ¿ÀºêÁ§Æ® ÆÄ±«
	if (strcmp("Arrow", Name.c_str()) == 0) {
		Destroy();
	}
}
