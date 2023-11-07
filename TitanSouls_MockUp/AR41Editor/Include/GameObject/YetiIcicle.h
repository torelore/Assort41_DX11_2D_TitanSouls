#pragma once

#include "GameObject/GameObject.h"

class CYetiIcicle
	: public CGameObject
{
	friend class CScene;

protected:
	CYetiIcicle();
	CYetiIcicle(const CYetiIcicle& Obj);
	virtual ~CYetiIcicle();

private:
	CSharedPtr<class CSpriteComponent>	m_Sprite;
	CSharedPtr<class CColliderOBB2D>	m_Body;

private :
	float m_Speed;
	bool m_IsLand;
	Vector3 m_DestPos;


public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual CYetiIcicle* Clone()    const;

public :
	void SetDestPos(const Vector3& Dest, float FallHeight = 400.f) {
		m_DestPos = Dest;

		SetWorldPosition(Dest.x, Dest.y + FallHeight);
	}
	
	bool IsLand() const {
		return m_IsLand;
	}

private:
	void CollisionYetiIcicle(const CollisionResult& result);
};


