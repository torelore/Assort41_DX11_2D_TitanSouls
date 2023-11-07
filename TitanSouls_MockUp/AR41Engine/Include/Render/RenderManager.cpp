
#include "RenderManager.h"
#include "../Component/SceneComponent.h"
#include "RenderState.h"
#include "../Scene/SceneManager.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneViewport.h"
#include "BlendState.h"
#include "DepthStencilState.h"

DEFINITION_SINGLE(CRenderManager)

CRenderManager::CRenderManager()	:
	m_RenderStateManager(nullptr)
{
}

CRenderManager::~CRenderManager()
{
	auto	iter = m_RenderLayerList.begin();
	auto	iterEnd = m_RenderLayerList.end();

	for (; iter != iterEnd; ++iter)
	{
		SAFE_DELETE((*iter));
	}

	SAFE_DELETE(m_RenderStateManager);
}

void CRenderManager::CreateLayer(const std::string& Name, int Priority)
{
	RenderLayer* Layer = new RenderLayer;

	Layer->Name = Name;
	Layer->LayerPriority = Priority;

	m_RenderLayerList.push_back(Layer);

	std::sort(m_RenderLayerList.begin(), m_RenderLayerList.end(), CRenderManager::SortLayer);
}

void CRenderManager::SetLayerPriority(const std::string& Name, int Priority)
{
	auto	iter = m_RenderLayerList.begin();
	auto	iterEnd = m_RenderLayerList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->Name == Name)
		{
			(*iter)->LayerPriority = Priority;
			break;
		}
	}

	std::sort(m_RenderLayerList.begin(), m_RenderLayerList.end(), CRenderManager::SortLayer);
}

void CRenderManager::SetLayerAlphaBlend(const std::string& Name)
{
	auto	iter = m_RenderLayerList.begin();
	auto	iterEnd = m_RenderLayerList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->Name == Name)
		{
			(*iter)->AlphaBlend = FindRenderState<CRenderState>("AlphaBlend");
			break;
		}
	}
}

void CRenderManager::DeleteLayer(const std::string& Name)
{
	auto	iter = m_RenderLayerList.begin();
	auto	iterEnd = m_RenderLayerList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->Name == Name)
		{
			SAFE_DELETE((*iter));
			iter = m_RenderLayerList.erase(iter);
			break;
		}
	}
}

void CRenderManager::AddRenderList(CSceneComponent* Component)
{
	auto	iter = m_RenderLayerList.begin();
	auto	iterEnd = m_RenderLayerList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->Name == Component->GetRenderLayerName())
		{
			(*iter)->RenderList.push_back(Component);
			break;
		}
	}
}

bool CRenderManager::Init()
{
	m_RenderStateManager = new CRenderStateManager;

	if (!m_RenderStateManager->Init())
		return false;

	CreateLayer("Default", 1);
	CreateLayer("Back", 0);

	SetLayerAlphaBlend("Default");

	m_AlphaBlend = m_RenderStateManager->FindRenderState<CBlendState>("AlphaBlend");
	m_DepthDisable = m_RenderStateManager->FindRenderState<CDepthStencilState>("DepthDisable");

	return true;
}

void CRenderManager::Render(float DeltaTime)
{
	{
		auto	iter = m_RenderLayerList.begin();
		auto	iterEnd = m_RenderLayerList.end();

		for (; iter != iterEnd; ++iter)
		{
			auto	iter1 = (*iter)->RenderList.begin();
			auto	iter1End = (*iter)->RenderList.end();

			if ((*iter)->AlphaBlend)
				(*iter)->AlphaBlend->SetState();

			for (; iter1 != iter1End;)
			{
				if (!(*iter1)->GetActive())
				{
					iter1 = (*iter)->RenderList.erase(iter1);
					iter1End = (*iter)->RenderList.end();
					continue;
				}

				else if (!(*iter1)->GetEnable())
				{
					++iter1;
					continue;
				}

				(*iter1)->Render();
				++iter1;
			}

			if ((*iter)->AlphaBlend)
				(*iter)->AlphaBlend->ResetState();
		}
	}



	// 2D, 3D 물체를 모두 출력했다면 UI를 출력해준다.
	// 깊이버퍼를 안쓰고 알파블렌드를 적용한다.
	m_AlphaBlend->SetState();
	m_DepthDisable->SetState();

	CSceneManager::GetInst()->GetScene()->GetViewport()->Render();

	m_DepthDisable->ResetState();
	m_AlphaBlend->ResetState();
}

void CRenderManager::SetBlendFactor(const std::string& Name, float r, float g, float b, float a)
{
	m_RenderStateManager->SetBlendFactor(Name, r, g, b, a);
}

void CRenderManager::AddBlendInfo(const std::string& Name, bool BlendEnable, 
	D3D11_BLEND SrcBlend, D3D11_BLEND DestBlend, D3D11_BLEND_OP BlendOp,
	D3D11_BLEND SrcAlphBlend, D3D11_BLEND DestAlphBlend, D3D11_BLEND_OP BlendAlphOp,
	UINT8 WriteMask)
{
	m_RenderStateManager->AddBlendInfo(Name, BlendEnable, SrcBlend, DestBlend, BlendOp,
		SrcAlphBlend, DestAlphBlend, BlendAlphOp, WriteMask);
}

bool CRenderManager::CreateBlendState(const std::string& Name,
	bool AlphaToCoverageEnable, bool IndependentBlendEnable)
{
	return m_RenderStateManager->CreateBlendState(Name, AlphaToCoverageEnable, IndependentBlendEnable);
}

bool CRenderManager::CreateDepthStencil(const std::string& Name, 
	bool DepthEnable, D3D11_DEPTH_WRITE_MASK DepthWriteMask, 
	D3D11_COMPARISON_FUNC DepthFunc, bool StencilEnable, 
	UINT8 StencilReadMask, UINT8 StencilWriteMask, 
	D3D11_DEPTH_STENCILOP_DESC FrontFace, D3D11_DEPTH_STENCILOP_DESC BackFace)
{
	return m_RenderStateManager->CreateDepthStencil(Name, DepthEnable,
		DepthWriteMask, DepthFunc, StencilEnable, StencilReadMask, StencilWriteMask,
		FrontFace, BackFace);
}

bool CRenderManager::SortLayer(RenderLayer* Src, RenderLayer* Dest)
{
	return Src->LayerPriority < Dest->LayerPriority;
}
