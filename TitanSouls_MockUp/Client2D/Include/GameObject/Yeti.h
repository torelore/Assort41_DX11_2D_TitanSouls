#pragma once

#include "GameObject/GameObject.h"

class CYeti :
	public CGameObject
{
	friend class CScene;

protected:
	CYeti();
	CYeti(const CYeti& Obj);
	virtual ~CYeti();

private:
	CSharedPtr<class CSpriteComponent>	m_Sprite;
	CSharedPtr<class CColliderSphere2D>	m_Body;
	CSharedPtr<class CColliderOBB2D>	m_LeftBody;
	CSharedPtr<class CColliderOBB2D>	m_RightBody;
	CSharedPtr<class CColliderOBB2D>	m_BottomBody;
	CSharedPtr<class CColliderOBB2D>	m_TopBody;
	CSharedPtr<class CColliderOBB2D>	m_BodyWeakPoint;

private:
	float m_timer;

	int m_ShootCount;
	int m_RollCount;

	bool m_IsAttacked;
	bool m_IsAttackStart;
	bool m_IsRollNow;
	bool m_IsDead;

	EDirection m_Direction;

	bool m_RollAfter1;
	bool m_RollAfter2;

	std::vector<Vector3> m_vecIciclePoint;

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual CYeti* Clone()    const;
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);

private:
	void CollisionYetiBody(const CollisionResult& result);
	void CollisionYetiWeak(const CollisionResult& result);

private :
	void Shoot();
	void ShootEnd();
	void Roll();
	void RollEnd();
	void DropIcicle();
	void Rest();
	void RestEnd();

public:
	void Dead();

private :
	void RollAfter1();
	void RollAfter2();

};

