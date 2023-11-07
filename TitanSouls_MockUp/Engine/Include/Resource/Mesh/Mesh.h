#pragma once

#include "../../Ref.h"
#include "../Material/Material.h"

struct MeshSubset
{
	IndexBuffer IB;
	struct MeshSlot* Slot;
	CSharedPtr<CMaterial> Material;
};

struct MeshContainer
{
	VertexBuffer	VB;
	std::vector<MeshSubset>	vecSubset;

	// 위상구조. 어떤 도형으로 그려낼지를 결정한다.
	D3D11_PRIMITIVE_TOPOLOGY	Primitive;
};

struct MeshSlot
{
	VertexBuffer* VB;
	IndexBuffer* IB;
	CSharedPtr<class CMaterial>	Material;
	D3D11_PRIMITIVE_TOPOLOGY	Primitive;

	MeshSlot() :
		VB(nullptr),
		IB(nullptr)
	{
	}
};

class CMesh :
	public CRef
{
	friend class CMeshManager;

protected:
	CMesh();
	virtual ~CMesh() = 0;

protected:
	class CScene* m_Scene;

public:
	void SetScene(class CScene* Scene)
	{
		m_Scene = Scene;
	}

protected:
	std::vector<MeshContainer*> m_vecContainer;
	std::vector<MeshSlot*>      m_vecMeshSlot;

	Vector3 m_Min;
	Vector3 m_Max;
	MeshType    m_MeshType;

public:
	MeshType GetMeshType()  const
	{
		return m_MeshType;
	}

	const Vector3& GetMin() const
	{
		return m_Min;
	}

	const Vector3& GetMax() const
	{
		return m_Max;
	}

	Vector3 GetMeshSize()    const
	{
		return m_Max - m_Min;
	}

	int GetSlotCount()  const
	{
		return (int)m_vecMeshSlot.size();
	}

	CMaterial* GetMaterial(int Slot)	const
	{
		return m_vecMeshSlot[Slot]->Material;
	}

public:
	void SetMaterial(int Container, int Subset, const std::string& Name);
	void SetMaterial(int Container, int Subset, class CMaterial* Material);

public:
	virtual bool CreateMesh(void* VtxData, int Size, int Count,
		D3D11_USAGE VtxUsage, D3D11_PRIMITIVE_TOPOLOGY Primitive,
		void* IdxData = nullptr, int IdxSize = 0, int IdxCount = 0,
		D3D11_USAGE IdxUsage = D3D11_USAGE_DEFAULT,
		DXGI_FORMAT Fmt = DXGI_FORMAT_UNKNOWN);
	virtual void Render();
	virtual void Render(int SlotNumber);
	virtual void RenderInstancing(int Count);
	virtual void RenderInstancing(int Count, int SlotNumber);


protected:
	bool CreateBuffer(BufferType Type, void* Data, int Size,
		int Count, D3D11_USAGE Usage, ID3D11Buffer** Buffer);
};

