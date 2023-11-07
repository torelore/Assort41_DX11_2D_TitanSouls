#include "Goliath.h"

#include "GoliathHand.h"
#include "GoliathDoor.h"

#include "Arrow.h"
#include "Player.h"

#include "Animation/Animation2D.h"

#include "Component/ColliderSphere2D.h"
#include "Component/ColliderOBB2D.h"
#include "Component/SpriteComponent.h"
#include "Component/TileMapComponent.h"

#include "Resource/ResourceManager.h"
#include "Scene/Scene.h"


// debug
#include <atlstr.h>

CGoliath::CGoliath()
{
	SetTypeID<CGoliath>();

	m_ObjectTypeName = "Titan_Colossus";

	m_Timer = 0.f;

	m_IsAttacked = false;
	m_IsAttackStart = false;
	m_IsDead = false;
}

CGoliath::CGoliath(const CGoliath& Obj) :
	CGameObject(Obj)
{
	m_HeadSprite = (CSpriteComponent*)FindComponent("Goliath_HeadSprite");
	m_BodySprite = (CSpriteComponent*)FindComponent("Goliath_BodySprite");
	m_BodyPaintSprite = (CSpriteComponent*)FindComponent("Goliath_BodyPaintSprite");
	m_LeftSholderSprite = (CSpriteComponent*)FindComponent("Goliath_LeftSholderSprite");
	m_RightSholderSprite = (CSpriteComponent*)FindComponent("Goliath_RightSholderSprite");

	m_Body = (CColliderOBB2D*)FindComponent("Goliath_Body");
	m_BodyWeakPoint = (CColliderOBB2D*)FindComponent("Goliath_WeakPoint");
}

CGoliath::~CGoliath()
{
}

void CGoliath::Start()
{
	CGameObject::Start();
}

bool CGoliath::Init()
{
	CGameObject::Init();

	m_Body = CreateComponent<CColliderOBB2D>("Goliath_BodyCollider");
	m_BodySprite = CreateComponent<CSpriteComponent>("Goliath_BodySprite");

	m_HeadChild = CreateComponent<CSceneComponent>("Goliath_HeadChild");
	m_HeadSprite = CreateComponent<CSpriteComponent>("Goliath_HeadSprite");

	m_BodyPaintChild = CreateComponent<CSceneComponent>("Goliath_BodyPaintChild");
	m_BodyPaintSprite = CreateComponent<CSpriteComponent>("Goliath_BodyPaintSprite");

	m_LeftSholderChild = CreateComponent<CSceneComponent>("Goliath_LeftSholderChild");
	m_LeftSholderSprite = CreateComponent<CSpriteComponent>("Goliath_LeftSholderSprite");

	m_RightholderChild = CreateComponent<CSceneComponent>("Goliath_RightSholderChild");
	m_RightSholderSprite = CreateComponent<CSpriteComponent>("Goliath_RightSholderSprite");


	m_BodyWeakPoint = CreateComponent<CColliderOBB2D>("Goliath_WeakPointCollider");


	SetRootComponent(m_Body);

	// Scale 변수
	float Scale = 2.f;

	const Vector2& vecHeadScale = Vector2(64.f, 70.f) * Scale;
	const Vector2& vecBodyScale = Vector2(96.f, 70.f) * Scale;
	const Vector2& vecSholderScale = Vector2(42.f, 42.f) * Scale;


	// Body
	m_Body->SetCollisionProfile("Monster");
	m_Body->AddChild(m_BodySprite);

	m_Body->SetCollisionCallback<CGoliath>(ECollision_Result::Collision, this, &CGoliath::CollisionGoliathBody);
	m_Body->SetBoxHalfSize(vecBodyScale/2.f);
	m_Body->SetWorldPositionZ(15.f);
	m_Body->AddChild(m_BodySprite);

	m_BodySprite->SetRelativeScale(vecBodyScale);
	m_BodySprite->SetPivot(0.5f, 0.5f);
	m_BodySprite->SetWorldPositionZ(-20.f);

	// Body Weak
	m_Body->AddChild(m_BodyWeakPoint);
	m_BodyWeakPoint->SetCollisionProfile("MonsterWeak");
	m_BodyWeakPoint->SetRelativePositionY(-55.f);
	m_BodyWeakPoint->SetBoxHalfSize(40.f, 20.f);
	m_BodyWeakPoint->SetCollisionCallback<CGoliath>(ECollision_Result::Collision, this, &CGoliath::CollisionGoliathWeak);


	// Head
	m_Body->AddChild(m_HeadChild);
	m_HeadChild->AddChild(m_HeadSprite);

	m_HeadSprite->SetRelativeScale(vecHeadScale);
	m_HeadSprite->SetPivot(0.5f, 0.5f);
	m_HeadSprite->SetInheritRotZ(true);
	m_HeadSprite->SetRelativePosition(0.f, 85.f);

	CMaterial* Material = m_HeadSprite->GetMaterial(0);
	Material->SetRenderState("DepthDisable");


	// Body Paint
	m_Body->AddChild(m_BodyPaintChild);
	m_BodyPaintChild->AddChild(m_BodyPaintSprite);

	m_BodyPaintSprite->SetRelativeScale(vecBodyScale);
	m_BodyPaintSprite->SetPivot(0.5f, 0.5f);


	// Sholder
	m_Body->AddChild(m_LeftSholderChild);
	m_LeftSholderChild->AddChild(m_LeftSholderSprite);

	m_LeftSholderChild->SetRelativePosition(-45.f, 15.f);

	m_LeftSholderSprite->SetRelativeScale(vecSholderScale);
	m_LeftSholderSprite->SetPivot(0.5f, 0.5f);

	Material = m_LeftSholderSprite->GetMaterial(0);
	Material->SetRenderState("DepthDisable");

	m_Body->AddChild(m_RightholderChild);
	m_RightholderChild->AddChild(m_RightSholderSprite);

	m_RightholderChild->SetRelativePosition(45.f, 15.f);

	m_RightSholderSprite->SetRelativeScale(vecSholderScale);
	m_RightSholderSprite->SetPivot(0.5f, 0.5f);

	Material = m_RightSholderSprite->GetMaterial(0);
	Material->SetRenderState("DepthDisable");



	// Animation
	// CAnimation Setting
	{
		CAnimation2D* Anim = m_HeadSprite->SetAnimation<CAnimation2D>("Colossus_Face");

		Anim->AddAnimation("Colossus_FaceRoar", "Colossus_FaceRoar");
		Anim->SetLoop("Colossus_FaceRoar", false);
		Anim->SetPlayTime("Colossus_FaceRoar", 2.f);
		Anim->SetCurrentEndFunction<CGoliath>("Colossus_FaceRoar", this, &CGoliath::AttackStart);

		Anim->AddAnimation("Colossus_FaceIdle", "Colossus_FaceIdle");
		Anim->SetLoop("Colossus_FaceIdle", true);

		Anim->SetCurrentAnimation("Colossus_FaceIdle");


		Anim = m_BodySprite->SetAnimation<CAnimation2D>("Colossus_Body");

		Anim->AddAnimation("Colossus_Body", "Colossus_Body");
		Anim->SetLoop("Colossus_Body", true);

		Anim->SetCurrentAnimation("Colossus_Body");

		
		Anim = m_BodyPaintSprite->SetAnimation<CAnimation2D>("Colossus_BodyPaint");

		Anim->AddAnimation("Colossus_BodyPaint", "Colossus_BodyPaint");
		Anim->SetLoop("Colossus_BodyPaint", true);

		Anim->SetCurrentAnimation("Colossus_BodyPaint");

		
		Anim = m_LeftSholderSprite->SetAnimation<CAnimation2D>("Colossus_SholderLeft");

		Anim->AddAnimation("Colossus_SholderLeft", "Colossus_SholderLeft");
		Anim->SetLoop("Colossus_SholderLeft", true);

		Anim->SetCurrentAnimation("Colossus_SholderLeft");


		Anim = m_RightSholderSprite->SetAnimation<CAnimation2D>("Colossus_SholderRight");
		Anim->AddAnimation("Colossus_SholderRight", "Colossus_SholderRight");
		Anim->SetLoop("Colossus_SholderRight", true);

		Anim->SetCurrentAnimation("Colossus_SholderRight");
	}


	return true;
}

void CGoliath::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);


	m_Timer += DeltaTime;

	//{
	//	CString strDebug;
	//	strDebug.Format(L"Head : %f \n", m_HeadSprite->GetWorldPos().z);
	//	OutputDebugString(strDebug);

	//	strDebug.Format(L"Body : %f \n", m_BodySprite->GetWorldPos().z);
	//	OutputDebugString(strDebug);

	//	strDebug.Format(L"Body paint : %f \n", m_BodyPaintSprite->GetWorldPos().z);
	//	OutputDebugString(strDebug);

	//	strDebug.Format(L"L Sholder : %f \n", m_LeftSholderSprite->GetWorldPos().z);
	//	OutputDebugString(strDebug);

	//	strDebug.Format(L"R Sholder : %f \n", m_RightSholderSprite->GetWorldPos().z);
	//	OutputDebugString(strDebug);


	//}
	return;


	// 어깨부위 이동
	{


		float x = sin(m_Timer) * 25.f;
		float y = cos(m_Timer) * 25.f;

		m_LeftSholderChild->SetRelativePosition(x, y);
		m_RightholderChild->SetRelativePosition(x, y);
	}


	// 몸 부위 움직임
	{

	}

	// 몸 페인트 투명도 조정
	{
		m_BodyPaintSprite->GetMaterial(0)->SetOpacity(DeltaTime);
	}

	//
	{

	}

}

void CGoliath::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

CGoliath* CGoliath::Clone() const
{
	return new CGoliath(*this);
}

void CGoliath::Save(FILE* File)
{
	CGameObject::Save(File);
}

void CGoliath::Load(FILE* File)
{
	CGameObject::Load(File);
}

void CGoliath::CollisionGoliathBody(const CollisionResult& result)
{
	if (m_IsDead)
		return;

	if (!m_IsAttackStart) {
		m_IsAttackStart = true;
		m_Scene->GetSceneInfo()->TitanStart();
		m_HeadSprite->GetAnimation()->SetCurrentAnimation("Colossus_FaceRoar");

		CPlayer* Player = (CPlayer*)m_Scene->FindObject("Player");
		Player->ShakeCamera(5.f, 1.f);

		CGoliathHand* LeftHand = (CGoliathHand*)m_Scene->FindObject("Colossus_LeftHand");
		LeftHand->GoliathStart();

		CGoliathHand* RightHand = (CGoliathHand*)m_Scene->FindObject("Colossus_RightHand");
		RightHand->GoliathStart();

		CGoliathDoor* Door = (CGoliathDoor*)m_Scene->FindObject("ColossusDoor");
		Door->BossStart();
	}


	CCollider* dest = m_Body->GetColliderDest();

	if (dest == nullptr)
		return;

	const std::string& Name = dest->GetOwner()->GetName();


	// 플레이어와 부딪혔다면 플레이어를 사망처리
	if (strcmp("Player", Name.c_str()) == 0) {
		CPlayer* Player = (CPlayer*)dest->GetOwner();

		Player->SetPlayerDead();
	}
}

void CGoliath::CollisionGoliathWeak(const CollisionResult& result)
{
	if (!m_IsAttackStart)
		return;

	if (m_IsDead)
		return;

	CCollider* dest = m_Body->GetColliderDest();

	if (dest == nullptr)
		return;

	// 화살을 정당한 플레이로 맞았는지 체크
	// 화살은 속도가 0이면 안된다.
	CArrow* Arrow = (CArrow*)m_Scene->FindObject("Arrow");

	Arrow->ArrowStop();

	m_Scene->GetSceneInfo()->TitanEnd();

	m_IsDead = true;

	m_Scene->FindObject("Colossus_LeftHand")->Destroy();
	m_Scene->FindObject("Colossus_RightHand")->Destroy();

	//CGoliathHand* LeftHand = (CGoliathHand*)m_Scene->FindObject("Colossus_LeftHand");
	//LeftHand->GoliathDead();

	//CGoliathHand* RightHand = (CGoliathHand*)m_Scene->FindObject("Colossus_RightHand");
	//RightHand->GoliathDead();
}

void CGoliath::AttackStart()
{
	m_HeadSprite->GetAnimation()->SetCurrentAnimation("Colossus_FaceIdle");



	CGoliathHand* LeftHand = (CGoliathHand*)m_Scene->FindObject("Colossus_LeftHand");
	LeftHand->AttackStart();

	CGoliathHand* RightHand = (CGoliathHand*)m_Scene->FindObject("Colossus_RightHand");
	RightHand->AttackStart();

}

void CGoliath::Dead()
{
	//CSound* Sound = m_Scene->GetResource()->FindSound("Colossus");

	//if (Sound)
	//	if (Sound->GetPlay())
	//		Sound->Stop();


	//m_IsDead = true;

	//CGoliathHand* LeftHand = (CGoliathHand*)m_Scene->FindObject("Colossus_LeftHand");
	//LeftHand->GoliathDead();

	//CGoliathHand* RightHand = (CGoliathHand*)m_Scene->FindObject("Colossus_RightHand");
	//RightHand->GoliathDead();

	CGoliathDoor* Door = (CGoliathDoor*)m_Scene->FindObject("ColossusDoor");
	Door->BossEnd();

}
