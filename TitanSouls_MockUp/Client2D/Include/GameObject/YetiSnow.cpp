#include "YetiSnow.h"

#include "Component/SpriteComponent.h"
#include "Component/ColliderSphere2D.h"
#include "Component/TileMapComponent.h"

#include "Resource/ResourceManager.h"

#include "Scene/Scene.h"

#include "Yeti.h"
#include "YetiIcicle.h"
#include "Player.h"


CYetiSnow::CYetiSnow()
{
	SetTypeID<CYetiSnow>();

	m_ObjectTypeName = "Yeti_Snow";
}

CYetiSnow::CYetiSnow(const CYetiSnow& Obj) :
	CGameObject(Obj)
{
	m_Body = (CColliderSphere2D*)FindComponent("Yeti_SnowBody");
	m_Sprite = (CSpriteComponent*)FindComponent("Yeti_SnowSprite");
}

CYetiSnow::~CYetiSnow()
{
}

void CYetiSnow::Start()
{
	CGameObject::Start();
}

bool CYetiSnow::Init()
{
	CGameObject::Init();

	m_Body = CreateComponent<CColliderSphere2D>("Yeti_SnowBody");
	m_Sprite = CreateComponent<CSpriteComponent>("Yeti_SnowSprite");


	SetRootComponent(m_Body);


	m_Body->AddChild(m_Sprite);


	m_Body->SetCollisionCallback<CYetiSnow>(ECollision_Result::Collision, this, &CYetiSnow::CollisionYetiSnow);
	m_Body->SetRadius(20.f);
	m_Body->SetCollisionProfile("MonsterAttack_Snow");


	m_Sprite->SetTexture("Yeti_Snow", TEXT("IMG/BOSS/YETI/yeti.png"));
	m_Sprite->SetPivot(0.5f, 0.5f);
	m_Sprite->SetRelativeScale(40.f, 40.f);
	m_Sprite->SetInheritRotZ(true);



	CResourceManager::GetInst()->CreateAnimationSequence2D("Yeti_Snow", "Yeti_SnowSprite", TEXT("IMG/BOSS/YETI/yeti.png"));
	CResourceManager::GetInst()->AddAnimationSequence2DFrame("Yeti_Snow", Vector2(32.f, 992.f), Vector2(64.f, 1024.f));

	CAnimation2D* Anim = m_Sprite->SetAnimation<CAnimation2D>("Yeti_Snow");
	Anim->AddAnimation("Yeti_Snow", "Yeti_Snow");
	Anim->SetCurrentAnimation("Yeti_Snow");


	return true;
}

void CYetiSnow::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

	// ������Ʈ�� �ִ� Ÿ�ϸ��� ����(������Ʈ���� �տ� �ִ� ���)���� üũ�Ͽ� basecolor�� ����
	CTileMapComponent* RootTilemapBackground = (CTileMapComponent*)m_Scene->FindObject("TilemapObjLayer_Root_FG")->FindComponent("TilemapCompLayer_Root_FG");
	ETileOption tileOption = RootTilemapBackground->GetTileOption(GetWorldPos());

	if (tileOption == ETileOption::FG)
		m_Sprite->GetMaterial(0)->SetBaseColor(0, 0, 0, 0);
	else
		m_Sprite->GetMaterial(0)->SetBaseColor(1, 1, 1, 1);



	float SnowSpeed = 1000.f;

	AddWorldPosition(GetWorldAxis(AXIS_Y) * SnowSpeed * DeltaTime);



	// ������Ʈ�� ���� ������� üũ�ϱ� ���� Ÿ�ϸ� �˻�
	// ���� ������ �ı�
	RootTilemapBackground = (CTileMapComponent*)m_Scene->FindObject("TilemapObjLayer_Root")->FindComponent("TilemapCompLayer_Root");
	tileOption = RootTilemapBackground->GetTileOption(GetWorldPos());

	if (tileOption == ETileOption::Wall) {
		Destroy();
	}

}

void CYetiSnow::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

CYetiSnow* CYetiSnow::Clone() const
{
	return new CYetiSnow(*this);
}

void CYetiSnow::CollisionYetiSnow(const CollisionResult& result)
{
	CCollider* dest = m_Body->GetColliderDest();

	if (dest == nullptr)
		return;

	const std::string& Name = dest->GetOwner()->GetName();


	// �÷��̾�� �ε����ٸ� �÷��̾ ���ó���ϰ� ������Ʈ �ı�
	if (strcmp("Player", Name.c_str()) == 0) {
		CPlayer* Player = (CPlayer*)dest->GetOwner();

		Player->SetPlayerDead();
		Destroy();
	}

	// ��帧�� �ε����ٸ� ������Ʈ ��ȣ �ı�
	if (strcmp("YetiIcicleObject", Name.c_str()) == 0) {
		Destroy();
	}
}
