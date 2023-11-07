#pragma once

#include "ConstantBufferData.h"

class CMaterialConstantBuffer :
    public CConstantBufferData
{
public:
	CMaterialConstantBuffer();
	CMaterialConstantBuffer(const CMaterialConstantBuffer& Buffer);
	virtual ~CMaterialConstantBuffer();

private:
	MaterialCBuffer	m_BufferData;

public:
	virtual bool Init();
	virtual void UpdateBuffer();
	virtual CMaterialConstantBuffer* Clone();

public:
	void SetBaseColor(const Vector4& Color)
	{
		m_BufferData.BaseColor = Color;
	}

	void SetAmbientColor(const Vector4& Color)
	{
		m_BufferData.AmbientColor = Color;
	}

	void SetSpecularColor(const Vector4& Color)
	{
		m_BufferData.SpecularColor = Color;
	}

	void SetEmissiveColor(const Vector4& Color)
	{
		m_BufferData.EmissiveColor = Color;
	}

	void SetOpacity(float Opacity)
	{
		m_BufferData.Opacity = Opacity;
	}

	void SetImageType(EImageType Type)
	{
		m_BufferData.TextureType = (int)Type;
	}

	void SetTextureWidth(float Width)
	{
		m_BufferData.TextureWidth = Width;
	}

	void SetTextureHeight(float Height)
	{
		m_BufferData.TextureHeight = Height;
	}
};

