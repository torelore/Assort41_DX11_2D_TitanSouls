#include "Yeti.h"

#include "YetiIcicle.h"
#include "YetiSnow.h"

#include "Arrow.h"
#include "Player.h"

#include "Animation/Animation2D.h"

#include "Component/ColliderSphere2D.h"
#include "Component/ColliderOBB2D.h"
#include "Component/SpriteComponent.h"
#include "Component/TileMapComponent.h"

#include "Resource/ResourceManager.h"

#include "Scene/Scene.h"



//debug
#include <atlstr.h>

CYeti::CYeti()
{
	SetTypeID<CYeti>();

	m_ObjectTypeName = "Titan_Yeti";
	m_IsAttacked = false;
	m_timer = 0.f;

	m_RollCount = 0;
	m_ShootCount = 0;

	m_IsAttackStart = false;
	m_IsRollNow = false;
	m_IsDead = false;

	m_Direction = EDirection::Down;

	m_RollAfter1 = false;
	m_RollAfter2 = false;


}

CYeti::CYeti(const CYeti& Obj) :
	CGameObject(Obj)
{
	m_Sprite = (CSpriteComponent*)FindComponent("Yeti_Sprite");
	m_Body = (CColliderSphere2D*)FindComponent("Yeti_Body");
	m_BodyWeakPoint = (CColliderOBB2D*)FindComponent("Yeti_WeakPoint");
}

CYeti::~CYeti()
{
}

void CYeti::Start()
{
	CGameObject::Start();
}

bool CYeti::Init()
{
	CGameObject::Init();

	m_Sprite = CreateComponent<CSpriteComponent>("Yeti_Sprite");
	m_Body = CreateComponent<CColliderSphere2D>("Yeti_Body");
	m_LeftBody = CreateComponent<CColliderOBB2D>("Yeti_LeftBody");
	m_RightBody = CreateComponent<CColliderOBB2D>("Yeti_RightBody");
	m_TopBody = CreateComponent<CColliderOBB2D>("Yeti_TopBody");
	m_BottomBody = CreateComponent<CColliderOBB2D>("Yeti_BottomBody");
	m_BodyWeakPoint = CreateComponent<CColliderOBB2D>("Yeti_WeakPoint");

	SetRootComponent(m_Body);

	m_Body->SetCollisionProfile("Monster");
	m_Body->AddChild(m_Sprite);

	m_Body->SetCollisionCallback<CYeti>(ECollision_Result::Collision, this, &CYeti::CollisionYetiBody);
	m_Body->SetRadius(80.f);
	m_Body->SetRelativePositionY(m_Body->GetRelativePos().y + 40.f);


	m_Body->AddChild(m_BodyWeakPoint);
	m_BodyWeakPoint->SetCollisionProfile("MonsterWeak");
	m_BodyWeakPoint->SetBoxHalfSize(40.f, 20.f);
	m_BodyWeakPoint->SetRelativePositionY(-20.f);
	m_BodyWeakPoint->SetCollisionCallback<CYeti>(ECollision_Result::Collision, this, &CYeti::CollisionYetiWeak);


	m_Body->AddChild(m_LeftBody);
	m_LeftBody->SetCollisionProfile("Monster");
	m_LeftBody->SetBoxHalfSize(2.f, 20.f);
	m_LeftBody->SetRelativePositionX(-45.f);
	m_LeftBody->SetRelativePositionY(-20.f);


	m_Body->AddChild(m_RightBody);
	m_RightBody->SetCollisionProfile("Monster");
	m_RightBody->SetBoxHalfSize(2.f, 20.f);
	m_RightBody->SetRelativePositionX(45.f);
	m_RightBody->SetRelativePositionY(-20.f);

	m_Body->AddChild(m_BottomBody);
	m_BottomBody->SetCollisionProfile("Monster");
	m_BottomBody->SetBoxHalfSize(40.f, 2.f);
	m_BottomBody->SetRelativePositionY(-45.f);

	m_Body->AddChild(m_TopBody);
	m_TopBody->SetCollisionProfile("Monster");
	m_TopBody->SetBoxHalfSize(40.f, 2.f);
	m_TopBody->SetRelativePositionY(5.f);



	m_Sprite->SetRelativeScale(248.f, 248.f);
	m_Sprite->SetPivot(0.5f, 0.5f);
	m_Sprite->SetInheritRotZ(true);


	// CAnimation Setting
	{
		CAnimation2D* Anim = m_Sprite->SetAnimation<CAnimation2D>("YetiAnim");

		// Idle + Rest
		{
			Anim->AddAnimation("Yeti_Idle", "Yeti_Idle");
			Anim->SetLoop("Yeti_Idle", true);


			Anim->AddAnimation("Yeti_IdleUp", "Yeti_RestUp");
			Anim->SetLoop("Yeti_IdleUp", true);


			float RestTime = 2.f;
			Anim->AddAnimation("Yeti_RestRight", "Yeti_RestRight");
			Anim->SetCurrentEndFunction<CYeti>("Yeti_RestRight", this, &CYeti::RestEnd);
			Anim->SetPlayTime("Yeti_RestRight", RestTime);
			Anim->SetLoop("Yeti_RestRight", false);

			Anim->AddAnimation("Yeti_RestUp", "Yeti_RestUp");
			Anim->SetCurrentEndFunction<CYeti>("Yeti_RestUp", this, &CYeti::RestEnd);
			Anim->SetPlayTime("Yeti_RestUp", RestTime);
			Anim->SetLoop("Yeti_RestUp", false);

			Anim->AddAnimation("Yeti_RestDown", "Yeti_RestDown");
			Anim->SetCurrentEndFunction<CYeti>("Yeti_RestDown", this, &CYeti::RestEnd);
			Anim->SetPlayTime("Yeti_RestDown", RestTime);
			Anim->SetLoop("Yeti_RestDown", false);

			Anim->AddAnimation("Yeti_RestRightUp", "Yeti_RestRightUp");
			Anim->SetCurrentEndFunction<CYeti>("Yeti_RestRightUp", this, &CYeti::RestEnd);
			Anim->SetPlayTime("Yeti_RestRightUp", RestTime);
			Anim->SetLoop("Yeti_RestRightUp", false);

			Anim->AddAnimation("Yeti_RestRightDown", "Yeti_RestRightDown");
			Anim->SetCurrentEndFunction<CYeti>("Yeti_RestRightDown", this, &CYeti::RestEnd);
			Anim->SetPlayTime("Yeti_RestRightDown", RestTime);
			Anim->SetLoop("Yeti_RestRightDown", false);

			Anim->AddAnimation("Yeti_RestLeftUp", "Yeti_RestLeftUp");
			Anim->SetCurrentEndFunction<CYeti>("Yeti_RestLeftUp", this, &CYeti::RestEnd);
			Anim->SetPlayTime("Yeti_RestLeftUp", RestTime);
			Anim->SetLoop("Yeti_RestLeftUp", false);

			Anim->AddAnimation("Yeti_RestLeftDown", "Yeti_RestLeftDown");
			Anim->SetCurrentEndFunction<CYeti>("Yeti_RestLeftDown", this, &CYeti::RestEnd);
			Anim->SetPlayTime("Yeti_RestLeftDown", RestTime);
			Anim->SetLoop("Yeti_RestLeftDown", false);

			Anim->AddAnimation("Yeti_RestLeft", "Yeti_RestLeft");
			Anim->SetCurrentEndFunction<CYeti>("Yeti_RestLeft", this, &CYeti::RestEnd);
			Anim->SetPlayTime("Yeti_RestLeft", RestTime);
			Anim->SetLoop("Yeti_RestLeft", false);
		}

		// Roll
		{
			Anim->AddAnimation("Yeti_RollRight", "Yeti_RollRight");
			Anim->SetLoop("Yeti_RollRight", true);
			Anim->SetPlayTime("Yeti_RollRight", 0.3f);
			Anim->SetCurrentEndFunction<CYeti>("Yeti_RollRight", this, &CYeti::DropIcicle);

			Anim->AddAnimation("Yeti_RollLeft", "Yeti_RollLeft");
			Anim->SetLoop("Yeti_RollLeft", true);
			Anim->SetPlayTime("Yeti_RollLeft", 0.3f);
			Anim->SetCurrentEndFunction<CYeti>("Yeti_RollRight", this, &CYeti::DropIcicle);

			Anim->AddAnimation("Yeti_RollUp", "Yeti_RollUp");
			Anim->SetLoop("Yeti_RollUp", true);
			Anim->SetPlayTime("Yeti_RollUp", 0.3f);
			Anim->SetCurrentEndFunction<CYeti>("Yeti_RollUp", this, &CYeti::DropIcicle);

			Anim->AddAnimation("Yeti_RollDown", "Yeti_RollDown");
			Anim->SetLoop("Yeti_RollDown", true);
			Anim->SetPlayTime("Yeti_RollDown", 0.3f);
			Anim->SetCurrentEndFunction<CYeti>("Yeti_RollDown", this, &CYeti::DropIcicle);

			Anim->AddAnimation("Yeti_RollRightUp", "Yeti_RollRightUp");
			Anim->SetLoop("Yeti_RollRightUp", true);
			Anim->SetPlayTime("Yeti_RollRightUp", 0.3f);
			Anim->SetCurrentEndFunction<CYeti>("Yeti_RollRightUp", this, &CYeti::DropIcicle);

			Anim->AddAnimation("Yeti_RollRightDown", "Yeti_RollRightDown");
			Anim->SetLoop("Yeti_RollRightDown", true);
			Anim->SetPlayTime("Yeti_RollRightDown", 0.3f);
			Anim->SetCurrentEndFunction<CYeti>("Yeti_RollRightDown", this, &CYeti::DropIcicle);

			Anim->AddAnimation("Yeti_RollLeftUp", "Yeti_RollLeftUp");
			Anim->SetLoop("Yeti_RollLeftUp", true);
			Anim->SetPlayTime("Yeti_RollLeftUp", 0.3f);
			Anim->SetCurrentEndFunction<CYeti>("Yeti_RollLeftUp", this, &CYeti::DropIcicle);

			Anim->AddAnimation("Yeti_RollLeftDown", "Yeti_RollLeftDown");
			Anim->SetLoop("Yeti_RollLeftDown", true);
			Anim->SetPlayTime("Yeti_RollLeftDown", 0.3f);
			Anim->SetCurrentEndFunction<CYeti>("Yeti_RollLeftDown", this, &CYeti::DropIcicle);
		}

		// Roll After 1, 2
		{
			Anim->AddAnimation("Yeti_RollAfter1", "Yeti_RollRight");
			Anim->SetLoop("Yeti_RollAfter1", false);
			Anim->SetCurrentEndFunction<CYeti>("Yeti_RollAfter1", this, &CYeti::RollAfter2);
			Anim->SetPlayTime("Yeti_RollAfter1", 0.3f);



			float playTime = 0.5f;

			Anim->AddAnimation("Yeti_RollAfter2Right", "Yeti_RollAfterRight");
			Anim->SetLoop("Yeti_RollAfter2Right", false);
			Anim->SetCurrentEndFunction<CYeti>("Yeti_RollAfter2Right", this, &CYeti::RollEnd);
			Anim->SetPlayTime("Yeti_RollAfter2Right", playTime);

			Anim->AddAnimation("Yeti_RollAfter2Left", "Yeti_RollAfterLeft");
			Anim->SetLoop("Yeti_RollAfter2Left", false);
			Anim->SetCurrentEndFunction<CYeti>("Yeti_RollAfter2Left", this, &CYeti::RollEnd);
			Anim->SetPlayTime("Yeti_RollAfter2Left", playTime);

			Anim->AddAnimation("Yeti_RollAfter2Up", "Yeti_RollAfterUp");
			Anim->SetLoop("Yeti_RollAfter2Up", false);
			Anim->SetCurrentEndFunction<CYeti>("Yeti_RollAfter2Up", this, &CYeti::RollEnd);
			Anim->SetPlayTime("Yeti_RollAfter2Up", playTime);

			Anim->AddAnimation("Yeti_RollAfter2Down", "Yeti_RollAfterDown");
			Anim->SetLoop("Yeti_RollAfter2Down", false);
			Anim->SetCurrentEndFunction<CYeti>("Yeti_RollAfter2Down", this, &CYeti::RollEnd);
			Anim->SetPlayTime("Yeti_RollAfter2Down", playTime);

			Anim->AddAnimation("Yeti_RollAfter2RightUp", "Yeti_RollAfterRightUp");
			Anim->SetLoop("Yeti_RollAfter2RightUp", false);
			Anim->SetCurrentEndFunction<CYeti>("Yeti_RollAfter2RightUp", this, &CYeti::RollEnd);
			Anim->SetPlayTime("Yeti_RollAfter2RightUp", playTime);

			Anim->AddAnimation("Yeti_RollAfter2RightDown", "Yeti_RollAfterRightDown");
			Anim->SetLoop("Yeti_RollAfter2RightDown", false);
			Anim->SetCurrentEndFunction<CYeti>("Yeti_RollAfter2RightDown", this, &CYeti::RollEnd);
			Anim->SetPlayTime("Yeti_RollAfter2RightDown", playTime);

			Anim->AddAnimation("Yeti_RollAfter2LeftUp", "Yeti_RollAfterLeftUp");
			Anim->SetLoop("Yeti_RollAfter2LeftUp", false);
			Anim->SetCurrentEndFunction<CYeti>("Yeti_RollAfter2LeftUp", this, &CYeti::RollEnd);
			Anim->SetPlayTime("Yeti_RollAfter2LeftUp", playTime);

			Anim->AddAnimation("Yeti_RollAfter2LeftDown", "Yeti_RollAfterLeftDown");
			Anim->SetLoop("Yeti_RollAfter2LeftDown", false);
			Anim->SetCurrentEndFunction<CYeti>("Yeti_RollAfter2LeftDown", this, &CYeti::RollEnd);
			Anim->SetPlayTime("Yeti_RollAfter2LeftDown", playTime);
		}

		// Shoot
		{
			Anim->AddAnimation("Yeti_ShootLeft", "Yeti_ShootLeft");
			Anim->SetLoop("Yeti_ShootLeft", false);
			Anim->SetCurrentEndFunction<CYeti>("Yeti_ShootLeft", this, &CYeti::ShootEnd);
			Anim->SetPlayTime("Yeti_ShootLeft", 0.5f);

			Anim->AddAnimation("Yeti_ShootRight", "Yeti_ShootRight");
			Anim->SetLoop("Yeti_ShootRight", false);
			Anim->SetCurrentEndFunction<CYeti>("Yeti_ShootRight", this, &CYeti::ShootEnd);
			Anim->SetPlayTime("Yeti_ShootRight", 0.5f);

			Anim->AddAnimation("Yeti_ShootUp", "Yeti_ShootUp");
			Anim->SetLoop("Yeti_ShootUp", false);
			Anim->SetCurrentEndFunction<CYeti>("Yeti_ShootUp", this, &CYeti::ShootEnd);
			Anim->SetPlayTime("Yeti_ShootUp", 0.5f);

			Anim->AddAnimation("Yeti_ShootDown", "Yeti_ShootDown");
			Anim->SetLoop("Yeti_ShootDown", false);
			Anim->SetCurrentEndFunction<CYeti>("Yeti_ShootDown", this, &CYeti::ShootEnd);
			Anim->SetPlayTime("Yeti_ShootDown", 0.5f);

			Anim->AddAnimation("Yeti_ShootLeftUp", "Yeti_ShootLeftUp");
			Anim->SetLoop("Yeti_ShootLeftUp", false);
			Anim->SetCurrentEndFunction<CYeti>("Yeti_ShootLeftUp", this, &CYeti::ShootEnd);
			Anim->SetPlayTime("Yeti_ShootLeftUp", 0.5f);

			Anim->AddAnimation("Yeti_ShootLeftDown", "Yeti_ShootLeftDown");
			Anim->SetLoop("Yeti_ShootLeftDown", false);
			Anim->SetCurrentEndFunction<CYeti>("Yeti_ShootLeftDown", this, &CYeti::ShootEnd);
			Anim->SetPlayTime("Yeti_ShootLeftDown", 0.5f);

			Anim->AddAnimation("Yeti_ShootRightUp", "Yeti_ShootRightUp");
			Anim->SetLoop("Yeti_ShootRightUp", false);
			Anim->SetCurrentEndFunction<CYeti>("Yeti_ShootRightUp", this, &CYeti::ShootEnd);
			Anim->SetPlayTime("Yeti_ShootRightUp", 0.5f);

			Anim->AddAnimation("Yeti_ShootRightDown", "Yeti_ShootRightDown");
			Anim->SetLoop("Yeti_ShootRightDown", false);
			Anim->SetCurrentEndFunction<CYeti>("Yeti_ShootRightDown", this, &CYeti::ShootEnd);
			Anim->SetPlayTime("Yeti_ShootRightDown", 0.5f);

		}

		// Yeti Dead
		{
			Anim->AddAnimation("Yeti_DeadRight", "Yeti_DeadRight");
			Anim->SetPlayTime("Yeti_DeadRight", 1.f);

			Anim->AddAnimation("Yeti_DeadLeft", "Yeti_DeadLeft");
			Anim->SetPlayTime("Yeti_DeadLeft", 1.f);

			Anim->AddAnimation("Yeti_DeadUp", "Yeti_DeadUp");
			Anim->SetPlayTime("Yeti_DeadUp", 1.f);

			Anim->AddAnimation("Yeti_DeadDown", "Yeti_DeadDown");
			Anim->SetPlayTime("Yeti_DeadDown", 1.f);

			Anim->AddAnimation("Yeti_DeadRightUp", "Yeti_DeadRightUp");
			Anim->SetPlayTime("Yeti_DeadRightUp", 1.f);

			Anim->AddAnimation("Yeti_DeadRightDown", "Yeti_DeadRightDown");
			Anim->SetPlayTime("Yeti_DeadRightDown", 1.f);

			Anim->AddAnimation("Yeti_DeadLeftUp", "Yeti_DeadLeftUp");
			Anim->SetPlayTime("Yeti_DeadLeftUp", 1.f);

			Anim->AddAnimation("Yeti_DeadLeftDown", "Yeti_DeadLeftDown");
			Anim->SetPlayTime("Yeti_DeadLeftDown", 1.f);
		}

		Anim->SetCurrentAnimation("Yeti_Idle");
	}

	return true;
}

void CYeti::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);


	// 예티가 죽었다면 업데이트 중지
	if (m_IsDead)
		return;

	// 공격당하기 전까지는 idle 상태로 대기.
	if (!m_IsAttacked)
		return;

	// 공격 당하고 첫 번째 공격을 했는지 체크하기 위한 플래그
	if (!m_IsAttackStart)
		Roll();


	// 예티가 보고 있는 방향에 따른 피격 조절을 위한 바디 컨트롤
	switch (m_Direction)
	{
	case EDirection::Up:
		m_LeftBody->SetEnable(true);
		m_RightBody->SetEnable(true);
		m_TopBody->SetEnable(true);
		m_BottomBody->SetEnable(false);
		break;
	case EDirection::Down:
		m_LeftBody->SetEnable(true);
		m_RightBody->SetEnable(true);
		m_TopBody->SetEnable(false);
		m_BottomBody->SetEnable(true);
		break;
	case EDirection::Left:
		m_LeftBody->SetEnable(true);
		m_RightBody->SetEnable(false);
		m_TopBody->SetEnable(true);
		m_BottomBody->SetEnable(true);
		break;
	case EDirection::Right:
		m_LeftBody->SetEnable(false);
		m_RightBody->SetEnable(true);
		m_TopBody->SetEnable(true);
		m_BottomBody->SetEnable(true);
		break;
	case EDirection::LeftUp:
		m_LeftBody->SetEnable(true);
		m_RightBody->SetEnable(false);
		m_TopBody->SetEnable(true);
		m_BottomBody->SetEnable(false);
		break;
	case EDirection::LeftDown:
		m_LeftBody->SetEnable(true);
		m_RightBody->SetEnable(false);
		m_TopBody->SetEnable(false);
		m_BottomBody->SetEnable(true);
		break;
	case EDirection::RightUp:
		m_LeftBody->SetEnable(false);
		m_RightBody->SetEnable(true);
		m_TopBody->SetEnable(true);
		m_BottomBody->SetEnable(false);
		break;
	case EDirection::RightDown:
		m_LeftBody->SetEnable(false);
		m_RightBody->SetEnable(true);
		m_TopBody->SetEnable(false);
		m_BottomBody->SetEnable(true);
		break;
	}


	float Speed = 700.f;
	float Radius = 100.f;

	
	// Roll 공격 도중 이동을 업데이트
	if (m_IsRollNow) {
		m_Body->SetEnable(true);


		CTileMapComponent* RootTilemapBackground = (CTileMapComponent*)m_Scene->FindObject("TilemapObjLayer_Root")->FindComponent("TilemapCompLayer_Root");

		switch (m_Direction)
		{
		case EDirection::Up:
		{
			const Vector3& tempPos = Vector3(GetWorldPos().x, GetWorldPos().y + Radius, GetWorldPos().z);
			ETileOption tileOption = RootTilemapBackground->GetTileOption(tempPos);

			if (tileOption == ETileOption::Wall) {
				RollAfter1();
				break;
			}

			AddWorldPosition(GetWorldAxis(AXIS_Y) * Speed * DeltaTime);
			break;
		}
		case EDirection::Down:
		{
			const Vector3& tempPos = Vector3(GetWorldPos().x, GetWorldPos().y - Radius, GetWorldPos().z);
			ETileOption tileOption = RootTilemapBackground->GetTileOption(tempPos);

			if (tileOption == ETileOption::Wall) {
				RollAfter1();
				break;
			}

			if (tempPos.y <= 350.f) {
				RollAfter1();
				break;
			}

			AddWorldPosition(GetWorldAxis(AXIS_Y) * -Speed * DeltaTime);
			break;
		}
		case EDirection::Left:
		{
			const Vector3& tempPos = Vector3(GetWorldPos().x - Radius, GetWorldPos().y, GetWorldPos().z);
			ETileOption tileOption = RootTilemapBackground->GetTileOption(tempPos);

			if (tileOption == ETileOption::Wall) {
				RollAfter1();
				break;
			}

			AddWorldPosition(GetWorldAxis(AXIS_X) * -Speed * DeltaTime);
			break;
		}
		case EDirection::Right:
		{
			const Vector3& tempPos = Vector3(GetWorldPos().x + Radius, GetWorldPos().y, GetWorldPos().z);
			ETileOption tileOption = RootTilemapBackground->GetTileOption(tempPos);

			if (tileOption == ETileOption::Wall) {
				RollAfter1();
				break;
			}

			AddWorldPosition(GetWorldAxis(AXIS_X) * Speed * DeltaTime);
			break;
		}
		case EDirection::LeftUp:
		{
			const Vector3& tempPos = Vector3(GetWorldPos().x - Radius, GetWorldPos().y + Radius, GetWorldPos().z);
			ETileOption tileOption = RootTilemapBackground->GetTileOption(tempPos);

			if (tileOption == ETileOption::Wall) {
				RollAfter1();
				break;
			}

			AddWorldPosition(GetWorldAxis(AXIS_X) * -Speed * 0.7f * DeltaTime);
			AddWorldPosition(GetWorldAxis(AXIS_Y) * Speed * 0.7f * DeltaTime);
			break;
		}
		case EDirection::LeftDown:
		{
			const Vector3& tempPos = Vector3(GetWorldPos().x - Radius, GetWorldPos().y - Radius, GetWorldPos().z);
			ETileOption tileOption = RootTilemapBackground->GetTileOption(tempPos);

			if (tileOption == ETileOption::Wall) {
				RollAfter1();
				break;
			}

			if (tempPos.y <= 350.f) {
				RollAfter1();
				break;
			}

			AddWorldPosition(GetWorldAxis(AXIS_X) * -Speed * 0.7f * DeltaTime);
			AddWorldPosition(GetWorldAxis(AXIS_Y) * -Speed * 0.7f * DeltaTime);
			break;
		}
		case EDirection::RightUp:
		{
			const Vector3& tempPos = Vector3(GetWorldPos().x + Radius, GetWorldPos().y + Radius, GetWorldPos().z);
			ETileOption tileOption = RootTilemapBackground->GetTileOption(tempPos);

			if (tileOption == ETileOption::Wall) {
				RollAfter1();
				break;
			}

			AddWorldPosition(GetWorldAxis(AXIS_X) * Speed * 0.7f * DeltaTime);
			AddWorldPosition(GetWorldAxis(AXIS_Y) * Speed * 0.7f * DeltaTime);
			break;
		}
		case EDirection::RightDown:
		{
			const Vector3& tempPos = Vector3(GetWorldPos().x + Radius, GetWorldPos().y - Radius, GetWorldPos().z);
			ETileOption tileOption = RootTilemapBackground->GetTileOption(tempPos);

			if (tileOption == ETileOption::Wall) {
				RollAfter1();
				break;
			}

			if (tempPos.y <= 350.f) {
				RollAfter1();
				break;
			}

			AddWorldPosition(GetWorldAxis(AXIS_X) * Speed * 0.7f * DeltaTime);
			AddWorldPosition(GetWorldAxis(AXIS_Y) * -Speed * 0.7f * DeltaTime);
			break;
		}
		} // Switch End

	}// if IsRollNow End


	// Roll 공격이 플레이어에게 적중하지 않았을 때, 벽까지 이동 후 벽에 닿아 튕겨나오는 이동 업데이트
	float JumpDistY = 300.f;
	float JumpDistX = 150.f;

	if (m_RollAfter1) {
		switch (m_Direction)
		{
		case EDirection::Up:
			AddWorldPosition(GetWorldAxis(AXIS_X) * JumpDistX * DeltaTime);
			AddWorldPosition(GetWorldAxis(AXIS_Y) * JumpDistY * DeltaTime);
			break;
		case EDirection::Down:
			AddWorldPosition(GetWorldAxis(AXIS_X) * JumpDistX * DeltaTime);
			AddWorldPosition(GetWorldAxis(AXIS_Y) * JumpDistY * DeltaTime);
			break;
		case EDirection::Left:
			AddWorldPosition(GetWorldAxis(AXIS_X) * JumpDistX * DeltaTime);
			AddWorldPosition(GetWorldAxis(AXIS_Y) * JumpDistY * DeltaTime);
			break;
		case EDirection::Right:
			AddWorldPosition(GetWorldAxis(AXIS_X) * -JumpDistX * DeltaTime);
			AddWorldPosition(GetWorldAxis(AXIS_Y) * JumpDistY* DeltaTime);
			break;
		case EDirection::LeftUp:
			AddWorldPosition(GetWorldAxis(AXIS_X) * JumpDistX * DeltaTime);
			AddWorldPosition(GetWorldAxis(AXIS_Y) * JumpDistY* DeltaTime);
			break;
		case EDirection::LeftDown:
			AddWorldPosition(GetWorldAxis(AXIS_X) * JumpDistX * DeltaTime);
			AddWorldPosition(GetWorldAxis(AXIS_Y) * JumpDistY* DeltaTime);
			break;
		case EDirection::RightUp:
			AddWorldPosition(GetWorldAxis(AXIS_X) * -JumpDistX * DeltaTime);
			AddWorldPosition(GetWorldAxis(AXIS_Y) * JumpDistY* DeltaTime);
			break;
		case EDirection::RightDown:
			AddWorldPosition(GetWorldAxis(AXIS_X) * -JumpDistX * DeltaTime);
			AddWorldPosition(GetWorldAxis(AXIS_Y) * JumpDistY* DeltaTime);
			break;
		default:
			break;
		}
	}

	if (m_RollAfter2) {
		switch (m_Direction)
		{
		case EDirection::Up:
			AddWorldPosition(GetWorldAxis(AXIS_X) * JumpDistX * g_DeltaTime);
			AddWorldPosition(GetWorldAxis(AXIS_Y) * -JumpDistY * g_DeltaTime);
			break;
		case EDirection::Down:
			AddWorldPosition(GetWorldAxis(AXIS_X) * JumpDistX * g_DeltaTime);
			AddWorldPosition(GetWorldAxis(AXIS_Y) * -JumpDistY * g_DeltaTime);
			break;
		case EDirection::Left:
			AddWorldPosition(GetWorldAxis(AXIS_X) * JumpDistX * g_DeltaTime);
			AddWorldPosition(GetWorldAxis(AXIS_Y) * -JumpDistY * g_DeltaTime);
			break;
		case EDirection::Right:
			AddWorldPosition(GetWorldAxis(AXIS_X) * -JumpDistX * g_DeltaTime);
			AddWorldPosition(GetWorldAxis(AXIS_Y) * -JumpDistY * g_DeltaTime);
			break;
		case EDirection::LeftUp:
			AddWorldPosition(GetWorldAxis(AXIS_X) * JumpDistX * g_DeltaTime);
			AddWorldPosition(GetWorldAxis(AXIS_Y) * -JumpDistY * g_DeltaTime);
			break;
		case EDirection::LeftDown:
			AddWorldPosition(GetWorldAxis(AXIS_X) * JumpDistX * g_DeltaTime);
			AddWorldPosition(GetWorldAxis(AXIS_Y) * -JumpDistY * g_DeltaTime);
			break;
		case EDirection::RightUp:
			AddWorldPosition(GetWorldAxis(AXIS_X) * -JumpDistX * g_DeltaTime);
			AddWorldPosition(GetWorldAxis(AXIS_Y) * -JumpDistY * g_DeltaTime);
			break;
		case EDirection::RightDown:
			AddWorldPosition(GetWorldAxis(AXIS_X) * -JumpDistX * g_DeltaTime);
			AddWorldPosition(GetWorldAxis(AXIS_Y) * -JumpDistY * g_DeltaTime);
			break;
		}
	}


}

void CYeti::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

CYeti* CYeti::Clone() const
{
	return new CYeti(*this);
}

void CYeti::Save(FILE* File)
{
	CGameObject::Save(File);
}

void CYeti::Load(FILE* File)
{
	CGameObject::Load(File);
}

void CYeti::CollisionYetiBody(const CollisionResult& result)
{
	if (m_IsDead)
		return;

	if (!m_IsAttacked) {
		m_IsAttacked = true;
		m_Scene->GetSceneInfo()->TitanStart();
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

	if (strcmp("YetiIcicleObject", Name.c_str()) == 0) {
		CYetiIcicle* ice = (CYetiIcicle*)dest->GetOwner();

		if(ice->IsLand())
			ice->Destroy();
	}
}

void CYeti::CollisionYetiWeak(const CollisionResult& result)
{
	// 화살을 정당한 플레이로 맞았는지 체크
	// 화살은 속도가 0이면 안된다.
	CArrow* Arrow = (CArrow*)m_Scene->FindObject("Arrow");

	if (Arrow == nullptr)
		return;


	// 화살이 움직이지 않고 있다면(가만히 있다면) 피격되지 않는다.
	if (!Arrow->IsMove() || Arrow->IsReflect())
		return;

	Arrow->ArrowStop();

	m_Scene->GetSceneInfo()->TitanEnd();

	m_IsDead = true;
}

void CYeti::Shoot()
{
	m_Body->SetEnable(false);

	m_ShootCount++;

	const Vector3& Pos = m_Scene->FindObject("Player")->GetWorldPos() - GetWorldPos();

	float rot = atan2f(Pos.x, Pos.y) * 180 / PI;

	if (rot < 0)
		rot = 360.f + rot;

	float standard = 45.f;

	if (rot < standard / 2.f || rot > 360.f - standard / 2.f) { // Up
		m_Sprite->GetAnimation()->ChangeAnimation("Yeti_ShootUp");
		m_Direction = EDirection::Up;
	}
	else if (rot < standard + standard / 2.f) { // RightUp
		m_Sprite->GetAnimation()->ChangeAnimation("Yeti_ShootRightUp");
		m_Direction = EDirection::RightUp;
	}
	else if (rot < standard * 2 + standard / 2.f) { // Right
		m_Sprite->GetAnimation()->ChangeAnimation("Yeti_ShootRight");
		m_Direction = EDirection::Right;
	}
	else if (rot < standard * 3 + standard / 2.f) { // RightDown
		m_Sprite->GetAnimation()->ChangeAnimation("Yeti_ShootRightDown");
		m_Direction = EDirection::RightDown;
	}
	else if (rot < standard * 4 + standard / 2.f) { // Down
		m_Sprite->GetAnimation()->ChangeAnimation("Yeti_ShootDown");
		m_Direction = EDirection::Down;
	}
	else if (rot < standard * 5 + standard / 2.f) { // LeftDown
		m_Sprite->GetAnimation()->ChangeAnimation("Yeti_ShootLeftDown");
		m_Direction = EDirection::LeftDown;
	}
	else if (rot < standard * 6 + standard / 2.f) { // Left
		m_Sprite->GetAnimation()->ChangeAnimation("Yeti_ShootLeft");
		m_Direction = EDirection::Left;
	}
	else if (rot < standard * 7 + standard / 2.f) { // LeftUp
		m_Sprite->GetAnimation()->ChangeAnimation("Yeti_ShootLeftUp");
		m_Direction = EDirection::LeftUp;
	}


	CYetiSnow* Snow = m_Scene->CreateObject<CYetiSnow>("YetiSnowObject");
	
	Snow->SetWorldPosition(GetWorldPos());
	Snow->SetWorldRotationZ(-rot);
}

void CYeti::ShootEnd()
{
	const Vector3& Pos = m_Scene->FindObject("Player")->GetWorldPos() - GetWorldPos();

	float rot = atan2f(Pos.x, Pos.y) * 180 / PI;

	if (rot < 0)
		rot = 360.f + rot;

	float standard = 45.f;


	if (rot < standard / 2.f || rot > 360.f - standard / 2.f) { // Up
		m_Sprite->GetAnimation()->ChangeAnimation("Yeti_RestUp");
	}
	else if (rot < standard + standard / 2.f) { // RightUp
		m_Sprite->GetAnimation()->ChangeAnimation("Yeti_RestRightUp");
	}
	else if (rot < standard * 2 + standard / 2.f) { // Right
		m_Sprite->GetAnimation()->ChangeAnimation("Yeti_RestRight");
	}
	else if (rot < standard * 3 + standard / 2.f) { // RightDown
		m_Sprite->GetAnimation()->ChangeAnimation("Yeti_RestRightDown");
	}
	else if (rot < standard * 4 + standard / 2.f) { // Down
		m_Sprite->GetAnimation()->ChangeAnimation("Yeti_RestDown");
	}
	else if (rot < standard * 5 + standard / 2.f) { // LeftDown
		m_Sprite->GetAnimation()->ChangeAnimation("Yeti_RestLeftDown");
	}
	else if (rot < standard * 6 + standard / 2.f) { // Left
		m_Sprite->GetAnimation()->ChangeAnimation("Yeti_RestLeft");
	}
	else if (rot < standard * 7 + standard / 2.f) { // LeftUp
		m_Sprite->GetAnimation()->ChangeAnimation("Yeti_RestLeftUp");
	}




	if (m_ShootCount < 3) {
		Shoot();
	}
	else {
		m_ShootCount = 0;
		Rest();
	}
}

void CYeti::Roll()
{
	if (m_IsDead)
		return;


	m_IsAttackStart = true;

	m_RollCount++;

	const Vector3& Pos = m_Scene->FindObject("Player")->GetWorldPos() - GetWorldPos();

	float rot = atan2f(Pos.x, Pos.y) * 180 / PI;

	if (rot < 0)
		rot = 360.f + rot;

	float standard = 45.f;

	if (rot < standard / 2.f || rot > 360.f - standard / 2.f) { // Up
		m_Sprite->GetAnimation()->ChangeAnimation("Yeti_RollUp");
		m_Direction = EDirection::Up;
	}
	else if (rot < standard + standard / 2.f) { // RightUp
		m_Sprite->GetAnimation()->ChangeAnimation("Yeti_RollRightUp");
		m_Direction = EDirection::RightUp;
	}
	else if (rot < standard * 2 + standard / 2.f) { // Right
		m_Sprite->GetAnimation()->ChangeAnimation("Yeti_RollRight");
		m_Direction = EDirection::Right;
	}
	else if (rot < standard * 3 + standard / 2.f) { // RightDown
		m_Sprite->GetAnimation()->ChangeAnimation("Yeti_RollRightDown");
		m_Direction = EDirection::RightDown;
	}
	else if (rot < standard * 4 + standard / 2.f) { // Down
		m_Sprite->GetAnimation()->ChangeAnimation("Yeti_RollDown");
		m_Direction = EDirection::Down;
	}
	else if (rot < standard * 5 + standard / 2.f) { // LeftDown
		m_Sprite->GetAnimation()->ChangeAnimation("Yeti_RollLeftDown");
		m_Direction = EDirection::LeftDown;
	}
	else if (rot < standard * 6 + standard / 2.f) { // Left
		m_Sprite->GetAnimation()->ChangeAnimation("Yeti_RollLeft");
		m_Direction = EDirection::Left;
	}
	else if (rot < standard * 7 + standard / 2.f) { // LeftUp
		m_Sprite->GetAnimation()->ChangeAnimation("Yeti_RollLeftUp");
		m_Direction = EDirection::LeftUp;
	}

	m_IsRollNow = true;
}

void CYeti::RollEnd()
{
	m_RollAfter2 = false;


	if (m_RollCount < 3) {
		Roll();
	}
	else {
		m_RollCount = 0;
		Shoot();
	}
}

void CYeti::DropIcicle()
{
	m_vecIciclePoint.push_back(GetWorldPos());
}

void CYeti::Rest()
{
	const Vector3& Pos = m_Scene->FindObject("Player")->GetWorldPos() - GetWorldPos();

	float rot = atan2f(Pos.x, Pos.y) * 180 / PI;

	if (rot < 0)
		rot = 360.f + rot;

	float standard = 45.f;

	if (rot < standard / 2.f || rot > 360.f - standard / 2.f) { // Up
		m_Sprite->GetAnimation()->ChangeAnimation("Yeti_RestUp");
	}
	else if (rot < standard + standard / 2.f) { // RightUp
		m_Sprite->GetAnimation()->ChangeAnimation("Yeti_RestRightUp");
	}
	else if (rot < standard * 2 + standard / 2.f) { // Right
		m_Sprite->GetAnimation()->ChangeAnimation("Yeti_RestRight");
	}
	else if (rot < standard * 3 + standard / 2.f) { // RightDown
		m_Sprite->GetAnimation()->ChangeAnimation("Yeti_RestRightDown");
	}
	else if (rot < standard * 4 + standard / 2.f) { // Down
		m_Sprite->GetAnimation()->ChangeAnimation("Yeti_RestDown");
	}
	else if (rot < standard * 5 + standard / 2.f) { // LeftDown
		m_Sprite->GetAnimation()->ChangeAnimation("Yeti_RestLeftDown");
	}
	else if (rot < standard * 6 + standard / 2.f) { // Left
		m_Sprite->GetAnimation()->ChangeAnimation("Yeti_RestLeft");
	}
	else if (rot < standard * 7 + standard / 2.f) { // LeftUp
		m_Sprite->GetAnimation()->ChangeAnimation("Yeti_RestLeftUp");
	}
}

void CYeti::RestEnd()
{
	Roll();
}

void CYeti::Dead()
{
	switch (m_Direction)
	{
	case EDirection::Up:
		m_Sprite->GetAnimation()->ChangeAnimation("Yeti_DeadUp");
		break;
	case EDirection::Down:
		m_Sprite->GetAnimation()->ChangeAnimation("Yeti_DeadDown");
		break;
	case EDirection::Left:
		m_Sprite->GetAnimation()->ChangeAnimation("Yeti_DeadLeft");
		break;
	case EDirection::Right:
		m_Sprite->GetAnimation()->ChangeAnimation("Yeti_DeadRight");
		break;
	case EDirection::LeftUp:
		m_Sprite->GetAnimation()->ChangeAnimation("Yeti_DeadLeftUp");
		break;
	case EDirection::LeftDown:
		m_Sprite->GetAnimation()->ChangeAnimation("Yeti_DeadLeftDown");
		break;
	case EDirection::RightUp:
		m_Sprite->GetAnimation()->ChangeAnimation("Yeti_DeadRightUp");
		break;
	case EDirection::RightDown:
		m_Sprite->GetAnimation()->ChangeAnimation("Yeti_DeadRightDown");
		break;
	}
}

void CYeti::RollAfter1()
{
	// 구르기 종료 플래그 설정
	m_IsRollNow = false;
	m_RollAfter1 = true;
	m_Sprite->GetAnimation()->ChangeAnimation("Yeti_RollAfter1");


	CPlayer* Player = (CPlayer*)m_Scene->FindObject("Player");
	Player->ShakeCamera(2.f, 0.5f);

	
	for (const Vector3& vecPos : m_vecIciclePoint) {
		CYetiIcicle* Icicle = m_Scene->CreateObject<CYetiIcicle>("YetiIcicleObject");

		Icicle->SetDestPos(vecPos);
		Icicle->SetWorldPositionZ(40.f);
	}

	m_vecIciclePoint.clear();
}

void CYeti::RollAfter2()
{
	m_RollAfter1 = false;
	m_RollAfter2 = true;


	// 벽에 부딪히고 난 후, 플레이어 위치에 따른 구르기 종료 애니메이션
	const Vector3& Pos = m_Scene->FindObject("Player")->GetWorldPos() - GetWorldPos();

	float rot = atan2f(Pos.x, Pos.y) * 180 / PI;

	if (rot < 0)
		rot = 360.f + rot;

	float standard = 45.f;

	if (rot < standard / 2.f || rot > 360.f - standard / 2.f) { // Up
		m_Sprite->GetAnimation()->ChangeAnimation("Yeti_RollAfter2Up");
	}
	else if (rot < standard + standard / 2.f) { // RightUp
		m_Sprite->GetAnimation()->ChangeAnimation("Yeti_RollAfter2RightUp");
	}
	else if (rot < standard * 2 + standard / 2.f) { // Right
		m_Sprite->GetAnimation()->ChangeAnimation("Yeti_RollAfter2Right");
	}
	else if (rot < standard * 3 + standard / 2.f) { // RightDown
		m_Sprite->GetAnimation()->ChangeAnimation("Yeti_RollAfter2RightDown");
	}
	else if (rot < standard * 4 + standard / 2.f) { // Down
		m_Sprite->GetAnimation()->ChangeAnimation("Yeti_RollAfter2Down");
	}
	else if (rot < standard * 5 + standard / 2.f) { // LeftDown
		m_Sprite->GetAnimation()->ChangeAnimation("Yeti_RollAfter2LeftDown");
	}
	else if (rot < standard * 6 + standard / 2.f) { // Left
		m_Sprite->GetAnimation()->ChangeAnimation("Yeti_RollAfter2Left");
	}
	else if (rot < standard * 7 + standard / 2.f) { // LeftUp
		m_Sprite->GetAnimation()->ChangeAnimation("Yeti_RollAfter2LeftUp");
	}
}
