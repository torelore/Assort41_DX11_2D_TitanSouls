
#include "TargetArm.h"

CTargetArm::CTargetArm() :
	m_TargetDistance(0.f)
{
	SetTypeID<CTargetArm>();

	m_ComponentTypeName = "TargetArm";
}

CTargetArm::CTargetArm(const CTargetArm& component) :
	CSceneComponent(component)
{
	m_TargetDistance = component.m_TargetDistance;
	m_TargetOffset = component.m_TargetOffset;
	m_TargetDistanceAxis = component.m_TargetDistanceAxis;
}

CTargetArm::~CTargetArm()
{
}

void CTargetArm::Destroy()
{
	CSceneComponent::Destroy();
}

void CTargetArm::Start()
{
	CSceneComponent::Start();
}

bool CTargetArm::Init()
{
	CSceneComponent::Init();

	return true;
}

void CTargetArm::Update(float DeltaTime)
{
	CSceneComponent::Update(DeltaTime);

	if (m_Parent)
	{
		Vector3	ParentPos = m_Parent->GetWorldPos();

		Vector3 Pos = ParentPos - GetWorldAxis(m_TargetDistanceAxis) * m_TargetDistance;

		SetWorldPosition(Pos + m_TargetOffset);
	}
}

void CTargetArm::PostUpdate(float DeltaTime)
{
	CSceneComponent::PostUpdate(DeltaTime);
}

void CTargetArm::Render()
{
	CSceneComponent::Render();
}

CTargetArm* CTargetArm::Clone() const
{
	return new CTargetArm(*this);
}

void CTargetArm::Save(FILE* File)
{
	CSceneComponent::Save(File);

	fwrite(&m_TargetOffset, sizeof(Vector3), 1, File);
	fwrite(&m_TargetDistance, sizeof(float), 1, File);
	fwrite(&m_TargetDistanceAxis, sizeof(AXIS), 1, File);
}

void CTargetArm::Load(FILE* File)
{
	CSceneComponent::Load(File);

	fread(&m_TargetOffset, sizeof(Vector3), 1, File);
	fread(&m_TargetDistance, sizeof(float), 1, File);
	fread(&m_TargetDistanceAxis, sizeof(AXIS), 1, File);
}
