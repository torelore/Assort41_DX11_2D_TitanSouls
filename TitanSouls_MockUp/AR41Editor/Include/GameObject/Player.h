#pragma once

#include "GameObject/GameObject.h"

class CPlayer :
	public CGameObject
{
	friend class CScene;

protected:
	CPlayer();
	CPlayer(const CPlayer& Obj);
	virtual ~CPlayer();

private:
	CSharedPtr<class CSpriteComponent>	m_Sprite;
	CSharedPtr<class CCameraComponent>	m_Camera;
	CSharedPtr<class CTargetArm>		m_Arm;
	CSharedPtr<class CColliderOBB2D>	m_Body;
	// CSharedPtr<class CNavigationAgent>	m_NavAgent; 길찾기 에이전트를 활용한 길찾기를 할 필요 없음

	float m_Speed;
	float m_BodySize;
	float m_AimTimer;
	float m_Timer;
	float m_ShakeMagnitude;
	float m_ShakeTime;
	float m_ShakeTimer;

	int m_AscentRollCount;

	float m_AscentPlayerPosY;


	EDirection m_Direction;
	EPlayerState m_PlayerState;
	
	bool m_ArrowExist;
	bool m_ArrowOnHand;
	bool m_ArrowCalling;

	bool m_MoveToPortal;
	bool m_ShakeCamera;

	bool m_IsAscent;
	bool m_IsAscentFall;

	bool m_DebugInvincible;

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual CPlayer* Clone()    const;
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);

private:
	void MoveUp();
	void MoveDown();
	void MoveLeft();
	void MoveRight();
	void MoveEnd();

	void Dash();
	void DashEnd();
	
	void ArrowAim();
	void ArrowShoot();
	void ArrowCall();
	void ArrowCallStop();

	void PlayerDead();

	void TitanExecution();
	void TitanExecutionEnd();

	void PlayerAscent();
	void PlayerAscentRollCount();
	void PlayerAscentEnd();

	void AnimAscentRollEnd();
	void AnimAscentRollMonoEnd();
	void AnimAscentFallEnd();
	void AnimAscentFallMonoEnd();

	void StepSoundStop();

	void InvincibleMode();


	void Test1();
	void Test2();


public :
	void DestroyArrow();
	bool GetArrowExist() const { return m_ArrowExist; }
	bool GetArrowOnHand() const { return m_ArrowOnHand; }
	bool GetMoveToPortal() const { return m_MoveToPortal; }
	bool GetDebugInvincible() const	{ return m_DebugInvincible; }
	float GetAimTimer() const { return m_AimTimer; }
	float GetBodySize() const { return m_BodySize; }
	
	std::string& GetPlayerState() const;

	void SetDirection(EDirection Direction) { m_Direction = Direction; }

	void MoveToPortal(const Vector3& StartPos, const EDirection& Direction);
	void SetPlayerDead();

	void ShakeCamera(float magnitude = 2.f, float ShakeTime = 0.5f);
};

