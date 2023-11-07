#include "GoliathDoor.h"

#include "Animation/Animation2D.h"
#include "Component/ColliderOBB2D.h"
#include "Component/SpriteComponent.h"
#include "Component/TileMapComponent.h"

#include "Scene/Scene.h"

#include "Player.h"

CGoliathDoor::CGoliathDoor()
{
	SetTypeID<CGoliathDoor>();
	m_IsFirstContact = false;
	m_IsBossStart = false;
	m_IsBossEnd = false;
}

CGoliathDoor::CGoliathDoor(const CGoliathDoor& Obj) :
	CGameObject(Obj)
{
	m_IsFirstContact = false;
	m_IsBossStart = false;
	m_IsBossEnd = false;
}

CGoliathDoor::~CGoliathDoor()
{
}

void CGoliathDoor::Start()
{
	CGameObject::Start();
}

bool CGoliathDoor::Init()
{
	CGameObject::Init();

	m_Body = CreateComponent<CColliderOBB2D>("GoliathDoorBody");
	m_Sprite = CreateComponent<CSpriteComponent>("GoliathDoorSprite");

	SetRootComponent(m_Body);

	m_Body->AddChild(m_Sprite);

	m_Body->SetCollisionCallback<CGoliathDoor>(ECollision_Result::Collision, this, &CGoliathDoor::CollisionDoor);
	m_Body->SetPivot(0.5f, 0.5f);
	m_Body->SetCollisionProfile("Door");

	float TileSize = 35.f;
	const Vector2& DoorSize = Vector2(TileSize * 1.5f, TileSize * 1.5f);

	m_Body->SetBoxHalfSize(DoorSize);

	m_Sprite->SetTexture("DoorSprite", TEXT("IMG/player.png"));
	m_Sprite->SetPivot(0.5f, 0.5f);
	m_Sprite->SetRelativeScale(DoorSize * 2.f);


	CResourceManager::GetInst()->CreateAnimationSequence2D("DoorIdle", "DoorIdle", TEXT("IMG/player.png"));
	CResourceManager::GetInst()->AddAnimationSequence2DFrame("DoorIdle", Vector2(0.f, 432.f), Vector2(48.f, 480.f));

	CAnimation2D* Anim = m_Sprite->SetAnimation<CAnimation2D>("DoorAnim");
	Anim->AddAnimation("DoorIdle", "DoorIdle");
	Anim->SetCurrentAnimation("DoorIdle");



	return true;
}

void CGoliathDoor::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
	m_Timer += DeltaTime;


	if (m_IsFirstContact) {
		CSound* Sound = m_Scene->GetResource()->FindSound("DoorMove");

		if (!Sound->GetPlay())
			Sound->Play();

		if (m_Timer >= 1.f) {
			float TileSize = 35.f;
			float pointX = 50.5f * TileSize;
			float mySize = 35.f * 3.f;

			if (pointX - mySize <= GetWorldPos().x) {
#if _DEBUG
				m_Body->AddWorldPositionX(-0.02f);
#else
				m_Body->AddWorldPositionX(-0.005f);
#endif
			}
			else {
				m_IsFirstContact = false;
			}
		}

		return;
	}


	if (m_IsBossStart) {
		float TileSize = 35.f;
		float pointX = 50.5f * TileSize;
		float mySize = 35.f * 3.f;


		if (GetWorldPos().x <= pointX) {
#if _DEBUG
			m_Body->AddWorldPositionX(0.05f);
#else
			m_Body->AddWorldPositionX(0.01f);
#endif
		}
		else {
			m_IsBossStart = false;
		}
	}

	if (m_IsBossEnd) {
		float TileSize = 35.f;
		float pointX = 50.5f * TileSize;
		float mySize = 35.f * 3.f;

		if (pointX - mySize <= GetWorldPos().x) {
#if _DEBUG
			m_Body->AddWorldPositionX(-0.05f);
#else
			m_Body->AddWorldPositionX(-0.01f);
#endif
		}
		else {
			m_IsBossEnd = false;
		}
	}


}

void CGoliathDoor::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

CGoliathDoor* CGoliathDoor::Clone() const
{
	return new CGoliathDoor(*this);
}

void CGoliathDoor::CollisionDoor(const CollisionResult& result)
{
	CCollider* dest = m_Body->GetColliderDest();

	if (dest == nullptr)
		return;


	const std::string& Name = dest->GetOwner()->GetName();

	if (strcmp("Player", Name.c_str()) == 0) {
		if (!m_IsFirstContact) {
			m_IsFirstContact = true;
			m_Timer = 0.f;
		}
	}
}