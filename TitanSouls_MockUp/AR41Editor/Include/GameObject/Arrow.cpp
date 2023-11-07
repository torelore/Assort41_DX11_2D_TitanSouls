#include "Arrow.h"

#include "Animation/Animation2D.h"

#include "Component/ColliderOBB2D.h"
#include "Component/ColliderSphere2D.h"
#include "Component/SpriteComponent.h"
#include "Component/TileMapComponent.h"

#include "Resource/ResourceManager.h"

#include "Scene/Scene.h"
#include "Player.h"
#include "GoliathHand.h"



// debug
#include <atlstr.h>

CArrow::CArrow()
{
	SetTypeID<CArrow>();

	m_ObjectTypeName = "Arrow";
	m_Speed = 0.f;
	m_Accel = 0.f;
	m_isReflect = false;
	m_isWeakHit = false;
}

CArrow::CArrow(const CArrow& Obj) :
	CGameObject(Obj)
{
	m_Sprite = (CSpriteComponent*)FindComponent("ArrowSprite");
	m_ArrowHead = (CColliderOBB2D*)FindComponent("ArrowHead");
	m_ArrowBody = (CColliderOBB2D*)FindComponent("ArrowBody");
	m_Speed = 0.f;
	m_Accel = 0.f;
}

CArrow::~CArrow()
{
}

void CArrow::Start()
{
	CGameObject::Start();
}

bool CArrow::Init()
{
	CGameObject::Init();

	m_ArrowHead = CreateComponent<CColliderOBB2D>("ArrowHead");
	m_ArrowBody = CreateComponent<CColliderOBB2D>("ArrowBody");
	m_Sprite = CreateComponent<CSpriteComponent>("ArrowSprite");

	SetRootComponent(m_ArrowHead);

	m_ArrowHead->AddChild(m_Sprite);
	m_ArrowHead->AddChild(m_ArrowBody);


	m_ArrowHead->SetCollisionCallback<CArrow>(ECollision_Result::Collision, this, &CArrow::CollisionArrowHead);
	m_ArrowHead->SetBoxHalfSize(3.f, 3.f);
	m_ArrowHead->SetCollisionProfile("ArrowHead");
	m_ArrowHead->SetRelativePositionY(m_ArrowHead->GetRelativePos().y - 6.f);


	m_ArrowBody->SetCollisionCallback<CArrow>(ECollision_Result::Collision, this, &CArrow::CollisionArrowBody);
	m_ArrowBody->SetBoxHalfSize(10.f, 10.f);
	m_ArrowBody->SetCollisionProfile("ArrowBody");
	m_ArrowBody->SetRelativePositionY(m_ArrowBody->GetRelativePos().y + 6.f);



	m_Sprite->SetTexture("ArrowSprite", TEXT("IMG/player.png"));
	m_Sprite->SetPivot(0.5f, 0.5f);
	m_Sprite->SetRelativeScale(25.f, 25.f);
	m_Sprite->SetInheritRotZ(true);



	// Arrow
	{
		CResourceManager::GetInst()->CreateAnimationSequence2D("ArrowIdle", "ArrowSprite", TEXT("IMG/player.png"));
		CResourceManager::GetInst()->AddAnimationSequence2DFrame("ArrowIdle", Vector2(512.f, 16.f), Vector2(496.f, 0.f));

		CResourceManager::GetInst()->CreateAnimationSequence2D("ArrowEntered", "ArrowSprite", TEXT("IMG/player.png"));
		CResourceManager::GetInst()->AddAnimationSequence2DFrame("ArrowEntered", Vector2(496.f, 32.f), Vector2(480.f, 16.f));

	}


	CAnimation2D* Anim = m_Sprite->SetAnimation<CAnimation2D>("ArrowAnim");
	Anim->AddAnimation("ArrowIdle", "ArrowIdle");
	Anim->AddAnimation("ArrowEntered", "ArrowEntered");
	Anim->SetCurrentAnimation("ArrowIdle");


	return true;
}

void CArrow::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

	// ȭ���� �ִ� Ÿ�ϸ��� ����(������Ʈ���� �տ� �ִ� ���)���� üũ�Ͽ� basecolor�� ����
	CTileMapComponent* RootTilemapBackground = (CTileMapComponent*)m_Scene->FindObject("TilemapObjLayer_Root_FG")->FindComponent("TilemapCompLayer_Root_FG");
	ETileOption tileOption = RootTilemapBackground->GetTileOption(GetWorldPos());

	if (tileOption == ETileOption::FG)
		m_Sprite->GetMaterial(0)->SetBaseColor(0, 0, 0, 0);
	else
		m_Sprite->GetMaterial(0)->SetBaseColor(1, 1, 1, 1);


	
	// ȭ���� �� �� ���� ���ǵ忡 ������ ���Ѵ�. 
	// �ӵ��� 0�� �Ǹ� ȭ���� �ڵ������� ���߰�, ���� �ݻ�Ǵ� �����̸� �ӵ��� 0�� �� ������ �ݻ簡 �ƴϰ� �ȴ�.
	if (m_Speed > 0) {
		m_Speed += m_Accel;
	}
	else {
		m_Speed = 0.f;
		m_isReflect = false;
	}


	// ���浹 ���� �ݻ簢 ó�� �̽���, �Ի簢 �״�� �ݻ�ǵ��� ó��
	if (m_isReflect) {
		AddWorldPosition(GetWorldAxis(AXIS_Y) * -m_Speed * DeltaTime);
	}
	else {
		// ȭ���� ��ų� ȸ���� ��, �߻簢�� �����ޱ� ������ �ش� Y������ ���ǵ忡 ���� ������ �Ѵ�.
		AddWorldPosition(GetWorldAxis(AXIS_Y) * m_Speed * DeltaTime);
	}

	// ȭ���� ���� ������� üũ�ϱ� ���� Ÿ�ϸ� �˻�
	RootTilemapBackground = (CTileMapComponent*)m_Scene->FindObject("TilemapObjLayer_Root")->FindComponent("TilemapCompLayer_Root");

	// ��� ������ ���� �������� üũ�ϱ� ���� 4���� �˻�.
	{
		// ���� ������ ������ ��
		float size = 5.f;

		// �������� ����� ��.
		const Vector3& UpPos = Vector3(GetWorldPos().x, GetWorldPos().y + size, GetWorldPos().z);
		tileOption = RootTilemapBackground->GetTileOption(UpPos);

		if (tileOption == ETileOption::Wall) {
			
			float ReflectRot = GetWorldRot().z;

			if (ReflectRot <= 0.f && ReflectRot >= -90.f) {
				ReflectRot -= 90.f;
			}
			else if (ReflectRot <= -270.f && ReflectRot >= -360.f) {
				ReflectRot += 90.f;
			}

			SetWorldRotationZ(ReflectRot);
			m_Scene->GetResource()->FindSound("ArrowImpact")->Play();

			return;
		}

		// �Ʒ������� ����� ��
		const Vector3& DownPos = Vector3(GetWorldPos().x, GetWorldPos().y - size, GetWorldPos().z);
		tileOption = RootTilemapBackground->GetTileOption(DownPos);

		if (tileOption == ETileOption::Wall) {

			float ReflectRot = GetWorldRot().z;

			if (ReflectRot <= -90.f && ReflectRot >= -180.f) {
				ReflectRot += 90.f;
			}
			else if (ReflectRot <= -180.f && ReflectRot >= -270.f) {
				ReflectRot -= 90.f;
			}

			SetWorldRotationZ(ReflectRot);
			m_Scene->GetResource()->FindSound("ArrowImpact")->Play();

			return;
		}


		const Vector3& LeftPos = Vector3(GetWorldPos().x - size, GetWorldPos().y, GetWorldPos().z);
		tileOption = RootTilemapBackground->GetTileOption(LeftPos);

		if (tileOption == ETileOption::Wall) {

			float ReflectRot = GetWorldRot().z;

			if (ReflectRot <= -270.f && ReflectRot >= -360.f) {
				ReflectRot -= 90.f;

				if (ReflectRot < -360.f) {
					ReflectRot += 360.f;
				}
			}
			else if (ReflectRot <= -180.f && ReflectRot >= -270.f) {
				ReflectRot += 90.f;
			}

			SetWorldRotationZ(ReflectRot);
			m_Scene->GetResource()->FindSound("ArrowImpact")->Play();

			return;
		}


		const Vector3& RightPos = Vector3(GetWorldPos().x + size, GetWorldPos().y, GetWorldPos().z);
		tileOption = RootTilemapBackground->GetTileOption(RightPos);

		if (tileOption == ETileOption::Wall) {

			float ReflectRot = GetWorldRot().z;

			if (ReflectRot <= 0.f && ReflectRot >= -90.f) {
				ReflectRot += 90.f;

				if (ReflectRot > 0.f) {
					ReflectRot -= 360.f;
				}

			}
			else if (ReflectRot <= -90.f && ReflectRot >= -180.f) {
				ReflectRot -= 90.f;
			}

			SetWorldRotationZ(ReflectRot);
			m_Scene->GetResource()->FindSound("ArrowImpact")->Play();

			return;
		}
	}


}

void CArrow::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

CArrow* CArrow::Clone() const
{
	return new CArrow(*this);
}

void CArrow::ArrowStop()
{
	m_Speed = 0.f;
	m_Sprite->GetAnimation()->SetCurrentAnimation("ArrowEntered");
	m_isWeakHit = true;
}

void CArrow::CollisionArrowHead(const CollisionResult& result)
{
	CCollider* dest = m_ArrowHead->GetColliderDest();

	if (dest == nullptr)
		return;


	const std::string& Name = dest->GetOwner()->GetName();

	// �ݷμ����� ���� �ٴڿ� ���� ������ �浹 ó��
	if (strcmp(Name.c_str(), "Colossus_LeftHand") == 0) {

		CGoliathHand* Hand = (CGoliathHand*) m_Scene->FindObject(Name);

		if(!Hand->IsLand())
			return;
	}

	if (strcmp(Name.c_str(), "Colossus_RightHand") == 0){

		CGoliathHand* Hand = (CGoliathHand*)m_Scene->FindObject(Name);

		if (!Hand->IsLand())
			return;
	}


	// �ӵ��� 0�϶����� �浹 x
	if (m_Speed <= 0.f)
		return;


	m_Scene->GetResource()->FindSound("ArrowImpact")->Play();



	// ���Ϳ� �ε����� �� �ݻ簢�� ����� �ݻ��ϰԲ� �Ѵ�.
	{
		const Vector3& DestPos = m_Scene->FindObject(Name)->GetWorldPos();
		const Vector3& MyPos = GetWorldPos();

		std::list<CSceneComponent*> listComponent = m_Scene->FindObject(Name)->GetSceneComponents();

		for (auto iter : listComponent) {
			if (typeid(CColliderOBB2D).hash_code() == iter->GetTypeID()) {

				m_isReflect = true;
				//CColliderOBB2D* collider = (CColliderOBB2D*)iter;
				//const Vector2& size = collider->GetBoxHalfSize();

				//float LeftX = DestPos.x - size.x;
				//float RightX = DestPos.x + size.x;
				//float TopY = DestPos.y + size.y;
				//float BottomY = DestPos.y - size.y;


				//if (MyPos.x >= LeftX && MyPos.x <= RightX) {
				//	// ���
				//	if (MyPos.y >= TopY) {

				//		float ReflectRot = GetWorldRot().z;

				//		if (ReflectRot <= -90.f && ReflectRot >= -180.f) {
				//			ReflectRot += 90.f;
				//		}
				//		else if (ReflectRot <= -180.f && ReflectRot >= -270.f) {
				//			ReflectRot -= 90.f;
				//		}

				//		SetWorldRotationZ(ReflectRot);
				//	}


				//	// �ϴ�
				//	if (MyPos.y <= BottomY) {

				//		float ReflectRot = GetWorldRot().z;

				//		if (ReflectRot <= 0.f && ReflectRot >= -90.f) {
				//			ReflectRot -= 90.f;
				//		}
				//		else if (ReflectRot <= -270.f && ReflectRot >= -360.f) {
				//			ReflectRot += 90.f;
				//		}

				//		SetWorldRotationZ(ReflectRot);
				//	}
				//}

				//if (MyPos.y >= BottomY && MyPos.y <= TopY) {
				//	// ����
				//	if (MyPos.x <= LeftX) {

				//		float ReflectRot = GetWorldRot().z;

				//		if (ReflectRot <= 0.f && ReflectRot >= -90.f) {
				//			ReflectRot += 90.f;

				//			if (ReflectRot > 0.f) {
				//				ReflectRot -= 360.f;
				//			}

				//		}
				//		else if (ReflectRot <= -90.f && ReflectRot >= -180.f) {
				//			ReflectRot -= 90.f;
				//		}
				//		SetWorldRotationZ(ReflectRot);
				//	}

				//	// ����
				//	if (MyPos.x >= RightX) {

				//		float ReflectRot = GetWorldRot().z;

				//		if (ReflectRot <= -270.f && ReflectRot >= -360.f) {
				//			ReflectRot -= 90.f;

				//			if (ReflectRot < -360.f) {
				//				ReflectRot += 360.f;
				//			}
				//		}
				//		else if (ReflectRot <= -180.f && ReflectRot >= -270.f) {
				//			ReflectRot += 90.f;
				//		}

				//		SetWorldRotationZ(ReflectRot);
				//	}
				//}

				break;
			}

			if (typeid(CColliderSphere2D).hash_code() == iter->GetTypeID()) {
				CColliderSphere2D* collider = (CColliderSphere2D*)iter;
				
				if (iter->GetEnable())
					m_isReflect = true;


				break;
			}
		}
	}
}

void CArrow::CollisionArrowBody(const CollisionResult& result)
{
	CPlayer* Player = (CPlayer*)m_Scene->FindObject("Player");

	if (!Player)
		return;

	if (m_Speed == 0 && Player->GetArrowOnHand())
		return;

	Player->DestroyArrow();

	m_Scene->GetResource()->FindSound("Pickuparrow")->Play();

	Destroy();
}