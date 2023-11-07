#pragma once

#include "GameObject/GameObject.h"

class CGoliathDoor
	: public CGameObject
{
	friend class CScene;

protected:
	CGoliathDoor();
	CGoliathDoor(const CGoliathDoor& Obj);
	virtual ~CGoliathDoor();

private:
	CSharedPtr<class CSpriteComponent>	m_Sprite;
	CSharedPtr<class CColliderOBB2D>	m_Body;

private :
	float m_Timer;

	bool m_IsFirstContact;
	bool m_IsBossStart;
	bool m_IsBossEnd;

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual CGoliathDoor* Clone()    const;

private:
	void CollisionDoor(const CollisionResult& result);

public :
	void BossStart() { m_IsBossStart = true; }
	void BossEnd() { m_IsBossEnd = true; }
};

