#pragma once

#include "GameObject/GameObject.h"

class CArrow
	: public CGameObject
{
	friend class CScene;

protected:
	CArrow();
	CArrow(const CArrow& Obj);
	virtual ~CArrow();

private:
	CSharedPtr<class CSpriteComponent>	m_Sprite;
	CSharedPtr<class CColliderOBB2D>	m_ArrowHead;
	CSharedPtr<class CColliderOBB2D>	m_ArrowBody;

	float	m_Speed;
	float	m_Accel;
	bool	m_isReflect;
	bool	m_isWeakHit;

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual CArrow* Clone()    const;

public :
	void SetSpeed(float speed) { m_Speed = speed; }
	void SetAccel(float Accel) { m_Accel = Accel; }
	void SetWeakHit() { m_isWeakHit = true; }

	float GetSpeed() const { return m_Speed; }
	float GetAccel() const { return m_Accel; }

	bool IsMove() const { return m_Speed > 0; }
	bool IsReflect() const { return m_isReflect; }
	bool IsWeakHit() const { return m_isWeakHit; }

	void ArrowStop();

private:
	void CollisionArrowHead(const CollisionResult& result);
	void CollisionArrowBody(const CollisionResult& result);
};

