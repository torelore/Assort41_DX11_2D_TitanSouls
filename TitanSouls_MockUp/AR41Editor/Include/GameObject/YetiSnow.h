#pragma once

#include "GameObject/GameObject.h"

class CYetiSnow
	: public CGameObject
{
	friend class CScene;

protected:
	CYetiSnow();
	CYetiSnow(const CYetiSnow& Obj);
	virtual ~CYetiSnow();

private:
	CSharedPtr<class CSpriteComponent>	m_Sprite;
	CSharedPtr<class CColliderSphere2D>	m_Body;

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual CYetiSnow* Clone()    const;

private:
	void CollisionYetiSnow(const CollisionResult& result);
};


