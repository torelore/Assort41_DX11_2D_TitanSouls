#include "Portal.h"

#include "Player.h"

#include "Component/ColliderOBB2D.h"
#include "Engine.h"
#include "Scene/SceneManager.h"
#include "../Scene/StartingRuinSceneInfo.h"
#include "../Scene/YetiSceneInfo.h"
#include "../Scene/KnightSceneInfo.h"
#include "../Scene/SlimeSceneInfo.h"


CPortal::CPortal()
{
	SetTypeID<CPortal>();

	m_ObjectTypeName = "Portal";
	m_DestScene = ESceneList::None;
	m_ToDirection = EDirection::Up;
	m_IsOpen = true;
}

CPortal::CPortal(const CPortal& Obj)
{
	m_Body = (CColliderOBB2D*)FindComponent("PortalBody");
}

CPortal::~CPortal()
{
}

void CPortal::SetCollisionProfileName(const std::string& Name)
{
	m_Body->SetCollisionProfile(Name);
}

void CPortal::SetPortalSize(const Vector2& Size)
{
	m_Body->SetBoxHalfSize(Size.x, Size.y);
}

void CPortal::Start()
{
	CGameObject::Start();
}

bool CPortal::Init()
{
	CGameObject::Init();

	m_Body = CreateComponent<CColliderOBB2D>("PortalBody");
	m_Body->SetCollisionCallback<CPortal>(ECollision_Result::Collision, this, &CPortal::CollisionPortal);
	m_Body->SetPivot(0.5f, 0.5f);
	m_Body->SetCollisionProfile("Portal");

	return true;
}

void CPortal::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CPortal::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

CPortal* CPortal::Clone() const
{
	return new CPortal(*this);
}

void CPortal::Save(FILE* File)
{
	CGameObject::Save(File);
}

void CPortal::Load(FILE* File)
{
	CGameObject::Load(File);
}

void CPortal::CollisionPortal(const CollisionResult& result)
{
	if (!m_IsOpen)
		return;

	if (m_DestScene == ESceneList::None)
		return;


	// 플레이어를 찾아 현재 위치를 포탈의 목표방향에 따라 포탈 내부로 이동하는 것처럼 처리
	CPlayer* Player = (CPlayer*)m_Scene->FindObject("Player");

	float PosY = GetWorldPos().y;

	if (m_ToDirection == EDirection::Up) {
		PosY -= (m_Body->GetBoxHalfSize().y + Player->GetBodySize() / 2.f);
	}
	else if (m_ToDirection == EDirection::Down) {
		PosY += (m_Body->GetBoxHalfSize().y + Player->GetBodySize() / 2.f);
	}

	const Vector3& Pos = Vector3(GetWorldPos().x, PosY, GetWorldPos().z);

	Player->MoveToPortal(Pos, m_ToDirection);

	m_IsOpen = false;
}

void CPortal::CreateNextScene()
{
	// 로딩 Scene을 생성한다.
	CSceneManager::GetInst()->CreateNextScene(true);

	switch (m_DestScene)
	{
	case ESceneList::StartingRuins:
	{
		CSceneManager::GetInst()->CreateSceneInfo<CStartingRuinSceneInfo>(false);

		Vector3 Pos;

		switch (m_StartScene)
		{
		case ESceneList::Yeti:
		{
			CPortal* Portal = (CPortal*)CSceneManager::GetInst()->GetNextScene()->FindObject("YetiPortal");

			if (!Portal)
				return;

			Pos = Portal->GetWorldPos();

			break;
		}
		case ESceneList::SludgeHeart:
		{
			CPortal* Portal = (CPortal*)CSceneManager::GetInst()->GetNextScene()->FindObject("SlimePortal");

			if (!Portal)
				return;

			Pos = Portal->GetWorldPos();

			break;
		}
		case ESceneList::Knight:
		{
			CPortal* Portal = (CPortal*)CSceneManager::GetInst()->GetNextScene()->FindObject("KnightPortal");

			if (!Portal)
				return;

			Pos = Portal->GetWorldPos();

			break;
		}
		}

		CPlayer* NextPlayer = (CPlayer*)CSceneManager::GetInst()->GetNextScene()->FindObject("Player");

		if (!NextPlayer)
			return;

		NextPlayer->SetWorldPosition(Pos.x, Pos.y - 35.f * 2.f);
		NextPlayer->SetDirection(m_ToDirection);

		break;
	}
	case ESceneList::Yeti:
		CSceneManager::GetInst()->CreateSceneInfo<CYetiSceneInfo>(false);
		break;
	case ESceneList::SludgeHeart:
		CSceneManager::GetInst()->CreateSceneInfo<CSlimeSceneInfo>(false);
		break;
	case ESceneList::Knight:
		CSceneManager::GetInst()->CreateSceneInfo<CKnightSceneInfo>(false);
		break;
	}
}
