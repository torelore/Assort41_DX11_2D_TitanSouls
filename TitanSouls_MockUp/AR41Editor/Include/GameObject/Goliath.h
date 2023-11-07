#pragma once

#include "GameObject/GameObject.h"

class CGoliath :
	public CGameObject
{
	friend class CScene;

protected:
	CGoliath();
	CGoliath(const CGoliath& Obj);
	virtual ~CGoliath();

private:
	CSharedPtr<class CColliderOBB2D>	m_Body;
	CSharedPtr<class CSpriteComponent>	m_BodySprite;

	CSharedPtr<class CSceneComponent>	m_HeadChild;
	CSharedPtr<class CSpriteComponent>	m_HeadSprite;

	CSharedPtr<class CSceneComponent>	m_BodyPaintChild;
	CSharedPtr<class CSpriteComponent>	m_BodyPaintSprite;

	CSharedPtr<class CSceneComponent>	m_LeftSholderChild;
	CSharedPtr<class CSpriteComponent>	m_LeftSholderSprite;

	CSharedPtr<class CSceneComponent>	m_RightholderChild;
	CSharedPtr<class CSpriteComponent>	m_RightSholderSprite;

	CSharedPtr<class CColliderOBB2D>	m_BodyWeakPoint;

private :
	float m_Timer;

	bool m_IsAttacked;
	bool m_IsAttackStart;
	bool m_IsDead;

	EDirection m_Direction;

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual CGoliath* Clone()    const;
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);

private :
	void CollisionGoliathBody(const CollisionResult& result);
	void CollisionGoliathWeak(const CollisionResult& result);

private :
	void AttackStart();

public :
	void Dead();
};

