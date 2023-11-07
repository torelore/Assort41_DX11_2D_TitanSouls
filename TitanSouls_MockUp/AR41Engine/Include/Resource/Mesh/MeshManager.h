#pragma once

#include "../../EngineInfo.h"

class CMeshManager
{
	friend class CResourceManager;

private:
	CMeshManager();
	~CMeshManager();

private:
	std::unordered_map<std::string, CSharedPtr<class CMesh>>	m_mapMesh;

public:
	bool Init();
	bool CreateMesh(class CScene* Scene, MeshType Type, const std::string& Name, 
		void* VtxData, int Size, int Count,
		D3D11_USAGE VtxUsage, D3D11_PRIMITIVE_TOPOLOGY Primitive,
		void* IdxData = nullptr, int IdxSize = 0, int IdxCount = 0,
		D3D11_USAGE IdxUsage = D3D11_USAGE_DEFAULT,
		DXGI_FORMAT Fmt = DXGI_FORMAT_UNKNOWN);

	class CMesh* FindMesh(const std::string& Name);
	void ReleaseMesh(const std::string& Name);
};

