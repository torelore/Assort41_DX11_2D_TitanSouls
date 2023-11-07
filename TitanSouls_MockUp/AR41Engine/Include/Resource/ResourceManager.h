#pragma once

#include "../EngineInfo.h"
#include "Shader/ShaderManager.h"
#include "Material/MaterialManager.h"
#include "Animation/AnimationManager.h"
#include "Sound/SoundManager.h"
#include "Font/FontManager.h"

class CResourceManager
{
private:
	class CMeshManager* m_MeshManager;
	CShaderManager* m_ShaderManager;
	class CTextureManager* m_TextureManager;
	CMaterialManager* m_MaterialManager;
	CAnimationManager* m_AnimationManager;
	CSoundManager* m_SoundManager;
	CFontManager* m_FontManager;

public:
	bool Init();
	void Update();


public:	// ===================== Mesh =========================
	bool CreateMesh(class CScene* Scene, MeshType Type, const std::string& Name,
		void* VtxData, int Size, int Count,
		D3D11_USAGE VtxUsage, D3D11_PRIMITIVE_TOPOLOGY Primitive,
		void* IdxData = nullptr, int IdxSize = 0, int IdxCount = 0,
		D3D11_USAGE IdxUsage = D3D11_USAGE_DEFAULT,
		DXGI_FORMAT Fmt = DXGI_FORMAT_UNKNOWN);

	class CMesh* FindMesh(const std::string& Name);
	void ReleaseMesh(const std::string& Name);



public:	// ===================== Shader =========================
	template <typename T>
	bool CreateShader(const std::string& Name, bool GlobalShader = false)
	{
		return m_ShaderManager->CreateShader<T>(Name, GlobalShader);
	}

	class CColliderConstantBuffer* GetColliderCBuffer()	const;
	class CShader* FindShader(const std::string& Name);
	void ReleaseShader(const std::string& Name);

	bool CreateConstantBuffer(const std::string& Name, int Size, int Register,
		int ShaderBufferType = (int)EShaderBufferType::All);
	class CConstantBuffer* FindConstantBuffer(const std::string& Name);

	

public:	// ===================== Texture =========================
	bool LoadTexture(const std::string& Name, const TCHAR* FileName,
		const std::string& PathName = TEXTURE_PATH);
	bool LoadTextureFullPath(const std::string& Name, const TCHAR* FullPath);
	bool LoadTexture(const std::string& Name, const std::vector<const TCHAR*>& vecFileName,
		const std::string& PathName = TEXTURE_PATH);
	bool LoadTextureFullPath(const std::string& Name, const std::vector<const TCHAR*>& vecFullPath);
	bool LoadTextureArray(const std::string& Name, const std::vector<const TCHAR*>& vecFileName,
		const std::string& PathName = TEXTURE_PATH);
	bool LoadTextureArrayFullPath(const std::string& Name, const std::vector<const TCHAR*>& vecFullPath);
	class CTexture* FindTexture(const std::string& Name);
	void ReleaseTexture(const std::string& Name);




public:	// ===================== Material =========================
	CMaterial* FindMaterial(const std::string& Name);
	void ReleaseMaterial(const std::string& Name);

	template <typename T>
	T* CreateMaterial(const std::string& Name)
	{
		return m_MaterialManager->CreateMaterial<T>(Name);
	}



public:	// ===================== Animation =========================
	bool CreateAnimationSequence2D(const std::string& Name, const std::string& TextureName,
		const TCHAR* FileName, const std::string& PathName = TEXTURE_PATH);
	bool CreateAnimationSequence2D(const std::string& Name, class CTexture* Texture);
	bool CreateAnimationSequence2DFullPath(const std::string& Name, const std::string& TextureName, const TCHAR* FullPath);
	bool CreateAnimationSequence2D(const std::string& Name, const std::string& TextureName, const std::vector<const TCHAR*>& vecFileName,
		const std::string& PathName = TEXTURE_PATH);
	bool CreateAnimationSequence2DFullPath(const std::string& Name, const std::string& TextureName, const std::vector<const TCHAR*>& vecFullPath);
	bool AddAnimationSequence2DFrame(const std::string& Name, const Vector2& Start, const Vector2& End);
	bool AddAnimationSequence2DFrame(const std::string& Name, float StartX, float StartY, float EndX,
		float EndY);
	bool AddAnimationSequence2DFrameAll(const std::string& Name, int Count, const Vector2& Start, const Vector2& End);
	bool AddAnimationSequence2DFrameAll(const std::string& Name, int Count, float StartX, float StartY, float EndX,
		float EndY);
	bool SaveSequence2D(const std::string& Name, const char* FullPath);
	bool LoadSequence2D(const std::string& Name, const char* FullPath);
	bool SaveSequence2D(const std::string& Name, const char* FileName, const std::string& PathName);
	bool LoadSequence2D(const std::string& Name, const char* FileName, const std::string& PathName);

	CAnimationSequence2D* FindAnimationSequence2D(const std::string& Name);
	void ReleaseAnimationSequence2D(const std::string& Name);
	class CAnimation2DConstantBuffer* GetAnim2DConstantBuffer()	const;



public:	// ============================ Sound ================================
	bool CreateSoundChannel(const std::string& Name);
	bool LoadSound(const std::string& GroupName, const std::string& Name,
		bool Loop, const char* FileName, const std::string& PathName = SOUND_PATH);
	bool SetVolume(int Volume);
	bool SetVolume(const std::string& GroupName, int Volume);
	bool SoundPlay(const std::string& Name);
	bool SoundStop(const std::string& Name);
	bool SoundPause(const std::string& Name);
	bool SoundResume(const std::string& Name);

	void SoundAllStop();


	FMOD::ChannelGroup* FindChannelGroup(const std::string& Name);
	class CSound* FindSound(const std::string& Name);
	void ReleaseSound(const std::string& Name);



public:	// ============================ Font ================================
	bool CreateFontCollection(const std::string& Name, const TCHAR* FileName,
		const std::string& PathName = FONT_PATH);
	bool LoadFont(const std::string& Name, const TCHAR* FontName,
		int Weight, float FontSize, const TCHAR* LocalName, int Stretch = 5);

	const TCHAR* GetFontFaceName(const std::string& CollectionName);
	const char* GetFontFaceNameMultibyte(const std::string& CollectionName);

	bool CreateFontColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	bool CreateFontColor(float r, float g, float b, float a);
	bool CreateFontColor(const Vector4& Color);
	bool CreateFontColor(unsigned int Color);

	ID2D1SolidColorBrush* FindFontColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	ID2D1SolidColorBrush* FindFontColor(float r, float g, float b, float a);
	ID2D1SolidColorBrush* FindFontColor(const Vector4& Color);
	ID2D1SolidColorBrush* FindFontColor(unsigned int Color);

	unsigned int CreateFontColorKey(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	unsigned int CreateFontColorKey(float r, float g, float b, float a);
	unsigned int CreateFontColorKey(const Vector4& Color);

	CFont* FindFont(const std::string& Name);
	CFontCollection* FindFontCollection(const std::string& Name);
	void ReleaseFont(const std::string& Name);
	void ReleaseFontCollection(const std::string& Name);



	DECLARE_SINGLE(CResourceManager)
};

