#pragma once

#include "GameObject/GameObject.h"

class CSludgeheart :
	public CGameObject
{
	friend class CScene;

protected:
	CSludgeheart();
	CSludgeheart(const CSludgeheart& Obj);
	virtual ~CSludgeheart();

private:
	CSharedPtr<class CSpriteComponent>	m_Sprite;
	CSharedPtr<class CColliderSphere2D>	m_Body;

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual CSludgeheart* Clone()    const;
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);
	
private :
	void CollisionSludgeheart(const CollisionResult& result);
};