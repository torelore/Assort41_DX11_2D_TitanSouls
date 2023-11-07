#include "Player.h"

#include "Animation/Animation2D.h"

#include "Arrow.h"
#include "Portal.h"
#include "Yeti.h"
#include "Goliath.h"

#include "Component/CameraComponent.h"
#include "Component/ColliderOBB2D.h"
#include "Component/SpriteComponent.h"
#include "Component/TargetArm.h"
#include "Component/TileMapComponent.h"

#include "Device.h"
#include "Input.h"

#include "Resource/ResourceManager.h"
#include "../UI/GameDefaultUI.h"
#include "Scene/Scene.h"
#include "Scene/CameraManager.h"



// debug
#include <atlstr.h>
//CString strDebug;
//strDebug.Format(L"test\n");
//OutputDebugString(strDebug);

CPlayer::CPlayer()
{
	SetTypeID<CPlayer>();

	m_ObjectTypeName = "Player";

	m_Direction = EDirection::Up;
	m_PlayerState = EPlayerState::Idle;

	m_Speed = 250.f;
	m_AimTimer = 0.f;
	m_Timer = 0.f;
	m_BodySize = 32.f; 
	m_ShakeMagnitude = 2.f;
	m_ShakeTimer = 0.f;
	m_AscentPlayerPosY = 0.f;

	m_AscentRollCount = 0;


	m_ArrowExist = false;
	m_ArrowOnHand = false;
	m_ArrowCalling = false;
	m_MoveToPortal = false;
	m_DebugInvincible = false;
	m_ShakeCamera = false;
	m_IsAscent = false;
	m_IsAscentFall = false;

}

CPlayer::CPlayer(const CPlayer& Obj) :
	CGameObject(Obj)
{
	m_Sprite = (CSpriteComponent*)FindComponent("Player_Sprite");
	m_Camera = (CCameraComponent*)FindComponent("Player_Camera");
	m_Arm = (CTargetArm*)FindComponent("Player_Arm");
	m_Body = (CColliderOBB2D*)FindComponent("Player_Body");
}

CPlayer::~CPlayer()
{
}

void CPlayer::Start()
{
	CGameObject::Start();

	if (m_Scene)
		m_Scene->GetCameraManager()->SetCurrentCamera(m_Camera);


	CInput::GetInst()->AddBindFunction<CPlayer>("MoveUp", Input_Type::Push, this, &CPlayer::MoveUp, m_Scene);
	CInput::GetInst()->AddBindFunction<CPlayer>("MoveDown", Input_Type::Push, this, &CPlayer::MoveDown, m_Scene);
	CInput::GetInst()->AddBindFunction<CPlayer>("MoveLeft", Input_Type::Push, this, &CPlayer::MoveLeft, m_Scene);
	CInput::GetInst()->AddBindFunction<CPlayer>("MoveRight", Input_Type::Push, this, &CPlayer::MoveRight, m_Scene);

	CInput::GetInst()->AddBindFunction<CPlayer>("MoveUp", Input_Type::Up, this, &CPlayer::MoveEnd, m_Scene);
	CInput::GetInst()->AddBindFunction<CPlayer>("MoveDown", Input_Type::Up, this, &CPlayer::MoveEnd, m_Scene);
	CInput::GetInst()->AddBindFunction<CPlayer>("MoveLeft", Input_Type::Up, this, &CPlayer::MoveEnd, m_Scene);
	CInput::GetInst()->AddBindFunction<CPlayer>("MoveRight", Input_Type::Up, this, &CPlayer::MoveEnd, m_Scene);

	CInput::GetInst()->AddBindFunction<CPlayer>("Dash", Input_Type::Down, this, &CPlayer::Dash, m_Scene);
	
	CInput::GetInst()->AddBindFunction<CPlayer>("ArrowAim", Input_Type::Push, this, &CPlayer::ArrowAim, m_Scene);
	CInput::GetInst()->AddBindFunction<CPlayer>("ArrowShoot", Input_Type::Up, this, &CPlayer::ArrowShoot, m_Scene);
	CInput::GetInst()->AddBindFunction<CPlayer>("ArrowCall", Input_Type::Push, this, &CPlayer::ArrowCall, m_Scene);
	CInput::GetInst()->AddBindFunction<CPlayer>("ArrowCallStop", Input_Type::Up, this, &CPlayer::ArrowCallStop, m_Scene);


	CInput::GetInst()->AddBindFunction<CPlayer>("DebugModeF2", Input_Type::Up, this, &CPlayer::InvincibleMode, m_Scene);


	CInput::GetInst()->AddBindFunction<CPlayer>("DebugModeF3", Input_Type::Push, this, &CPlayer::Test1, m_Scene);
	CInput::GetInst()->AddBindFunction<CPlayer>("DebugModeF3", Input_Type::Up, this, &CPlayer::Test2, m_Scene);

}

bool CPlayer::Init()
{
	CGameObject::Init();

	m_Sprite = CreateComponent<CSpriteComponent>("Player_Sprite");
	m_Camera = CreateComponent<CCameraComponent>("Player_Camera");
	m_Arm = CreateComponent<CTargetArm>("Player_Arm");
	m_Body = CreateComponent<CColliderOBB2D>("Player_Body");

	SetRootComponent(m_Body);

	m_Body->SetCollisionProfile("Player");
	m_Body->AddChild(m_Sprite);
	m_Body->SetBoxHalfSize(m_BodySize / 2.f, m_BodySize / 2.f);

	//m_Sprite->GetMaterial(0)->SetBaseColorUnsignedChar(255, 255, 0, 255);
	m_Sprite->AddChild(m_Arm);

	m_Arm->AddChild(m_Camera);

	Resolution RS = CDevice::GetInst()->GetResolution();

	float Width = (float)RS.Width / 2.f;
	float Height = (float)RS.Height / 2.f;

	m_Arm->SetTargetOffset(Vector3(-Width, -Height, 0.f));

	m_Sprite->SetRelativeScale(m_BodySize, m_BodySize);
	m_Sprite->SetPivot(0.5f, 0.5f);
	m_Sprite->SetInheritRotZ(true);


	//CMaterial* Material = m_Sprite->GetMaterial(0);
	//Material->SetOpacity(0.5f);


	// Animation
	std::vector<const char*> vecDir = { "Right", "Up", "Left", "Down", "RightDown", "LeftDown", "LeftUp", "RightUp" };

	// Animation Setting
	{
		CAnimation2D* Anim = m_Sprite->SetAnimation<CAnimation2D>("PlayerAnim");

		// Idle(land)
		{
			const std::string& state = "Idle";

			for (size_t vecIdx = 0; vecIdx < vecDir.size(); vecIdx++) {
				char AnimName[40] = "";

				strcat_s(AnimName, state.c_str());
				strcat_s(AnimName, vecDir[vecIdx]);


				Anim->AddAnimation(AnimName, AnimName);
				Anim->SetLoop(AnimName, true);
			}
		}

		
		// Move
		{
			const std::string& state = "Move";

			for (size_t vecIdx = 0; vecIdx < vecDir.size(); vecIdx++) {
				char AnimName[40] = "";

				strcat_s(AnimName, state.c_str());
				strcat_s(AnimName, vecDir[vecIdx]);


				Anim->AddAnimation(AnimName, AnimName);
				Anim->SetLoop(AnimName, true);
			}
		}


		// Dash(land)
		{
			const std::string& state = "Dash";

			for (size_t vecIdx = 0; vecIdx < vecDir.size(); vecIdx++) {
				char AnimName[40] = "";

				strcat_s(AnimName, state.c_str());
				strcat_s(AnimName, vecDir[vecIdx]);


				Anim->AddAnimation(AnimName, AnimName);
				Anim->SetCurrentEndFunction<CPlayer>(AnimName, this, &CPlayer::DashEnd);
				Anim->SetPlayTime(AnimName, 0.5f);
				Anim->SetLoop(AnimName, false);
			}
		}


		// Swim
		{
			const std::string& state = "Swim";

			for (size_t vecIdx = 0; vecIdx < vecDir.size(); vecIdx++) {
				char AnimName[40] = "";

				strcat_s(AnimName, state.c_str());
				strcat_s(AnimName, vecDir[vecIdx]);


				Anim->AddAnimation(AnimName, AnimName);
				Anim->SetLoop(AnimName, true);
			}
		}


		// Idle(Water) (2Sec)
		{
			const std::string& state = "IdleSwim";

			for (size_t vecIdx = 0; vecIdx < vecDir.size(); vecIdx++) {
				char AnimName[40] = "";

				strcat_s(AnimName, state.c_str());
				strcat_s(AnimName, vecDir[vecIdx]);


				Anim->AddAnimation(AnimName, AnimName);
				Anim->SetLoop(AnimName, true);
				Anim->SetPlayTime(AnimName, 2.f);
			}
		}


		// Climbing
		{
			Anim->AddAnimation("Climb", "Climb");
			Anim->SetLoop("Climb", false);

			Anim->AddAnimation("IdleClimb", "IdleClimb");
			Anim->SetLoop("IdleClimb", true);
		}

		
		// Aim
		{
			const std::string& state = "Aim";

			for (size_t vecIdx = 0; vecIdx < vecDir.size(); vecIdx++) {
				char AnimName[40] = "";

				strcat_s(AnimName, state.c_str());
				strcat_s(AnimName, vecDir[vecIdx]);


				Anim->AddAnimation(AnimName, AnimName);
				Anim->SetLoop(AnimName, true);
			}
		}


		// Calling
		{
			const std::string& state = "Call";

			for (size_t vecIdx = 0; vecIdx < vecDir.size(); vecIdx++) {
				char AnimName[40] = "";

				strcat_s(AnimName, state.c_str());
				strcat_s(AnimName, vecDir[vecIdx]);


				Anim->AddAnimation(AnimName, AnimName);
				Anim->SetLoop(AnimName, true);
			}
		}


		// Dead (2Sec)
		{
			const std::string& state = "Dead";

			for (size_t vecIdx = 0; vecIdx < vecDir.size(); vecIdx++) {
				char AnimName[40] = "";

				strcat_s(AnimName, state.c_str());
				strcat_s(AnimName, vecDir[vecIdx]);


				Anim->AddAnimation(AnimName, AnimName);
				Anim->SetPlayTime(AnimName, 2.f);
				Anim->SetLoop(AnimName, false);
			}
		}


		// Ascent
		{
			Anim->AddAnimation("AscentRoll_Mono", "AscentRoll_Mono");
			Anim->SetLoop("AscentRoll_Mono", false);
			Anim->SetPlayTime("AscentRoll_Mono", 1.f);
			Anim->SetCurrentEndFunction<CPlayer>("AscentRoll_Mono", this, &CPlayer::AnimAscentRollMonoEnd);

			Anim->AddAnimation("AscentRoll", "AscentRoll");
			Anim->SetLoop("AscentRoll", false);
			Anim->SetPlayTime("AscentRoll", 0.5f);
			Anim->SetCurrentEndFunction<CPlayer>("AscentRoll", this, &CPlayer::AnimAscentRollEnd);

			Anim->AddAnimation("AscentFall_Mono", "AscentFall_Mono");
			Anim->SetLoop("AscentFall_Mono", false);
			Anim->SetPlayTime("AscentFall_Mono", 1.f);
			Anim->SetCurrentEndFunction<CPlayer>("AscentFall_Mono", this, &CPlayer::AnimAscentFallMonoEnd);

			Anim->AddAnimation("AscentFall", "AscentFall");
			Anim->SetLoop("AscentFall", false);
			Anim->SetPlayTime("AscentFall", 7.f);
			Anim->SetCurrentEndFunction<CPlayer>("AscentFall", this, &CPlayer::AnimAscentFallEnd);
		}


		Anim->SetCurrentAnimation("IdleUp");

	} // Animation Setting End



	return true;
}

void CPlayer::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

	m_Timer += DeltaTime;
	m_ShakeTimer += DeltaTime;

	if (m_PlayerState == EPlayerState::Dead)
		return;

	if (m_ArrowOnHand)
		m_AimTimer += DeltaTime;


	if (m_ShakeCamera) {
		if (m_ShakeTimer > m_ShakeTime)
			m_ShakeCamera = false;


		const Vector3& ArmPos = m_Arm->GetWorldPos();
		const Vector3& CameraPos = m_Camera->GetWorldPos();

		if (ArmPos.x == CameraPos.x) {
			m_Camera->SetWorldPosition(CameraPos.x + m_ShakeMagnitude, CameraPos.y);
		}
		else if (ArmPos.x >= CameraPos.x) {
			float x = ArmPos.x + m_ShakeMagnitude;

			m_Camera->SetWorldPosition(x, CameraPos.y);
		}
		else if (ArmPos.x <= CameraPos.x) {
			float x = ArmPos.x - m_ShakeMagnitude;

			m_Camera->SetWorldPosition(x, CameraPos.y);
		}
	}


	// 승천, 처형 모션
	if (m_IsAscent) {
		if (m_AscentPlayerPosY == 0.f) {
			m_AscentPlayerPosY = GetWorldPos().y;
		}

		const Vector3& rScale = m_Sprite->GetRelativeScale();

		if (rScale.x <= m_BodySize * 3.f) {
#if _DEBUG
			m_Sprite->AddRelativeScale(0.01f, 0.01f);
#else
			m_Sprite->AddRelativeScale(0.005f, 0.005f);
#endif
		}

#if _DEBUG
		m_Body->AddRelativePositionY(0.05f);
#else
		m_Body->AddRelativePositionY(0.005f);
#endif

		return;
	}

	if (m_IsAscentFall) {

		const Vector3& rScale = m_Sprite->GetRelativeScale();

		if (rScale.x >= m_BodySize) {
#if _DEBUG
			m_Sprite->AddRelativeScale(-0.01f, -0.01f);
#else
			m_Sprite->AddRelativeScale(-0.005f, -0.005f);
#endif
		}

		if (m_AscentPlayerPosY <= GetWorldPos().y) {

#if _DEBUG
			m_Body->AddRelativePositionY(-0.05f);
#else
			m_Body->AddRelativePositionY(-0.005f);
#endif

		}

		return;
	}


	// 플레이어가 서있는 타일맵이 전경(플레이어보다 앞에 있는 배경)인지 체크하여 basecolor를 변경
	CTileMapComponent* RootTilemapBackground = (CTileMapComponent*)m_Scene->FindObject("TilemapObjLayer_Root_FG")->FindComponent("TilemapCompLayer_Root_FG");
	ETileOption tileOption = RootTilemapBackground->GetTileOption(GetWorldPos());

	if (tileOption == ETileOption::FG)
		m_Sprite->GetMaterial(0)->SetBaseColor(0, 0, 0, 0);
	else 
		m_Sprite->GetMaterial(0)->SetBaseColor(1, 1, 1, 1);

	// 플레이어가 현재 서있는 타일의 종류를 체크
	RootTilemapBackground = (CTileMapComponent*)m_Scene->FindObject("TilemapObjLayer_Root")->FindComponent("TilemapCompLayer_Root");
	tileOption = RootTilemapBackground->GetTileOption(GetWorldPos());

	char AnimName[40] = "";

	bool isDash = false;
	bool isClimb = false;
	bool isWater = false;


	// 포탈에 들어갈 경우에 대한 처리
	if (m_MoveToPortal) {
		StepSoundStop();
		strcat_s(AnimName, "Move");

		if (!m_Scene->GetResource()->FindSound("EnterDoor")->GetPlay())
			m_Scene->GetResource()->FindSound("EnterDoor")->Play();

		switch (m_Direction)
		{
		case EDirection::Up:
			strcat_s(AnimName, "Up");
			m_Body->AddWorldPosition(m_Body->GetWorldAxis(AXIS_Y) * m_Speed / 5.f * DeltaTime);
			break;
		case EDirection::Down:
			strcat_s(AnimName, "Down");
			m_Body->AddWorldPosition(m_Body->GetWorldAxis(AXIS_Y) * -m_Speed / 5.f * DeltaTime);
			break;
		}


		CCollider* dest = m_Body->GetColliderDest();

		if (dest == nullptr)
			return;

		CPortal* Portal = (CPortal*)dest->GetOwner();

		if ((int)GetWorldPos().y == (int)Portal->GetWorldPos().y)
			Portal->CreateNextScene();

		return;
	}


	// 플레이어 상태에 따른 애니메이션 변경 분기 삽입
	switch (m_PlayerState)
	{
	case EPlayerState::Idle:
		StepSoundStop();

		strcat_s(AnimName, "Idle");

		if (tileOption == ETileOption::Water)
			strcat_s(AnimName, "Swim");
		else if (tileOption == ETileOption::Climb) {
			strcat_s(AnimName, "Climb");
			isClimb = true;
		} 
		break;
	case EPlayerState::Aiming:
		StepSoundStop();

		strcat_s(AnimName, "Aim");

		break;
	case EPlayerState::Calling:
		StepSoundStop();

		strcat_s(AnimName, "Call");
		break;
	case EPlayerState::Walking:
		if (tileOption == ETileOption::Water) {
			strcat_s(AnimName, "Swim");


			m_Scene->GetResource()->FindSound("VineStep")->Stop();
			m_Scene->GetResource()->FindSound("StoneStep")->Stop();

			if (!m_Scene->GetResource()->FindSound("WaterStep")->GetPlay())
				m_Scene->GetResource()->FindSound("WaterStep")->Play();
		}
		else if (tileOption == ETileOption::Climb) {
			strcat_s(AnimName, "Climb");


			m_Scene->GetResource()->FindSound("WaterStep")->Stop();
			m_Scene->GetResource()->FindSound("StoneStep")->Stop();

			if (!m_Scene->GetResource()->FindSound("VineStep")->GetPlay())
				m_Scene->GetResource()->FindSound("VineStep")->Play();

			isClimb = true;
		}
		else {
			strcat_s(AnimName, "Move");


			m_Scene->GetResource()->FindSound("WaterStep")->Stop();
			m_Scene->GetResource()->FindSound("VineStep")->Stop();

			if (!m_Scene->GetResource()->FindSound("StoneStep")->GetPlay())
				m_Scene->GetResource()->FindSound("StoneStep")->Play();
		}
		break;
	case EPlayerState::Dash:
		StepSoundStop();

		strcat_s(AnimName, "Dash");

		isDash = true;
		break;
	}

	// 등반 중일 경우, 방향 상관없는 애니메이션을 처리하기 때문에, 플레이어 방향과 상관없이 애니메이션 변경 후 반환.
	if (isClimb) {
		m_Sprite->GetAnimation()->ChangeAnimation(AnimName);
		return;
	}



	float DashSpeedScale = 2.f;
	float DiagonalDashSpeedScale = sqrt(DashSpeedScale);


	// 플레이어 방향에 따른 애니메이션 분기 삽입
	switch (m_Direction)
	{
	case EDirection::Up:
		strcat_s(AnimName, "Up");

		if (isDash) {
			const Vector3& tempPos = Vector3(GetWorldPos().x, GetWorldPos().y + m_BodySize / 2.f, GetWorldPos().z);
			tileOption = RootTilemapBackground->GetTileOption(tempPos);

			if (tileOption == ETileOption::Wall || tileOption == ETileOption::Stair || tileOption == ETileOption::Climb || tileOption == ETileOption::Water) {
				m_PlayerState = EPlayerState::Walking;
				break;
			}

			AddWorldPosition(GetWorldAxis(AXIS_Y) * m_Speed * DashSpeedScale * DeltaTime);
		}
		break;
	case EDirection::Down:
		strcat_s(AnimName, "Down");

		if (isDash) {
			const Vector3& tempPos = Vector3(GetWorldPos().x, GetWorldPos().y - m_BodySize / 2.f, GetWorldPos().z);
			tileOption = RootTilemapBackground->GetTileOption(tempPos);

			if (tileOption == ETileOption::Wall || tileOption == ETileOption::Stair || tileOption == ETileOption::Climb || tileOption == ETileOption::Water) {
				m_PlayerState = EPlayerState::Walking;
				break;
			}

			AddWorldPosition(GetWorldAxis(AXIS_Y) * -m_Speed * DashSpeedScale * DeltaTime);
		}
		break;
	case EDirection::Left:
		strcat_s(AnimName, "Left");

		if (isDash) {
			const Vector3& tempPos = Vector3(GetWorldPos().x - m_BodySize / 2.f, GetWorldPos().y, GetWorldPos().z);
			tileOption = RootTilemapBackground->GetTileOption(tempPos);

			if (tileOption == ETileOption::Wall || tileOption == ETileOption::Stair || tileOption == ETileOption::Climb || tileOption == ETileOption::Water) {
				m_PlayerState = EPlayerState::Walking;
				break;
			}

			AddWorldPosition(GetWorldAxis(AXIS_X) * -m_Speed * DashSpeedScale * DeltaTime);
		}
		break;
	case EDirection::Right:
		strcat_s(AnimName, "Right");

		if (isDash) {
			const Vector3& tempPos = Vector3(GetWorldPos().x + m_BodySize / 2.f, GetWorldPos().y, GetWorldPos().z);
			tileOption = RootTilemapBackground->GetTileOption(tempPos);

			if (tileOption == ETileOption::Wall || tileOption == ETileOption::Stair || tileOption == ETileOption::Climb || tileOption == ETileOption::Water) {
				m_PlayerState = EPlayerState::Walking;
				break;
			}

			AddWorldPosition(GetWorldAxis(AXIS_X) * m_Speed * DashSpeedScale* DeltaTime);
		}
		break;
	case EDirection::LeftUp:
		strcat_s(AnimName, "LeftUp");

		if (isDash) {
			const Vector3& tempPos = Vector3(GetWorldPos().x - m_BodySize / 2.f, GetWorldPos().y + m_BodySize / 2.f, GetWorldPos().z);
			tileOption = RootTilemapBackground->GetTileOption(tempPos);

			if (tileOption == ETileOption::Wall || tileOption == ETileOption::Stair || tileOption == ETileOption::Climb || tileOption == ETileOption::Water) {
				m_PlayerState = EPlayerState::Walking;
				break;
			}

			AddWorldPosition(GetWorldAxis(AXIS_X) * -m_Speed * DiagonalDashSpeedScale * DeltaTime);
			AddWorldPosition(GetWorldAxis(AXIS_Y)* m_Speed* DiagonalDashSpeedScale* DeltaTime);
		}
		break;
	case EDirection::LeftDown:
		strcat_s(AnimName, "LeftDown");

		if (isDash) {
			const Vector3& tempPos = Vector3(GetWorldPos().x - m_BodySize / 2.f, GetWorldPos().y - m_BodySize / 2.f, GetWorldPos().z);
			tileOption = RootTilemapBackground->GetTileOption(tempPos);

			if (tileOption == ETileOption::Wall || tileOption == ETileOption::Stair || tileOption == ETileOption::Climb || tileOption == ETileOption::Water) {
				m_PlayerState = EPlayerState::Walking;
				break;
			}

			AddWorldPosition(GetWorldAxis(AXIS_X) * -m_Speed * DiagonalDashSpeedScale * DeltaTime);
			AddWorldPosition(GetWorldAxis(AXIS_Y) * -m_Speed * DiagonalDashSpeedScale * DeltaTime);
		}
		break;
	case EDirection::RightUp:
		strcat_s(AnimName, "RightUp");

		if (isDash) {
			const Vector3& tempPos = Vector3(GetWorldPos().x + m_BodySize / 2.f, GetWorldPos().y + m_BodySize / 2.f, GetWorldPos().z);
			tileOption = RootTilemapBackground->GetTileOption(tempPos);

			if (tileOption == ETileOption::Wall || tileOption == ETileOption::Stair || tileOption == ETileOption::Climb || tileOption == ETileOption::Water) {
				m_PlayerState = EPlayerState::Walking;
				break;
			}

			AddWorldPosition(GetWorldAxis(AXIS_X)* m_Speed* DiagonalDashSpeedScale* DeltaTime);
			AddWorldPosition(GetWorldAxis(AXIS_Y)* m_Speed* DiagonalDashSpeedScale* DeltaTime);
		}
		break;
	case EDirection::RightDown:
		strcat_s(AnimName, "RightDown");

		if (isDash) {
			const Vector3& tempPos = Vector3(GetWorldPos().x + m_BodySize / 2.f, GetWorldPos().y - m_BodySize / 2.f, GetWorldPos().z);
			tileOption = RootTilemapBackground->GetTileOption(tempPos);

			if (tileOption == ETileOption::Wall || tileOption == ETileOption::Stair || tileOption == ETileOption::Climb || tileOption == ETileOption::Water) {
				m_PlayerState = EPlayerState::Walking;
				break;
			}

			AddWorldPosition(GetWorldAxis(AXIS_X)* m_Speed* DiagonalDashSpeedScale* DeltaTime);
			AddWorldPosition(GetWorldAxis(AXIS_Y) * -m_Speed * DiagonalDashSpeedScale * DeltaTime);
		}
		break;
	}


	// 업데이트에서 애니메이션을 변경하기 때문에
	// 기존과 동일한 애니메이션이 실행되는 경우 해당 애니메이션이 끝나기 전에
	// 같은 애니메이션을 다시 시작하지 않게 하기 위한 처리 
	if (strcmp(AnimName, m_Sprite->GetAnimation()->GetCurrentAnimationName("").c_str()) != 0) {
		m_Sprite->GetAnimation()->ChangeAnimation(AnimName);
	}

}

void CPlayer::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

CPlayer* CPlayer::Clone() const
{
	return new CPlayer(*this);
}

void CPlayer::Save(FILE* File)
{
	CGameObject::Save(File);
}

void CPlayer::Load(FILE* File)
{
	CGameObject::Load(File);
}

void CPlayer::MoveUp()
{
	// 대쉬 중, 조준 중, 사망 상태일 때에는 움직이지 않는다.
	if (m_PlayerState == EPlayerState::Dash || m_PlayerState == EPlayerState::Aiming || m_PlayerState == EPlayerState::Dead)
		return;


	// 입력 직후 방향을 바꿔준다.
	if (m_Direction == EDirection::Left)
		m_Direction = EDirection::LeftUp;
	else if (m_Direction == EDirection::Right)
		m_Direction = EDirection::RightUp;
	else
		m_Direction = EDirection::Up;


	// 벽, 계단, 그 외의 경우에 대한 속도와 이동 가능 여부에 대한 컨트롤.
	CTileMapComponent* RootTilemapBackground = (CTileMapComponent*)m_Scene->FindObject("TilemapObjLayer_Root")->FindComponent("TilemapCompLayer_Root");
	const Vector3& Pos = Vector3(GetWorldPos().x, GetWorldPos().y + m_BodySize / 2.f, GetWorldPos().z);
	ETileOption tileOption = RootTilemapBackground->GetTileOption(Pos);

	if (tileOption == ETileOption::Wall)
		return;
	else if (tileOption == ETileOption::Stair)
		m_Speed = 150.f;
	else
		m_Speed = 250.f;


	m_Body->AddWorldPosition(m_Body->GetWorldAxis(AXIS_Y) * m_Speed * g_DeltaTime);

	m_PlayerState = EPlayerState::Walking;
}

void CPlayer::MoveDown()
{
	// 대쉬 중, 조준 중, 사망 상태일 때에는 움직이지 않는다.
	if (m_PlayerState == EPlayerState::Dash || m_PlayerState == EPlayerState::Aiming || m_PlayerState == EPlayerState::Dead)
		return;

	if (m_Direction == EDirection::Left)
		m_Direction = EDirection::LeftDown;
	else if (m_Direction == EDirection::Right)
		m_Direction = EDirection::RightDown;
	else
		m_Direction = EDirection::Down;


	CTileMapComponent* RootTilemapBackground = (CTileMapComponent*)m_Scene->FindObject("TilemapObjLayer_Root")->FindComponent("TilemapCompLayer_Root");
	const Vector3& Pos = Vector3(GetWorldPos().x, GetWorldPos().y - m_BodySize / 2.f, GetWorldPos().z);
	ETileOption tileOption = RootTilemapBackground->GetTileOption(Pos);

	if (tileOption == ETileOption::Wall)
		return;
	else if (tileOption == ETileOption::Stair)
		m_Speed = 150.f;
	else
		m_Speed = 250.f;


	m_Body->AddWorldPosition(m_Body->GetWorldAxis(AXIS_Y) * -m_Speed * g_DeltaTime);

	m_PlayerState = EPlayerState::Walking;
}

void CPlayer::MoveLeft()
{
	// 대쉬 중, 조준 중, 사망 상태일 때에는 움직이지 않는다.
	if (m_PlayerState == EPlayerState::Dash || m_PlayerState == EPlayerState::Aiming || m_PlayerState == EPlayerState::Dead)
		return;


	if (m_Direction == EDirection::Up)
		m_Direction = EDirection::LeftUp;
	else if (m_Direction == EDirection::Down)
		m_Direction = EDirection::LeftDown;
	else
		m_Direction = EDirection::Left;



	CTileMapComponent* RootTilemapBackground = (CTileMapComponent*)m_Scene->FindObject("TilemapObjLayer_Root")->FindComponent("TilemapCompLayer_Root");
	const Vector3& Pos = Vector3(GetWorldPos().x - m_BodySize / 2.f, GetWorldPos().y, GetWorldPos().z);
	ETileOption tileOption = RootTilemapBackground->GetTileOption(Pos);

	if (tileOption == ETileOption::Wall)
		return;
	else if (tileOption == ETileOption::Stair)
		m_Speed = 150.f;
	else
		m_Speed = 250.f;


	m_Body->AddWorldPosition(m_Body->GetWorldAxis(AXIS_X) * -m_Speed * g_DeltaTime);

	m_PlayerState = EPlayerState::Walking;
}

void CPlayer::MoveRight()
{
	// 대쉬 중, 조준 중, 사망 상태일 때에는 움직이지 않는다.
	if (m_PlayerState == EPlayerState::Dash || m_PlayerState == EPlayerState::Aiming || m_PlayerState == EPlayerState::Dead)
		return;


	if (m_Direction == EDirection::Up)
		m_Direction = EDirection::RightUp;
	else if (m_Direction == EDirection::Down)
		m_Direction = EDirection::RightDown;
	else
		m_Direction = EDirection::Right;


	CTileMapComponent* RootTilemapBackground = (CTileMapComponent*)m_Scene->FindObject("TilemapObjLayer_Root")->FindComponent("TilemapCompLayer_Root");
	const Vector3& Pos = Vector3(GetWorldPos().x + m_BodySize / 2.f, GetWorldPos().y, GetWorldPos().z);
	ETileOption tileOption = RootTilemapBackground->GetTileOption(Pos);

	if (tileOption == ETileOption::Wall)
		return;
	else if (tileOption == ETileOption::Stair)
		m_Speed = 150.f;
	else
		m_Speed = 250.f;


	m_Body->AddWorldPosition(m_Body->GetWorldAxis(AXIS_X) * m_Speed * g_DeltaTime);

	m_PlayerState = EPlayerState::Walking;
}

void CPlayer::MoveEnd()
{
	if (m_PlayerState == EPlayerState::Dash || m_PlayerState == EPlayerState::Aiming || m_PlayerState == EPlayerState::Dead)
		return;

	m_PlayerState = EPlayerState::Idle;
}

void CPlayer::Dash()
{
	if (m_PlayerState == EPlayerState::Dash || m_PlayerState == EPlayerState::Dead)
		return;

	// 물, 절벽, 계단 위에 있을 때에는 대쉬 불가.
	CTileMapComponent* RootTilemapBackground = (CTileMapComponent*)m_Scene->FindObject("TilemapObjLayer_Root")->FindComponent("TilemapCompLayer_Root");
	ETileOption tileOption = RootTilemapBackground->GetTileOption(GetWorldPos());

	if (tileOption == ETileOption::Water || tileOption == ETileOption::Climb || tileOption == ETileOption::Stair)
		return;


	// 화살을 꺼내들고 조준 중일 때에는 즉시 화살을 제거하고 대쉬
	if (m_ArrowExist && m_ArrowOnHand) {
		CArrow* Arrow = (CArrow*)m_Scene->FindObject("Arrow");
	
		if(Arrow)
			Arrow->Destroy();

		m_ArrowExist = false;
		m_ArrowOnHand = false;
	}

	m_Scene->GetResource()->FindSound("Roll")->Play();

	m_PlayerState = EPlayerState::Dash;
}

void CPlayer::DashEnd()
{
	m_PlayerState = EPlayerState::Idle;
}

void CPlayer::ArrowAim()
{
	if (m_PlayerState == EPlayerState::Dead)
		return;

	// 화살이 월드에 존재하지 않거나, 손에 화살이 있다면
	if (!m_ArrowExist || m_ArrowOnHand) {
		// 현재 마우스가 위치한 방향을 산출하는 삼각함수
		Resolution RS = CDevice::GetInst()->GetResolution();

		const Vector3& MousePos = Vector3(CInput::GetInst()->GetMousePos().x - RS.Width / 2.f, CInput::GetInst()->GetMousePos().y - RS.Height / 2.f, 0.f);

		float rot = atan2f(MousePos.x, MousePos.y) * 180 / PI;

		if (rot < 0)
			rot = 360.f + rot;

		// 조준 중인 방향을 향해 캐릭터의 보는 방향을 변경.
		{
			float standard = 45.f;

			if (rot < standard / 2.f || rot > 360.f - standard / 2.f) { // Up
				m_Direction = EDirection::Up;
			}
			else if (rot < standard + standard / 2.f) { // RightUp
				m_Direction = EDirection::RightUp;
			}
			else if (rot < standard * 2 + standard / 2.f) { // Right
				m_Direction = EDirection::Right;
			}
			else if (rot < standard * 3 + standard / 2.f) { // RightDown
				m_Direction = EDirection::RightDown;
			}
			else if (rot < standard * 4 + standard / 2.f) { // Down
				m_Direction = EDirection::Down;
			}
			else if (rot < standard * 5 + standard / 2.f) { // LeftDown
				m_Direction = EDirection::LeftDown;
			}
			else if (rot < standard * 6 + standard / 2.f) { // Left
				m_Direction = EDirection::Left;
			}
			else if (rot < standard * 7 + standard / 2.f) { // Left
				m_Direction = EDirection::Left;
			}
		}

		// 화살 조준 위치에 따라 카메라의 위치를 변경
		float degree = rot * PI / 180.f;

		float x = sin(degree) * 50.f;
		float y = cos(degree) * 50.f;

		const Vector3& ArmPos = m_Arm->GetWorldPos();
		m_Camera->SetWorldPosition(ArmPos.x + x, ArmPos.y + y);


		// 화살 오브젝트가 이미 있다면 가져다 쓰고, 아니라면 만든다.
		CArrow* Arrow = (CArrow*)m_Scene->FindObject("Arrow");

		if (Arrow == nullptr) 
			Arrow = m_Scene->CreateObject<CArrow>("Arrow");

		Arrow->SetWorldPosition(GetWorldPos());
		Arrow->SetWorldRotationZ(-rot);


		m_ArrowExist = true;
		m_ArrowOnHand = true;

		CSound* ChargeSound = m_Scene->GetResource()->FindSound("arrow_charge");

		if (!ChargeSound->GetPlay())
			ChargeSound->Play();

		m_PlayerState = EPlayerState::Aiming;
	}
}

void CPlayer::ArrowShoot()
{
	m_Scene->GetResource()->FindSound("arrow_charge")->Stop();


	// 화살이 손에 있었다면 발사한다.
	if (m_ArrowOnHand) {
		// 카메라 원위치
		const Vector3& ArmPos = m_Arm->GetWorldPos();
		m_Camera->SetWorldPosition(ArmPos);


		CArrow* Arrow = (CArrow*)m_Scene->FindObject("Arrow");

		if (!Arrow)
			return;

		// 조준한 시간만큼 화살이 멀리 발사되게끔 처리.
		// 속도는 가로(시간)/세로(속도)일 때의 아크탄젠트그래프의 형태와 같은 곡선을 그린다.
		float ArrowSpeed = 0.f;
		float SpeedMax = 0.f;

#if _DEBUG
		ArrowSpeed = 500.f + m_AimTimer * 400.f;
		SpeedMax = 1000.f;
#else
		ArrowSpeed = 1000.f + m_AimTimer * 600.f;
		SpeedMax = 2000.f;
#endif


		if (ArrowSpeed > SpeedMax)
			ArrowSpeed = SpeedMax;

		Arrow->SetSpeed(ArrowSpeed);
		m_AimTimer = 0.f;


	#if _DEBUG
		Arrow->SetAccel(-5.f);
	#else
		Arrow->SetAccel(-1.f);
	#endif


		m_ArrowOnHand = false;

		// 카메라 위치 재조정
		m_Arm->SetTargetDistance(0.f);

		m_Scene->GetResource()->FindSound("arrow_shoot")->Play();

		m_PlayerState = EPlayerState::Idle;
	}
}

void CPlayer::ArrowCall()
{
	if (m_PlayerState == EPlayerState::Dead)
		return;


	CArrow* Arrow = (CArrow*)m_Scene->FindObject("Arrow");

	if (Arrow == nullptr)
		return;

	if (Arrow->IsWeakHit())
		return;

	// 플레이어가 화살을 부르는 도중, 걷거나 대쉬를 할 경우 즉시 화살 회수를 중지하도록 처리
	if (m_PlayerState == EPlayerState::Walking || m_PlayerState == EPlayerState::Dash) {
		ArrowCallStop();
		return;
	}

	// 화살이 존재하고, 손에 없으며, 부르는 중이 아니었을 경우에만 동작
	if (m_ArrowExist && !m_ArrowOnHand && !m_ArrowCalling) {
		// 플레이어의 현재 위치에 해당하는 각도를 삼각함수(tan)로 탐색하여 화살의 방향 지정
		const Vector3& ArrowPos = GetWorldPos() - Arrow->GetWorldPos();

		float rot = atan2f(ArrowPos.x, ArrowPos.y) * 180 / PI;

		if (rot < 0) {
			rot = 360.f + rot;
		}

		Arrow->SetRelativeRotationZ(-rot);
#if _DEBUG
		Arrow->SetSpeed(100.f);
		Arrow->SetAccel(20.f);
#else
		Arrow->SetSpeed(500.f);
		Arrow->SetAccel(2.f);
#endif


		m_ArrowCalling = true;


		if (!m_Scene->GetResource()->FindSound("arrow_retrieval")->GetPlay())
			m_Scene->GetResource()->FindSound("arrow_retrieval")->Play();


		m_PlayerState = EPlayerState::Calling;
	}
}

void CPlayer::ArrowCallStop()
{
	if (m_PlayerState == EPlayerState::Dead)
		return;

	m_Scene->GetResource()->FindSound("arrow_retrieval")->Stop();


	CArrow* Arrow = (CArrow*)m_Scene->FindObject("Arrow");

	if (Arrow == nullptr)
		return;


	if (Arrow->IsWeakHit()) {
		TitanExecution();
		return;
	}


	// 화살이 존재하고, 손에 있지 않을 때만 동작
	if (m_ArrowExist && !m_ArrowOnHand) {
		// 현재 화살이 끌려오는 상태(플레이어 방향으로 가속을 받은 상태)에서 반대로 지정하여 관성에 따라 멈추도록 작업
	#if _DEBUG
		Arrow->SetAccel(-20.f);
	#else
		Arrow->SetAccel(-10.f);
	#endif

	}

	m_ArrowCalling = false;

	m_PlayerState = EPlayerState::Idle;
}

void CPlayer::PlayerDead()
{
	char AnimName[40] = "Dead";

	switch (m_Direction)
	{
	case EDirection::Up:
		strcat_s(AnimName, "Up");
		break;
	case EDirection::Down:
		strcat_s(AnimName, "Down");
		break;
	case EDirection::Left:
		strcat_s(AnimName, "Left");
		break;
	case EDirection::Right:
		strcat_s(AnimName, "Right");
		break;
	case EDirection::LeftUp:
		strcat_s(AnimName, "LeftUp");
		break;
	case EDirection::LeftDown:
		strcat_s(AnimName, "LeftDown");
		break;
	case EDirection::RightUp:
		strcat_s(AnimName, "RightUp");
		break;
	case EDirection::RightDown:
		strcat_s(AnimName, "RightDown");
		break;
	}
	
	m_Sprite->GetAnimation()->SetCurrentAnimation(AnimName);
	m_PlayerState = EPlayerState::Dead;
}

void CPlayer::TitanExecution()
{
	m_Scene->GetResource()->SoundAllStop();

	CSound* Sound = m_Scene->GetResource()->FindSound("Release");

	Sound->Play();

	CArrow* Arrow = (CArrow*)m_Scene->FindObject("Arrow");
	Arrow->Destroy();

	PlayerAscent();
}

void CPlayer::TitanExecutionEnd()
{
	PlayerAscentEnd();

}

void CPlayer::PlayerAscent()
{
	m_IsAscent = true;
	m_Sprite->GetAnimation()->SetCurrentAnimation("AscentRoll");

}

void CPlayer::PlayerAscentRollCount()
{
	if (m_IsAscent) {
		if (m_AscentRollCount > 10) {
			m_Sprite->GetAnimation()->SetCurrentAnimation("AscentRoll_Mono");
		}
		else {
			m_Sprite->GetAnimation()->SetCurrentAnimation("AscentRoll");
		}
	}
}


void CPlayer::PlayerAscentEnd()
{
	// 타이탄이 죽고 나면, 머티리얼 컬러에 GRAY톤으로 설정되어 Execution 및 Ascent를 실행하기 때문에 Ascent가 마무리되고 난 후, Base컬러를 원색으로 되돌린다.
	if (strcmp("YetiScene", m_Scene->GetName().c_str()) == 0) {
		CTileMapComponent* tileComp = (CTileMapComponent*)m_Scene->FindObject("Yeti_TilemapObjLayer_1_BG1")->FindComponent("Yeti_TilemapCompLayer_1_BG1");
		tileComp->GetTileMaterial()->SetBaseColor(1, 1, 1, 1);

		CYeti* Yeti = (CYeti*)m_Scene->FindObject("Yeti");
		Yeti->Dead();

	}
	if (strcmp("StartingRuinScene", m_Scene->GetName().c_str()) == 0) {
		CTileMapComponent* tileComp = (CTileMapComponent*)m_Scene->FindObject("StartingRuins_TilemapObjLayer_1_BG1")->FindComponent("StartingRuins_TilemapCompLayer_1_BG1");
		tileComp->GetTileMaterial()->SetBaseColor(1, 1, 1, 1);

		CGoliath* Goliath = (CGoliath*)m_Scene->FindObject("Colossus");
		Goliath->Dead();
	}
}

void CPlayer::AnimAscentRollEnd()
{
	m_AscentRollCount++;
	PlayerAscentRollCount();
}

void CPlayer::AnimAscentRollMonoEnd()
{
	m_Sprite->GetAnimation()->SetCurrentAnimation("AscentFall_Mono");

	m_IsAscentFall = true;
	m_IsAscent = false;
}

void CPlayer::AnimAscentFallEnd()
{
	m_AscentPlayerPosY = 0.f;
	m_Direction = EDirection::Down;
	m_IsAscentFall = false;
	TitanExecutionEnd();
}

void CPlayer::AnimAscentFallMonoEnd()
{
	m_Sprite->GetAnimation()->SetCurrentAnimation("AscentFall");
}

void CPlayer::StepSoundStop()
{
	m_Scene->GetResource()->FindSound("WaterStep")->Stop();
	m_Scene->GetResource()->FindSound("VineStep")->Stop();
	m_Scene->GetResource()->FindSound("StoneStep")->Stop();
}

void CPlayer::InvincibleMode()
{
	if (m_DebugInvincible)
		m_DebugInvincible = false;
	else
		m_DebugInvincible = true;

}

void CPlayer::Test1()
{


}

void CPlayer::Test2()
{
	//const Vector3& CameraPos = m_Arm->GetWorldPos();
	//const Vector3& MyPos = GetWorldPos();

	//m_Camera->SetWorldPosition(CameraPos);
	PlayerAscent();

}


void CPlayer::DestroyArrow()
{
	m_ArrowExist = false;
}

std::string& CPlayer::GetPlayerState() const
{
	std::string stdState = "";

	switch (m_PlayerState)
	{
	case EPlayerState::Idle:
		stdState = "Idle";
		break;
	case EPlayerState::Aiming:
		stdState = "Aiming";
		break;
	case EPlayerState::Calling:
		stdState = "Calling";
		break;
	case EPlayerState::Walking:
		stdState = "Walking";
		break;
	case EPlayerState::Dash:
		stdState = "Dash";
		break;
	case EPlayerState::Dead:
		stdState = "Dead";
		break;
	}

	return stdState;
}

void CPlayer::MoveToPortal(const Vector3& StartPos, const EDirection& Direction)
{
	// 포탈 이동이 실행되었다면, 충돌에 대한 처리를 한 번만 하기 위한 플래그 삽입
	if (m_MoveToPortal)
		return;

	SetWorldPosition(StartPos);
	m_Direction = Direction;

	m_PlayerState = EPlayerState::Walking;
	m_Direction = Direction;

	m_MoveToPortal = true;
}

void CPlayer::SetPlayerDead()
{
	if (m_DebugInvincible)
		return;

	if (m_PlayerState == EPlayerState::Dead)
		return;

	CGameDefaultUI* GameDefaultUI = (CGameDefaultUI*) m_Scene->GetViewport()->FindUIWindow<CGameDefaultUI>("GameDefaultUI");
	GameDefaultUI->DeadUIOpen();

	PlayerDead();
}

void CPlayer::ShakeCamera(float magnitude, float ShakeTime)
{
	m_ShakeMagnitude = magnitude;
	m_ShakeTime = ShakeTime;
	m_ShakeCamera = true;
	m_ShakeTimer = 0.f;
}
