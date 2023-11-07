#include "GoliathHand.h"

#include "Animation/Animation2D.h"

#include "Component/ColliderOBB2D.h"
#include "Component/SpriteComponent.h"
#include "Component/TileMapComponent.h"

#include "Resource/ResourceManager.h"

#include "Scene/Scene.h"

#include "Goliath.h"
#include "Player.h"
#include "Arrow.h"

CGoliathHand::CGoliathHand()
{
	SetTypeID<CGoliathHand>();

	m_ObjectTypeName = "Colossus_Hand";
	m_Speed = 0.f;
	m_LR = EHandLR::Left;
	m_IsLand = true;
	m_AttackCount = 0;
	m_IsAttack = false;
	m_IsDead = false;

	m_GS = false;
	m_RtM = false;
	m_MtP = false;
	m_MtC = false;
	m_AtP = false;

	m_DestPos = Vector2(0.f, 0.f);
	m_AttackPosY = 0.f;
}

CGoliathHand::CGoliathHand(const CGoliathHand& Obj) :
	CGameObject(Obj)
{
	m_Body = (CColliderOBB2D*)FindComponent("Goliath_HandBody");
	m_Sprite = (CSpriteComponent*)FindComponent("Goliath_HandSprite");

	m_Child = (CSceneComponent*)FindComponent("Goliath_HandChild");
	m_SpriteChild = (CSpriteComponent*)FindComponent("Goliath_HandSpriteChild");
}

CGoliathHand::~CGoliathHand()
{
}

void CGoliathHand::Start()
{
	CGameObject::Start();
}

bool CGoliathHand::Init()
{
	CGameObject::Init();

	m_Body = CreateComponent<CColliderOBB2D>("Goliath_HandBody");
	m_Sprite = CreateComponent<CSpriteComponent>("Goliath_HandSprite");

	m_Child = CreateComponent<CSceneComponent>("Goliath_HandChild");
	m_SpriteChild = CreateComponent<CSpriteComponent>("Goliath_HandSpriteChild");

	SetRootComponent(m_Body);

	const Vector2& HandSize = Vector2(64.f, 64.f);


	// 손 바디 세팅
	m_Body->SetCollisionProfile("Monster");
	m_Body->AddChild(m_Sprite);
	m_Body->AddChild(m_Child);

	m_Body->SetCollisionCallback<CGoliathHand>(ECollision_Result::Collision, this, &CGoliathHand::CollisionHand);
	m_Body->SetBoxHalfSize(HandSize);

	// 손 스프라이트 세팅
	m_Sprite->SetRelativeScale(HandSize * 2.5f);
	m_Sprite->SetPivot(0.5f, 0.5f);

	CMaterial* Material = m_Sprite->GetMaterial(0);

	Material = m_Sprite->GetMaterial(0);
	Material->SetRenderState("DepthDisable");

	// 그림자 오브젝트 세팅
	m_Child->AddChild(m_SpriteChild);
	m_Child->SetRelativePosition(0.f, -30.f);

	// 그림자 투명처리
	m_SpriteChild->SetRelativeScale(HandSize);
	m_SpriteChild->SetPivot(0.5f, 0.5f);

	Material = m_SpriteChild->GetMaterial(0);
	Material->SetOpacity(0.3f);



	return true;
}

void CGoliathHand::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

	if (m_IsDead)
		return;

	if (m_IsLand) {
		m_Body->SetEnable(true);
	}
	else {
		m_Body->SetEnable(false);

	}


	const Vector3& Pos = GetWorldPos();

	// 골리아스 전투 시작 시 움직임 처리
	// Goliath Start
	if (m_GS) {

		float CenterX = 1767.5f;

		if (m_LR == EHandLR::Left) {
			if(Pos.x <= CenterX - 128.f)
				m_Body->AddWorldPosition(m_Body->GetWorldAxis(AXIS_X) * 300.f * DeltaTime);

			if (Pos.y <= 5600.f) 
				m_Body->AddWorldPosition(m_Body->GetWorldAxis(AXIS_Y) * 300.f * DeltaTime);

		}
		else {
			if (Pos.x >= CenterX + 128.f)
				m_Body->AddWorldPosition(m_Body->GetWorldAxis(AXIS_X) * -300.f * DeltaTime);


			if (Pos.y <= 5600.f) 
				m_Body->AddWorldPosition(m_Body->GetWorldAxis(AXIS_Y) * 300.f * DeltaTime);
		}

		return;
	}

	CPlayer* Player = (CPlayer*)m_Scene->FindObject("Player");
	const Vector3& PlayerPos = Player->GetWorldPos();


	// 공격 전 플레이어에게 이동 준비하는 움직임 처리
	// Ready To Move
	if (m_RtM) {
		m_IsLand = false;

		if(Pos.y < m_AttackPosY + 50.f)
			m_Body->AddWorldPosition(m_Body->GetWorldAxis(AXIS_Y) * 500.f * DeltaTime);

		CSound* Sound = (CSound*)m_Scene->GetResource()->FindSound("Colossus_Hit");
		Sound->Stop();

		return;

	}


	CTileMapComponent* RootTilemapBackground = (CTileMapComponent*)m_Scene->FindObject("TilemapObjLayer_Root_FG")->FindComponent("TilemapCompLayer_Root_FG");


	// 플레이어에게 이동하는 움직임 처리
	// MoveToPlayer
	if (m_MtP) {
		m_IsLand = false;

		if (Pos.x <= m_DestPos.x) {
			const Vector3& RightPos = Vector3(GetWorldPos().x, GetWorldPos().y, GetWorldPos().z);
			ETileOption tileOption = RootTilemapBackground->GetTileOption(RightPos);

			if (tileOption != ETileOption::Wall) 
				m_Body->AddWorldPosition(m_Body->GetWorldAxis(AXIS_X) * 500.f * DeltaTime);
		}
		else if(Pos.x > m_DestPos.x) {
			const Vector3& LeftPos = Vector3(GetWorldPos().x, GetWorldPos().y, GetWorldPos().z);
			ETileOption tileOption = RootTilemapBackground->GetTileOption(LeftPos);

			if (tileOption != ETileOption::Wall)
				m_Body->AddWorldPosition(m_Body->GetWorldAxis(AXIS_X) * -500.f * DeltaTime);

		}

		if (Pos.y >= 5300.f) {
			if (Pos.y < m_DestPos.y + 50.f) {
				m_Body->AddWorldPosition(m_Body->GetWorldAxis(AXIS_Y) * 500.f * DeltaTime);
			}
			else if (Pos.y > m_DestPos.y + 50.f) {
				m_Body->AddWorldPosition(m_Body->GetWorldAxis(AXIS_Y) * -500.f * DeltaTime);
			}
		}

		return;
	}

		
	// 플레이어에게 공격하는 움직임 처리
	// AttackToPlayer
	if (m_AtP) {
		if(Pos.y > m_AttackPosY - 50.f)
			m_Body->AddWorldPosition(m_Body->GetWorldAxis(AXIS_Y) * -500.f * DeltaTime);
		else {
			m_IsLand = true;

			CSound* Sound = (CSound*)m_Scene->GetResource()->FindSound("Colossus_Hit");

			if(!Sound->GetPlay())
				Sound->Play();


			CPlayer* Player = (CPlayer*)m_Scene->FindObject("Player");
			Player->ShakeCamera(2.f, 0.5f);
		}

		return;
	}


	// 골리아스에게 돌아가는 움직임 처리
	// MoveToColossus
	if (m_MtC) {
		float CenterX = 1767.5f;

		float xSpeed = 200.f;
		float ySpeed = 200.f;

		if (m_LR == EHandLR::Left) {
			if (Pos.y <= 5600.f)
				m_Body->AddWorldPosition(m_Body->GetWorldAxis(AXIS_Y) * ySpeed * 2.f * DeltaTime);
			else {
				if (Pos.x <= CenterX) {
					m_Body->AddWorldPosition(m_Body->GetWorldAxis(AXIS_X) * xSpeed * DeltaTime);

					if (Pos.x == CenterX) 
						m_MtC = false;
				}
				else {
					m_Body->AddWorldPosition(m_Body->GetWorldAxis(AXIS_X) * -xSpeed * DeltaTime);

					if (Pos.x == CenterX)
						m_MtC = false;
				}
			}
		}
		else {
			if (Pos.y <= 5600.f)
				m_Body->AddWorldPosition(m_Body->GetWorldAxis(AXIS_Y) * ySpeed * 2.f * DeltaTime);
			else {
				if (Pos.x >= CenterX){
					m_Body->AddWorldPosition(m_Body->GetWorldAxis(AXIS_X) * -xSpeed * DeltaTime);

					if (Pos.x == CenterX)
						m_MtC = false;
				}
				else{
					m_Body->AddWorldPosition(m_Body->GetWorldAxis(AXIS_X) * xSpeed * DeltaTime);

					if (Pos.x == CenterX)
						m_MtC = false;
				}

			}
		}
	}


}

void CGoliathHand::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

CGoliathHand* CGoliathHand::Clone() const
{
	return new CGoliathHand(*this);
}

void CGoliathHand::Save(FILE* File)
{
	CGameObject::Save(File);
}

void CGoliathHand::Load(FILE* File)
{
	CGameObject::Load(File);
}

void CGoliathHand::SetHandLeft()
{
	m_LR = EHandLR::Left;

	// Animation Setting
	{
		CAnimation2D* Anim = m_Sprite->SetAnimation<CAnimation2D>("Colossus_LeftHand_Anim");
		float RtMTime = 0.5f;
		float MtPTime = 0.5f;
		float AtPTime = 0.3f;
		float MtCTime = 0.5f;

		{
			Anim->AddAnimation("Colossus_LeftHand_Idle", "Colossus_LeftHand_Idle");
			Anim->SetLoop("Colossus_LeftHand_Idle", false);



			Anim->AddAnimation("Colossus_LeftHand_Up", "Colossus_LeftHand_Up");
			Anim->SetLoop("Colossus_LeftHand_Up", false);
			Anim->SetCurrentEndFunction<CGoliathHand>("Colossus_LeftHand_Up", this, &CGoliathHand::ReadyToMoveEnd);
			Anim->SetPlayTime("Colossus_LeftHand_Up", RtMTime);



			Anim->AddAnimation("Colossus_LeftHand_MoveFront", "Colossus_LeftHand_MoveFront");
			Anim->SetLoop("Colossus_LeftHand_MoveFront", false);
			Anim->SetCurrentEndFunction<CGoliathHand>("Colossus_LeftHand_MoveFront", this, &CGoliathHand::MoveToPlayerEnd);
			Anim->SetPlayTime("Colossus_LeftHand_MoveFront", MtPTime);



			Anim->AddAnimation("Colossus_LeftHand_Attack", "Colossus_LeftHand_Idle");
			Anim->SetLoop("Colossus_LeftHand_Attack", false);
			Anim->SetCurrentEndFunction<CGoliathHand>("Colossus_LeftHand_Attack", this, &CGoliathHand::AttackToPlayerEnd);
			Anim->SetPlayTime("Colossus_LeftHand_Attack", AtPTime);


			Anim->AddAnimation("Colossus_LeftHand_MoveBack", "Colossus_LeftHand_MoveBack");
			Anim->SetLoop("Colossus_LeftHand_MoveBack", false);
			Anim->SetCurrentEndFunction<CGoliathHand>("Colossus_LeftHand_MoveBack", this, &CGoliathHand::MoveToColossusEnd);
			Anim->SetPlayTime("Colossus_LeftHand_MoveBack", MtCTime);




			Anim->AddAnimation("Colossus_LeftHand_Guard", "Colossus_LeftHand_Guard");
			Anim->SetLoop("Colossus_LeftHand_Guard", false);
			Anim->SetCurrentEndFunction<CGoliathHand>("Colossus_LeftHand_Guard", this, &CGoliathHand::Guard);

			Anim->AddAnimation("Colossus_LeftHand_Dead", "Colossus_LeftHand_Dead");
			Anim->SetLoop("Colossus_LeftHand_Dead", false);


			Anim->SetCurrentAnimation("Colossus_LeftHand_Dead");
		}



		Anim = m_SpriteChild->SetAnimation<CAnimation2D>("Colossus_LeftHand_ShadowAnim");

		/*{
			Anim->AddAnimation("Colossus_LeftHandShadow_Idle", "Colossus_LeftHandShadow_Idle");
			Anim->SetLoop("Colossus_LeftHandShadow_Idle", false);


			Anim->AddAnimation("Colossus_LeftHandShadow_UpFront", "Colossus_LeftHandShadow_UpFront");
			Anim->SetLoop("Colossus_LeftHandShadow_UpFront", false);
			Anim->SetCurrentEndFunction<CGoliathHand>("Colossus_LeftHandShadow_UpFront", this, &CGoliathHand::ReadyToMoveEnd);
			Anim->SetPlayTime("Colossus_LeftHandShadow_UpFront", RtMTime);

			Anim->AddAnimation("Colossus_LeftHandShadow_UpSide", "Colossus_LeftHandShadow_UpSide");
			Anim->SetLoop("Colossus_LeftHandShadow_UpSide", false);
			Anim->SetCurrentEndFunction<CGoliathHand>("Colossus_LeftHandShadow_UpSide", this, &CGoliathHand::ReadyToMoveEnd);
			Anim->SetPlayTime("Colossus_LeftHandShadow_UpSide", RtMTime);



			Anim->AddAnimation("Colossus_LeftHandShadow_MoveFront", "Colossus_LeftHandShadow_MoveFront");
			Anim->SetLoop("Colossus_LeftHandShadow_MoveFront", false);
			Anim->SetCurrentEndFunction<CGoliathHand>("Colossus_LeftHandShadow_MoveFront", this, &CGoliathHand::MoveToPlayerEnd);
			Anim->SetPlayTime("Colossus_LeftHandShadow_MoveFront", MtPTime);

			Anim->AddAnimation("Colossus_LeftHandShadow_MoveSide", "Colossus_LeftHandShadow_MoveSide");
			Anim->SetLoop("Colossus_LeftHandShadow_MoveSide", false);
			Anim->SetCurrentEndFunction<CGoliathHand>("Colossus_LeftHandShadow_MoveSide", this, &CGoliathHand::MoveToPlayerEnd);
			Anim->SetPlayTime("Colossus_LeftHandShadow_MoveSide", MtPTime);



			Anim->AddAnimation("Colossus_LeftHandShadow_Attack", "Colossus_LeftHandShadow_Idle");
			Anim->SetLoop("Colossus_LeftHandShadow_Attack", false);
			Anim->SetCurrentEndFunction<CGoliathHand>("Colossus_LeftHandShadow_Attack", this, &CGoliathHand::AttackToPlayerEnd);
			Anim->SetPlayTime("Colossus_LeftHandShadow_Attack", AtPTime);


			Anim->AddAnimation("Colossus_LeftHandShadow_MoveBack", "Colossus_LeftHandShadow_MoveBack");
			Anim->SetLoop("Colossus_LeftHandShadow_MoveBack", false);
			Anim->SetCurrentEndFunction<CGoliathHand>("Colossus_LeftHandShadow_MoveBack", this, &CGoliathHand::MoveToColossusEnd);
			Anim->SetPlayTime("Colossus_LeftHandShadow_MoveBack", MtCTime);




			Anim->AddAnimation("Colossus_LeftHandShadow_Guard", "Colossus_LeftHandShadow_Guard");
			Anim->SetLoop("Colossus_LeftHandShadow_Guard", false);
			Anim->SetCurrentEndFunction<CGoliathHand>("Colossus_LeftHandShadow_Guard", this, &CGoliathHand::Guard);

			Anim->AddAnimation("Colossus_LeftHandShadow_Dead", "Colossus_LeftHandShadow_Dead");
			Anim->SetLoop("Colossus_LeftHandShadow_Dead", false);


			Anim->SetCurrentAnimation("Colossus_LeftHandShadow_Dead");
		}*/
	}


}

void CGoliathHand::SetHandRight()
{
	m_LR = EHandLR::Right;

	// Animation Setting
	{
		CAnimation2D* Anim = m_Sprite->SetAnimation<CAnimation2D>("Colossus_RightHand_Anim");
		float RtMTime = 0.5f;
		float MtPTime = 0.5f;
		float AtPTime = 0.3f;
		float MtCTime = 0.5f;

		{
			Anim->AddAnimation("Colossus_RightHand_Idle", "Colossus_RightHand_Idle");
			Anim->SetLoop("Colossus_RightHand_Idle", false);



			Anim->AddAnimation("Colossus_RightHand_Up", "Colossus_RightHand_Up");
			Anim->SetLoop("Colossus_RightHand_Up", false);
			Anim->SetCurrentEndFunction<CGoliathHand>("Colossus_RightHand_Up", this, &CGoliathHand::ReadyToMoveEnd);
			Anim->SetPlayTime("Colossus_RightHand_Up", RtMTime);



			Anim->AddAnimation("Colossus_RightHand_MoveFront", "Colossus_RightHand_MoveFront");
			Anim->SetLoop("Colossus_RightHand_MoveFront", false);
			Anim->SetCurrentEndFunction<CGoliathHand>("Colossus_RightHand_MoveFront", this, &CGoliathHand::MoveToPlayerEnd);
			Anim->SetPlayTime("Colossus_RightHand_MoveFront", MtPTime);



			Anim->AddAnimation("Colossus_RightHand_Attack", "Colossus_RightHand_Idle");
			Anim->SetLoop("Colossus_RightHand_Attack", false);
			Anim->SetCurrentEndFunction<CGoliathHand>("Colossus_RightHand_Attack", this, &CGoliathHand::AttackToPlayerEnd);
			Anim->SetPlayTime("Colossus_RightHand_Attack", AtPTime);


			Anim->AddAnimation("Colossus_RightHand_MoveBack", "Colossus_RightHand_MoveBack");
			Anim->SetLoop("Colossus_RightHand_MoveBack", false);
			Anim->SetCurrentEndFunction<CGoliathHand>("Colossus_RightHand_MoveBack", this, &CGoliathHand::MoveToColossusEnd);
			Anim->SetPlayTime("Colossus_RightHand_MoveBack", MtCTime);




			Anim->AddAnimation("Colossus_RightHand_Guard", "Colossus_RightHand_Guard");
			Anim->SetLoop("Colossus_RightHand_Guard", false);
			Anim->SetCurrentEndFunction<CGoliathHand>("Colossus_RightHand_Guard", this, &CGoliathHand::Guard);

			Anim->AddAnimation("Colossus_RightHand_Dead", "Colossus_RightHand_Dead");
			Anim->SetLoop("Colossus_RightHand_Dead", false);


			Anim->SetCurrentAnimation("Colossus_RightHand_Dead");
		}



		Anim = m_SpriteChild->SetAnimation<CAnimation2D>("Colossus_RightHand_ShadowAnim");

		/*{
			Anim->AddAnimation("Colossus_RightHandShadow_Idle", "Colossus_RightHandShadow_Idle");
			Anim->SetLoop("Colossus_RightHandShadow_Idle", false);



			Anim->AddAnimation("Colossus_RightHandShadow_UpFront", "Colossus_RightHandShadow_UpFront");
			Anim->SetLoop("Colossus_RightHandShadow_UpFront", false);
			Anim->SetCurrentEndFunction<CGoliathHand>("Colossus_RightHandShadow_UpFront", this, &CGoliathHand::ReadyToMoveEnd);
			Anim->SetPlayTime("Colossus_RightHandShadow_UpFront", RtMTime);

			Anim->AddAnimation("Colossus_RightHandShadow_UpSide", "Colossus_RightHandShadow_UpSide");
			Anim->SetLoop("Colossus_RightHandShadow_UpSide", false);
			Anim->SetCurrentEndFunction<CGoliathHand>("Colossus_RightHandShadow_UpSide", this, &CGoliathHand::ReadyToMoveEnd);
			Anim->SetPlayTime("Colossus_RightHandShadow_UpSide", RtMTime);



			Anim->AddAnimation("Colossus_RightHandShadow_MoveFront", "Colossus_RightHandShadow_MoveFront");
			Anim->SetLoop("Colossus_RightHandShadow_MoveFront", false);
			Anim->SetCurrentEndFunction<CGoliathHand>("Colossus_RightHandShadow_MoveFront", this, &CGoliathHand::MoveToPlayerEnd);
			Anim->SetPlayTime("Colossus_RightHandShadow_MoveFront", MtPTime);

			Anim->AddAnimation("Colossus_RightHandShadow_MoveSide", "Colossus_RightHandShadow_MoveSide");
			Anim->SetLoop("Colossus_RightHandShadow_MoveSide", false);
			Anim->SetCurrentEndFunction<CGoliathHand>("Colossus_RightHandShadow_MoveSide", this, &CGoliathHand::MoveToPlayerEnd);
			Anim->SetPlayTime("Colossus_RightHandShadow_MoveSide", MtPTime);


			Anim->AddAnimation("Colossus_RightHandShadow_Attack", "Colossus_RightHandShadow_Idle");
			Anim->SetLoop("Colossus_RightHandShadow_Attack", false);
			Anim->SetCurrentEndFunction<CGoliathHand>("Colossus_RightHandShadow_Attack", this, &CGoliathHand::AttackToPlayerEnd);
			Anim->SetPlayTime("Colossus_RightHandShadow_Attack", AtPTime);


			Anim->AddAnimation("Colossus_RightHandShadow_MoveBack", "Colossus_RightHandShadow_MoveBack");
			Anim->SetLoop("Colossus_RightHandShadow_MoveBack", false);
			Anim->SetCurrentEndFunction<CGoliathHand>("Colossus_RightHandShadow_MoveBack", this, &CGoliathHand::MoveToColossusEnd);
			Anim->SetPlayTime("Colossus_RightHandShadow_MoveBack", MtCTime);




			Anim->AddAnimation("Colossus_RightHandShadow_Guard", "Colossus_RightHandShadow_Guard");
			Anim->SetLoop("Colossus_RightHandShadow_Guard", false);
			Anim->SetCurrentEndFunction<CGoliathHand>("Colossus_RightHandShadow_Guard", this, &CGoliathHand::Guard);

			Anim->AddAnimation("Colossus_RightHandShadow_Dead", "Colossus_RightHandShadow_Dead");
			Anim->SetLoop("Colossus_RightHandShadow_Dead", false);


			Anim->SetCurrentAnimation("Colossus_RightHandShadow_Dead");
		}*/
	}


}

void CGoliathHand::CollisionHand(const CollisionResult& result)
{
	if (m_IsDead)
		return;

	if (!m_IsLand)
		return;



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

void CGoliathHand::GoliathStart()
{
	// 전투 시작 애니메이션
	m_GS = true;

	// 애니메이션 세팅
	{
		char AnimName[40] = "Colossus_";

		if (m_LR == EHandLR::Left)
			strcat_s(AnimName, "LeftHand_");
		else
			strcat_s(AnimName, "RightHand_");

		char ShadowAnimName[40] = "";
		strcat_s(ShadowAnimName, AnimName);
		strcat_s(ShadowAnimName, "Shadow");


		// 어택 카운트가 0일 때에는 방어 후 전방을 향해 이동한다.
		// 1 이상일 때에는 이미 손이 앞으로 나가있었기 때문에 옆으로 이동한다.
		if (m_AttackCount == 0) {
			strcat_s(AnimName, "Idle");
			strcat_s(ShadowAnimName, "Idle");
		}
		else {
			strcat_s(AnimName, "Idle");
			strcat_s(ShadowAnimName, "Idle");
		}

		m_Sprite->GetAnimation()->SetCurrentAnimation(AnimName);
		m_SpriteChild->GetAnimation()->SetCurrentAnimation(ShadowAnimName);
	}
}

void CGoliathHand::AttackStart()
{
	if (m_LR == EHandLR::Left)
		ReadyToMove();
	else
		MoveToColossus();
}

void CGoliathHand::GoliathStop()
{
}

void CGoliathHand::GoliathDead()
{
	Dead();
}

void CGoliathHand::ReadyToMove()
{
	m_GS = false;

	m_AttackPosY = GetWorldPos().y;
	m_DestPos = m_Scene->FindObject("Player")->GetWorldPos();

	// 각 공격 전, 공중에 떠올라 플레이어를 탐색하는 함수
	m_RtM = true;
	m_MtP = false;
	m_AtP = false;
	m_MtC = false;

	char AnimName[40] = "Colossus_";

	if (m_LR == EHandLR::Left)
		strcat_s(AnimName, "LeftHand_");
	else
		strcat_s(AnimName, "RightHand_");

	//char ShadowAnimName[40] = "";
	//strcat_s(ShadowAnimName, AnimName);
	//strcat_s(ShadowAnimName, "Shadow");


	// 어택 카운트가 0일 때에는 방어 후 전방을 향해 이동한다.
	// 1 이상일 때에는 이미 손이 앞으로 나가있었기 때문에 옆으로 이동한다.
	strcat_s(AnimName, "Up");

	m_Sprite->GetAnimation()->SetCurrentAnimation(AnimName);
	//m_SpriteChild->GetAnimation()->SetCurrentAnimation(ShadowAnimName);
}

void CGoliathHand::ReadyToMoveEnd()
{
	if (m_AttackCount > 2) {
		MoveToColossus();

		CGoliathHand* Hand;

		if (m_LR == EHandLR::Left)
			Hand = (CGoliathHand*)m_Scene->FindObject("Colossus_RightHand");
		else
			Hand = (CGoliathHand*)m_Scene->FindObject("Colossus_LeftHand");

		Hand->ReadyToMove();
	}
	else {
		MoveToPlayer();
	}
}

void CGoliathHand::MoveToPlayer()
{
	// 플레이어를 향해 이동한다(공격)
	m_RtM = false;
	m_MtP = true;
	m_AtP = false;
	m_MtC = false;


	// 애니메이션 세팅
	{
		char AnimName[40] = "Colossus_";

		if (m_LR == EHandLR::Left)
			strcat_s(AnimName, "LeftHand_");
		else
			strcat_s(AnimName, "RightHand_");

		//char ShadowAnimName[40] = "";
		//strcat_s(ShadowAnimName, AnimName);
		//strcat_s(ShadowAnimName, "Shadow");


		// 어택 카운트가 0일 때에는 방어 후 전방을 향해 이동한다.
		// 1 이상일 때에는 이미 손이 앞으로 나가있었기 때문에 옆으로 이동한다.
		strcat_s(AnimName, "MoveFront");
		//strcat_s(ShadowAnimName, "MoveFront");


		m_Sprite->GetAnimation()->SetCurrentAnimation(AnimName);
		//m_SpriteChild->GetAnimation()->SetCurrentAnimation(ShadowAnimName);
	}
}

void CGoliathHand::MoveToPlayerEnd()
{
	m_AttackPosY = GetWorldPos().y;

	AttackToPlayer();
}

void CGoliathHand::AttackToPlayer()
{
	m_RtM = false;
	m_MtP = false;
	m_AtP = true;
	m_MtC = false;


	// 애니메이션 세팅
	{
		char AnimName[40] = "Colossus_";

		if (m_LR == EHandLR::Left)
			strcat_s(AnimName, "LeftHand_");
		else
			strcat_s(AnimName, "RightHand_");

		//char ShadowAnimName[40] = "";
		//strcat_s(ShadowAnimName, AnimName);
		//strcat_s(ShadowAnimName, "Shadow");


		strcat_s(AnimName, "Attack");
		//strcat_s(ShadowAnimName, "Attack");


		m_Sprite->GetAnimation()->SetCurrentAnimation(AnimName);
		//m_SpriteChild->GetAnimation()->SetCurrentAnimation(ShadowAnimName);
	}
}

void CGoliathHand::AttackToPlayerEnd()
{

	m_AttackCount++;

	ReadyToMove();
}

void CGoliathHand::MoveToColossus()
{
	// 골리아스를 향해 이동한다(방어)
	m_RtM = false;
	m_MtP = false;
	m_AtP = false;
	m_MtC = true;


	// 애니메이션 세팅
	char AnimName[40] = "Colossus_";

	if (m_LR == EHandLR::Left)
		strcat_s(AnimName, "LeftHand_");
	else
		strcat_s(AnimName, "RightHand_");

	//char ShadowAnimName[40] = "";
	//strcat_s(ShadowAnimName, AnimName);
	//strcat_s(ShadowAnimName, "Shadow");


	strcat_s(AnimName, "MoveBack");
	//strcat_s(ShadowAnimName, "MoveBack");

	m_Sprite->GetAnimation()->SetCurrentAnimation(AnimName);
	//m_SpriteChild->GetAnimation()->SetCurrentAnimation(ShadowAnimName);

}

void CGoliathHand::MoveToColossusEnd()
{
	m_IsLand = true;

	// 몸쪽으로 이동을 완료한 후, 가드자세를 취한다.
	char AnimName[40] = "Colossus_";

	if (m_LR == EHandLR::Left)
		strcat_s(AnimName, "LeftHand_");
	else
		strcat_s(AnimName, "RightHand_");

	//char ShadowAnimName[40] = "";
	//strcat_s(ShadowAnimName, AnimName);
	//strcat_s(ShadowAnimName, "Shadow");

	strcat_s(AnimName, "Guard");
	//strcat_s(ShadowAnimName, "Guard");

	m_Sprite->GetAnimation()->SetCurrentAnimation(AnimName);
	//m_SpriteChild->GetAnimation()->SetCurrentAnimation(ShadowAnimName);

	// 공격 횟수를 0으로 초기화한다.
	m_AttackCount = 0;
}

void CGoliathHand::Guard()
{
	m_RtM = false;
	m_MtP = false;
	m_AtP = false;
	m_MtC = true;
}

void CGoliathHand::Dead()
{
	char AnimName[40] = "Colossus_";

	if (m_LR == EHandLR::Left)
		strcat_s(AnimName, "LeftHand_");
	else
		strcat_s(AnimName, "RightHand_");

	//char ShadowAnimName[40] = "";
	//strcat_s(ShadowAnimName, AnimName);
	//strcat_s(ShadowAnimName, "Shadow");

	strcat_s(AnimName, "Dead");
	//strcat_s(ShadowAnimName, "Dead");

	m_Sprite->GetAnimation()->SetCurrentAnimation(AnimName);
	//m_SpriteChild->GetAnimation()->SetCurrentAnimation(ShadowAnimName);

	m_IsDead = true;
}
