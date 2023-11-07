
#include "TransformConstantBuffer.h"

CTransformConstantBuffer::CTransformConstantBuffer()	:
	m_BufferData{}
{
}

CTransformConstantBuffer::CTransformConstantBuffer(const CTransformConstantBuffer& Buffer)	:
	CConstantBufferData(Buffer)
{
	m_BufferData = Buffer.m_BufferData;
}

CTransformConstantBuffer::~CTransformConstantBuffer()
{
}

bool CTransformConstantBuffer::Init()
{
	SetConstantBuffer("Transform");

	return true;
}

void CTransformConstantBuffer::UpdateBuffer()
{
	m_BufferData.matWVP = m_BufferData.matWorld * m_BufferData.matView * m_BufferData.matProj;


	// Shader���� ����� �� �ְ� �ϱ� ���ؼ� ��ġ����� ���� �Ѱ��ش�.
	m_BufferData.matWorld.Transpose();
	m_BufferData.matView.Transpose();
	m_BufferData.matProj.Transpose();
	m_BufferData.matWVP.Transpose();

	m_Buffer->UpdateBuffer(&m_BufferData);
}

CTransformConstantBuffer* CTransformConstantBuffer::Clone()
{
	return new CTransformConstantBuffer(*this);
}
