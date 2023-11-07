#pragma once

#include "GameObject/GameObject.h"

class CPortal :
	public CGameObject
{
	friend class CScene;

protected:
	CPortal();
	CPortal(const CPortal& Obj);
	virtual ~CPortal();

private:
	CSharedPtr<class CColliderOBB2D>	m_Body;

private:
	ESceneList		m_StartScene;
	ESceneList		m_DestScene;
	EDirection		m_ToDirection;
	bool			m_IsOpen;

public:
	void SetCollisionProfileName(const std::string& Name);

	void SetStartScene(ESceneList StartScene) {
		m_StartScene = StartScene;
	}

	void SetPortalDest(ESceneList DestScene, EDirection PortalToDirection = EDirection::Up) {
		m_DestScene = DestScene;
		m_ToDirection = PortalToDirection;
	}

	void SetPortalDirection(EDirection ToDirection) {
		m_ToDirection = ToDirection;
	}

	void SetPortalSize(const Vector2& Size);

	void PortalOpen() { m_IsOpen = true; }
	void PortalClose() { m_IsOpen = false; }

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual CPortal* Clone()    const;
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);


private:
	void CollisionPortal(const CollisionResult& result);

public :
	void CreateNextScene();
};

