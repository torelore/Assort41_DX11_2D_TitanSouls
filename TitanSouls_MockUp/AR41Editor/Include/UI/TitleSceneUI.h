#pragma once

#include "UI\UIWindow.h"


enum class ETitleUIMode
{
	Main,
	SelectSave,
	Option,
	Quit,
	End
};

enum class EMainMenu
{
	Start,
	Option,
	Quit,
	End
};

enum class ESaveSelect
{
	First,
	Second,
	Third,
	Fourth,
	Back,
	End
};


enum class EQuitMenu
{
	Yes,
	No
};

class CTitleSceneUI :
    public CUIWindow
{
	friend class CSceneViewport;
	friend class CScene;

protected:
	CTitleSceneUI();
	CTitleSceneUI(const CTitleSceneUI& Window);
	virtual ~CTitleSceneUI();

protected:
	std::unordered_map<std::string, CSharedPtr<CUIWidget>> m_mapMainUI;
	std::unordered_map<std::string, CSharedPtr<CUIWidget>> m_mapSaveSelectUI;
	std::unordered_map<std::string, CSharedPtr<CUIWidget>> m_mapOptionUI;
	std::unordered_map<std::string, CSharedPtr<CUIWidget>> m_mapQuitUI;

private :
	ETitleUIMode m_NowUIMode;
	EMainMenu m_MainSelected;
	ESaveSelect m_SaveSelected;
	EQuitMenu m_QuitSelected;

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Render();
	virtual CTitleSceneUI* Clone();
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);

private :
	void UIActive();

	void CreateTitleMainUI();
	void CreateSaveSelectUI();
	void CreateOptionUI();
	void CreateQuitUI();

private:
	void TitleKeyUp();
	void TitleKeyDown();
	void TitleKeyEnter();
	void TitleKeyBack();
};

