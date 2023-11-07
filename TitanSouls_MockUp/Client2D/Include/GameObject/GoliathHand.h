#pragma once

#include "GameObject/GameObject.h"

enum class EHandLR {
	Left,
	Right
};

class CGoliathHand
	: public CGameObject
{
	friend class CScene;

protected:
	CGoliathHand();
	CGoliathHand(const CGoliathHand& Obj);
	virtual ~CGoliathHand();

private:
	CSharedPtr<class CColliderOBB2D>	m_Body;
	CSharedPtr<class CSpriteComponent>	m_Sprite;

	CSharedPtr<class CSceneComponent>	m_Child;
	CSharedPtr<class CSpriteComponent>	m_SpriteChild;

	float		m_Speed;	// 움직임에 대한 속도를 조정하기 위한 변수
	EHandLR		m_LR;		// 왼손인지, 오른손인지 체크(플레이어가 보는 방향 기준으로 왼손 오른손을 나눔)
	bool		m_IsLand;	// 땅에 있을 때에만 플레이어 공격을 체크함
	int			m_AttackCount; // 공격 횟수 체크용 변수, 3회마다 다음 손으로 공격
	bool		m_IsAttack;	// 공격중인 손인지 체크하기 위한 변수, 공격 중이 아닐 때에는 방어를 한다.
	bool		m_IsDead;	// 오브젝트가 사망 상태인지 체크하기 위한 변수

	bool		m_GS;
	bool		m_RtM;
	Vector2		m_DestPos;
	bool		m_MtP;
	bool		m_AtP;
	bool		m_MtC;

	float		m_AttackPosY;


public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual CGoliathHand* Clone()    const;
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);

public:
	void SetHandLeft();
	void SetHandRight();
	bool IsLand() const {
		return m_IsLand;
	}

private:
	void CollisionHand(const CollisionResult& result);

public :
	void GoliathStart();
	void AttackStart();
	void GoliathStop();
	void GoliathDead();

private :
	void ReadyToMove();
	void ReadyToMoveEnd();
	void MoveToPlayer();
	void MoveToPlayerEnd();
	void AttackToPlayer();
	void AttackToPlayerEnd();
	void MoveToColossus();
	void MoveToColossusEnd();
	void Guard();
	void Dead();
};

