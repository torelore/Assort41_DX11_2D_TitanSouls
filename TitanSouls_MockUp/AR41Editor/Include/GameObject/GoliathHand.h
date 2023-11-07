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

	float		m_Speed;	// �����ӿ� ���� �ӵ��� �����ϱ� ���� ����
	EHandLR		m_LR;		// �޼�����, ���������� üũ(�÷��̾ ���� ���� �������� �޼� �������� ����)
	bool		m_IsLand;	// ���� ���� ������ �÷��̾� ������ üũ��
	int			m_AttackCount; // ���� Ƚ�� üũ�� ����, 3ȸ���� ���� ������ ����
	bool		m_IsAttack;	// �������� ������ üũ�ϱ� ���� ����, ���� ���� �ƴ� ������ �� �Ѵ�.
	bool		m_IsDead;	// ������Ʈ�� ��� �������� üũ�ϱ� ���� ����

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

