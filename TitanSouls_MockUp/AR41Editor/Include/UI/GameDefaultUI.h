#pragma once

#include "UI\UIWindow.h"

enum class EMenuUISelect {
	Back,
	Retry,
	Exit
};

enum class EDeadUISelect {
	Retry,
	Exit
};

class CGameDefaultUI :
    public CUIWindow
{
	friend class CSceneViewport;
	friend class CScene;

protected:
	CGameDefaultUI();
	CGameDefaultUI(const CGameDefaultUI& Window);
	virtual ~CGameDefaultUI();

protected:
	std::unordered_map<std::string, CSharedPtr<CUIWidget>> m_mapUI;
	std::unordered_map<std::string, CSharedPtr<CUIWidget>> m_mapDeadUI;
	std::unordered_map<std::string, CSharedPtr<CUIWidget>> m_mapDebugUI;

private :
	EMenuUISelect m_MenuUISelected;
	EDeadUISelect m_DeadUISelected;
	ESceneList m_NowScene;
	bool m_IsMenuActive;
	bool m_IsDeadActive;
	bool m_IsDebugActive;

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Render();
	virtual CGameDefaultUI* Clone();
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);

public :
	void SetNowScene(ESceneList NowScene) {
		m_NowScene = NowScene;
	}


private:
	void CreateMenuUI();
	void CreateDeadUI();
	void CreateDebugUI();

	void SelectRetryUI();
	void SelectExitUI();

private :
	void MenuUIActive();
	void MenuUIInActive();

	void DeadUIActive();
	void DeadUIInActive();

	void DebugUIActive();
	void DebugUIInActive();

	// Menu UI
private :
	void MenuUIOpen();
	void MenuUIClose();

	void DefaultUIEsc();

	void MenuButtonHoveredBack();
	void MenuButtonHoveredRetry();
	void MenuButtonHoveredExit();

	// DeadUI
public:
	void DeadUIOpen();
	void DeadUIClose();

private:
	void DeadUIButtonHoveredRetry();
	void DeadUIButtonHoveredExit();

	// DebugUI
private :
	void DebugUIOpen();
	void DebugUIClose();

	void DebugUIF1();
};

