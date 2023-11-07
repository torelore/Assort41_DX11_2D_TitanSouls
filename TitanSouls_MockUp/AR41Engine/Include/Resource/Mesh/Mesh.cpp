
#include "Mesh.h"
#include "../../Device.h"
#include "../Material/Material.h"
#include "../ResourceManager.h"
#include "../../Scene/Scene.h"
#include "../../Scene/SceneResource.h"

CMesh::CMesh()	:
	m_Min(FLT_MAX, FLT_MAX, FLT_MAX),
	m_Max(FLT_MIN, FLT_MIN, FLT_MIN)
{
}

CMesh::~CMesh()
{
	size_t	Size = m_vecContainer.size();

	for (size_t i = 0; i < Size; ++i)
	{
		SAFE_DELETE(m_vecContainer[i]);
	}
	Size = m_vecMeshSlot.size();

	for (size_t i = 0; i < Size; ++i)
	{
		SAFE_DELETE(m_vecMeshSlot[i]);
	}
}

void CMesh::SetMaterial(int Container, int Subset, const std::string& Name)
{
	CMaterial* Material = nullptr;

	if (m_Scene)
	{
		Material = m_Scene->GetResource()->FindMaterial(Name);
	}

	else
	{
		Material = CResourceManager::GetInst()->FindMaterial(Name);
	}

	m_vecContainer[Container]->vecSubset[Subset].Material = Material;
	m_vecContainer[Container]->vecSubset[Subset].Slot->Material = Material;
}

void CMesh::SetMaterial(int Container, int Subset, CMaterial* Material)
{
	m_vecContainer[Container]->vecSubset[Subset].Material = Material;
	m_vecContainer[Container]->vecSubset[Subset].Slot->Material = Material;
}

bool CMesh::CreateMesh(void* VtxData, int Size, int Count,
	D3D11_USAGE VtxUsage, D3D11_PRIMITIVE_TOPOLOGY Primitive, 
	void* IdxData, int IdxSize, int IdxCount, D3D11_USAGE IdxUsage, 
	DXGI_FORMAT Fmt)
{
	MeshContainer* Container = new MeshContainer;

	Container->VB.Size = Size;
	Container->VB.Count = Count;
	Container->Primitive = Primitive;
	Container->VB.Data = new char[Size * Count];
	memcpy(Container->VB.Data, VtxData, Size * Count);

	if (!CreateBuffer(BufferType::Vertex, VtxData, Size, Count,
		VtxUsage, &Container->VB.Buffer))
		return false;

	m_vecContainer.push_back(Container);

	MeshSlot* Slot = new MeshSlot;

	m_vecMeshSlot.push_back(Slot);

	Slot->VB = &Container->VB;
	Slot->Primitive = Container->Primitive;

	if (IdxData != nullptr)
	{
		MeshSubset	Subset;
		Container->vecSubset.push_back(Subset);
		int Index = (int)Container->vecSubset.size() - 1;

		Container->vecSubset[Index].Slot = Slot;

		Slot->IB = &Container->vecSubset[Index].IB;

		Slot->IB->Size = IdxSize;
		Slot->IB->Count = IdxCount;
		Slot->IB->Fmt = Fmt;
		Slot->IB->Data = new char[IdxSize * IdxCount];
		memcpy(Slot->IB->Data, IdxData, IdxSize * IdxCount);

		if (!CreateBuffer(BufferType::Index, IdxData, IdxSize, IdxCount,
			IdxUsage, &Slot->IB->Buffer))
			return false;
	}

	return true;
}

void CMesh::Render()
{
	size_t	Size = m_vecMeshSlot.size();

	for(size_t i = 0; i < Size; ++i)
	{
		unsigned int	Stride = (unsigned int)m_vecMeshSlot[i]->VB->Size;
		unsigned int	Offset = 0;

		CDevice::GetInst()->GetContext()->IASetPrimitiveTopology(m_vecMeshSlot[i]->Primitive);
		CDevice::GetInst()->GetContext()->IASetVertexBuffers(0, 1,
			&m_vecMeshSlot[i]->VB->Buffer, &Stride, &Offset);

		if (m_vecMeshSlot[i]->IB)
		{
			CDevice::GetInst()->GetContext()->IASetIndexBuffer(
				m_vecMeshSlot[i]->IB->Buffer, m_vecMeshSlot[i]->IB->Fmt,
				0);
			CDevice::GetInst()->GetContext()->DrawIndexed(
				m_vecMeshSlot[i]->IB->Count, 0, 0);
		}

		else
		{
			CDevice::GetInst()->GetContext()->IASetIndexBuffer(
				nullptr, DXGI_FORMAT_UNKNOWN, 0);
			CDevice::GetInst()->GetContext()->Draw(
				m_vecMeshSlot[i]->VB->Count, 0);
		}
	}
}

void CMesh::Render(int SlotNumber)
{
	unsigned int	Stride = (unsigned int)m_vecMeshSlot[SlotNumber]->VB->Size;
	unsigned int	Offset = 0;

	CDevice::GetInst()->GetContext()->IASetPrimitiveTopology(m_vecMeshSlot[SlotNumber]->Primitive);
	CDevice::GetInst()->GetContext()->IASetVertexBuffers(0, 1,
		&m_vecMeshSlot[SlotNumber]->VB->Buffer, &Stride, &Offset);

	if (m_vecMeshSlot[SlotNumber]->IB)
	{
		CDevice::GetInst()->GetContext()->IASetIndexBuffer(
			m_vecMeshSlot[SlotNumber]->IB->Buffer, m_vecMeshSlot[SlotNumber]->IB->Fmt,
			0);
		CDevice::GetInst()->GetContext()->DrawIndexed(
			m_vecMeshSlot[SlotNumber]->IB->Count, 0, 0);
	}

	else
	{
		CDevice::GetInst()->GetContext()->IASetIndexBuffer(
			nullptr, DXGI_FORMAT_UNKNOWN, 0);
		CDevice::GetInst()->GetContext()->Draw(
			m_vecMeshSlot[SlotNumber]->VB->Count, 0);
	}
}

void CMesh::RenderInstancing(int Count)
{
	size_t	Size = m_vecMeshSlot.size();

	for (size_t i = 0; i < Size; ++i)
	{
		unsigned int	Stride = (unsigned int)m_vecMeshSlot[i]->VB->Size;
		unsigned int	Offset = 0;

		CDevice::GetInst()->GetContext()->IASetPrimitiveTopology(m_vecMeshSlot[i]->Primitive);
		CDevice::GetInst()->GetContext()->IASetVertexBuffers(0, 1,
			&m_vecMeshSlot[i]->VB->Buffer, &Stride, &Offset);

		if (m_vecMeshSlot[i]->IB)
		{
			CDevice::GetInst()->GetContext()->IASetIndexBuffer(
				m_vecMeshSlot[i]->IB->Buffer, m_vecMeshSlot[i]->IB->Fmt,
				0);
			CDevice::GetInst()->GetContext()->DrawIndexedInstanced(
				m_vecMeshSlot[i]->IB->Count, Count, 0, 0, 0);
		}

		else
		{
			CDevice::GetInst()->GetContext()->IASetIndexBuffer(
				nullptr, DXGI_FORMAT_UNKNOWN, 0);
			CDevice::GetInst()->GetContext()->DrawInstanced(
				m_vecMeshSlot[i]->VB->Count, Count, 0, 0);
		}
	}
}

void CMesh::RenderInstancing(int Count, int SlotNumber)
{
	unsigned int	Stride = (unsigned int)m_vecMeshSlot[SlotNumber]->VB->Size;
	unsigned int	Offset = 0;

	CDevice::GetInst()->GetContext()->IASetPrimitiveTopology(m_vecMeshSlot[SlotNumber]->Primitive);
	CDevice::GetInst()->GetContext()->IASetVertexBuffers(0, 1,
		&m_vecMeshSlot[SlotNumber]->VB->Buffer, &Stride, &Offset);

	if (m_vecMeshSlot[SlotNumber]->IB)
	{
		CDevice::GetInst()->GetContext()->IASetIndexBuffer(
			m_vecMeshSlot[SlotNumber]->IB->Buffer, m_vecMeshSlot[SlotNumber]->IB->Fmt,
			0);
		CDevice::GetInst()->GetContext()->DrawIndexedInstanced(
			m_vecMeshSlot[SlotNumber]->IB->Count, Count, 0, 0, 0);
	}

	else
	{
		CDevice::GetInst()->GetContext()->IASetIndexBuffer(
			nullptr, DXGI_FORMAT_UNKNOWN, 0);
		CDevice::GetInst()->GetContext()->DrawInstanced(
			m_vecMeshSlot[SlotNumber]->VB->Count, Count, 0, 0);
	}
}

bool CMesh::CreateBuffer(BufferType Type, void* Data, int Size, 
	int Count, D3D11_USAGE Usage, ID3D11Buffer** Buffer)
{
	D3D11_BUFFER_DESC	Desc = {};

	Desc.ByteWidth = Size * Count;
	Desc.Usage = Usage;

	if (Type == BufferType::Vertex)
		Desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	else
		Desc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	if (Usage == D3D11_USAGE_DYNAMIC)
		Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	else if (Usage == D3D11_USAGE_STAGING)
		Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;

	D3D11_SUBRESOURCE_DATA	BufferData = {};

	BufferData.pSysMem = Data;

	if (FAILED(CDevice::GetInst()->GetDevice()->CreateBuffer(&Desc, &BufferData,
		Buffer)))
		return false;

	if (Type == BufferType::Vertex)
	{
		// 정점은 위치 12바이트가 항상 먼저 들어와 있을 것이다.
		char* VertexData = (char*)Data;

		for (int i = 0; i < Count; ++i)
		{
			Vector3* Pos = (Vector3*)VertexData;
			VertexData += Size;

			if (m_Min.x > Pos->x)
				m_Min.x = Pos->x;

			if (m_Min.y > Pos->y)
				m_Min.y = Pos->y;

			if (m_Min.z > Pos->z)
				m_Min.z = Pos->z;

			if (m_Max.x < Pos->x)
				m_Max.x = Pos->x;

			if (m_Max.y < Pos->y)
				m_Max.y = Pos->y;

			if (m_Max.z < Pos->z)
				m_Max.z = Pos->z;
		}
	}

	return true;
}
