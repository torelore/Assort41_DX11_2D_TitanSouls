#pragma once

#include "GameObject/GameObject.h"

class CKnightElahan :
	public CGameObject
{
	friend class CScene;

protected:
	CKnightElahan();
	CKnightElahan(const CKnightElahan& Obj);
	virtual ~CKnightElahan();

private:
	CSharedPtr<class CSpriteComponent>	m_Sprite;
	CSharedPtr<class CColliderOBB2D>	m_Body;

private:
	float m_timer;

	bool m_IsAttacked;
	bool m_IsAttackStart;
	bool m_IsDead;

	EDirection m_Direction;


public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual CKnightElahan* Clone()    const;
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);

private:
	void CollisionKnightBody(const CollisionResult& result);
};