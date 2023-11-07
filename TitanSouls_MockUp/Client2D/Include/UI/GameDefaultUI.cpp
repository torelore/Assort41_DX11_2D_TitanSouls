#include "GameDefaultUI.h"

#include "../GameObject/Player.h"
#include "../GameObject/Arrow.h"

#include "UI/UIButton.h"
#include "UI/UIText.h"
#include "UI/UIImage.h"

#include "Scene/SceneManager.h"
#include "Scene/Scene.h"

#include "Engine.h"
#include "Input.h"
#include "Device.h"
#include "Resource/ResourceManager.h"

#include "../Scene/TitleScene.h"
#include "../Scene/StartingRuinSceneInfo.h"
#include "../Scene/SlimeSceneInfo.h"
#include "../Scene/KnightSceneInfo.h"
#include "../Scene/YetiSceneInfo.h"

CGameDefaultUI::CGameDefaultUI() :
	m_MenuUISelected(EMenuUISelect::Retry),
	m_DeadUISelected(EDeadUISelect::Retry),
	m_NowScene(ESceneList::None),
	m_IsMenuActive(false),
	m_IsDeadActive(false),
	m_IsDebugActive(false)
{
    m_WindowTypeName = "CGameDefaultUI";
}

CGameDefaultUI::CGameDefaultUI(const CGameDefaultUI& Window) :
    CUIWindow(Window),
	m_MenuUISelected(EMenuUISelect::Retry),
	m_DeadUISelected(EDeadUISelect::Retry),
	m_NowScene(ESceneList::None),
	m_IsMenuActive(false),
	m_IsDeadActive(false),
	m_IsDebugActive(false)
{
}

CGameDefaultUI::~CGameDefaultUI()
{
}

void CGameDefaultUI::Start()
{
	CUIWindow::Start();

	CInput::GetInst()->AddBindFunction<CGameDefaultUI>("UIBack", Input_Type::Up, this, &CGameDefaultUI::MenuUIClose, m_Scene);

	CInput::GetInst()->AddBindFunction<CGameDefaultUI>("UIEsc", Input_Type::Up, this, &CGameDefaultUI::DefaultUIEsc, m_Scene);

	CInput::GetInst()->AddBindFunction<CGameDefaultUI>("DebugModeF1", Input_Type::Up, this, &CGameDefaultUI::DebugUIF1, m_Scene);
}

bool CGameDefaultUI::Init()
{
	CUIWindow::Init();

	CreateMenuUI();
	CreateDeadUI();
	CreateDebugUI();
	
	MenuUIClose();
	DeadUIClose();
	DebugUIClose();

    return true;
}

void CGameDefaultUI::Update(float DeltaTime)
{
	CUIWindow::Update(DeltaTime);

	if (m_IsMenuActive) {
		CUIText* UIText = (CUIText*)m_mapUI.find("StateText")->second.Get();

		switch (m_MenuUISelected)
		{
		case EMenuUISelect::Back:
			UIText->SetText(TEXT("BACK"));
			break;
		case EMenuUISelect::Retry:
			UIText->SetText(TEXT("RETRY"));
			break;
		case EMenuUISelect::Exit:
			UIText->SetText(TEXT("EXIT"));
			break;
		}
	}
	
	if (m_IsDeadActive) {
		CUIText* UIText = (CUIText*)m_mapDeadUI.find("DeadStateText")->second.Get();

		switch (m_DeadUISelected)
		{
		case EDeadUISelect::Retry:
			UIText->SetText(TEXT("RETRY"));
			break;
		case EDeadUISelect::Exit:
			UIText->SetText(TEXT("EXIT"));
			break;
		}
	}

	if (m_IsDebugActive) {




		CUIText* UIText = (CUIText*)m_mapDebugUI.find("DebugFps")->second.Get();

		TCHAR setText[MAX_PATH];
		
		float Fps = CEngine::GetInst()->GetFPS();

		wsprintf(setText, L"FPS : %d\n", (int)Fps);
		UIText->SetText(setText);
		


		// Player

		CPlayer* Player = (CPlayer*)m_Scene->FindObject("Player");
		const Vector3& PlayerPos = Player->GetWorldPos();


		UIText = (CUIText*)m_mapDebugUI.find("DebugPlayerInvincible")->second.Get();

		TCHAR InvText[MAX_PATH];
		if(Player->GetDebugInvincible())
			wsprintf(InvText, L" * Invincible Mode\n");
		else
			wsprintf(InvText, L"         \n");


		UIText->SetText(InvText);



		UIText = (CUIText*)m_mapDebugUI.find("DebugPlayerWorldPos")->second.Get();

		TCHAR posText[MAX_PATH];
		wsprintf(posText, L"World Pos : x %d, y %d, z %d\n", (int)PlayerPos.x, (int)PlayerPos.y, (int)PlayerPos.z);
		UIText->SetText(posText);



		UIText = (CUIText*)m_mapDebugUI.find("DebugPlayerAimTimer")->second.Get();

		TCHAR AimTimerText[MAX_PATH];
		wsprintf(AimTimerText, L"Aim Timer : %d", (int)Player->GetAimTimer());
		UIText->SetText(AimTimerText);



		// Arrow
		CArrow* Arrow = (CArrow*)m_Scene->FindObject("Arrow");

		if (Arrow) {
			const Vector3& ArrowPos = Arrow->GetWorldPos();

			UIText = (CUIText*)m_mapDebugUI.find("DebugArrowPos")->second.Get();
			TCHAR ArrowPosText[MAX_PATH];
			wsprintf(ArrowPosText, L"World Pos : x %d, y %d, z %d", (int)ArrowPos.x, (int)ArrowPos.y, (int)ArrowPos.z);
			UIText->SetText(ArrowPosText);


			UIText = (CUIText*)m_mapDebugUI.find("DebugArrowSpeed")->second.Get();

			TCHAR ArrowSpeedText[MAX_PATH];
			wsprintf(ArrowSpeedText, L"Speed : %d", (int)Arrow->GetSpeed());
			UIText->SetText(ArrowSpeedText);


			UIText = (CUIText*)m_mapDebugUI.find("DebugArrowAccel")->second.Get();

			TCHAR ArrowAccelText[MAX_PATH];
			wsprintf(ArrowAccelText, L"Accel : %d", (int)Arrow->GetAccel());
			UIText->SetText(ArrowAccelText);


			UIText = (CUIText*)m_mapDebugUI.find("DebugArrowRot")->second.Get();

			TCHAR ArrowRotText[MAX_PATH];
			wsprintf(ArrowRotText, L"Rot : %d", (int)Arrow->GetWorldRot().z);
			UIText->SetText(ArrowRotText);



		}
		else {
			UIText = (CUIText*)m_mapDebugUI.find("DebugArrowPos")->second.Get();
			TCHAR ArrowPosText[MAX_PATH];
			wsprintf(ArrowPosText, L"World Pos : Arrow None");
			UIText->SetText(ArrowPosText);


			UIText = (CUIText*)m_mapDebugUI.find("DebugArrowSpeed")->second.Get();

			TCHAR ArrowSpeedText[MAX_PATH];
			wsprintf(ArrowSpeedText, L"Speed : 0");
			UIText->SetText(ArrowSpeedText);


			UIText = (CUIText*)m_mapDebugUI.find("DebugArrowAccel")->second.Get();

			TCHAR ArrowAccelText[MAX_PATH];
			wsprintf(ArrowAccelText, L"Accel : 0");
			UIText->SetText(ArrowAccelText);



			UIText = (CUIText*)m_mapDebugUI.find("DebugArrowRot")->second.Get();

			TCHAR ArrowRotText[MAX_PATH];
			wsprintf(ArrowRotText, L"Rot : 0");
			UIText->SetText(ArrowRotText);

		}



		


	}


}

void CGameDefaultUI::PostUpdate(float DeltaTime)
{
	CUIWindow::PostUpdate(DeltaTime);
}

void CGameDefaultUI::Render()
{
	CUIWindow::Render();
}

CGameDefaultUI* CGameDefaultUI::Clone()
{
	return new CGameDefaultUI(*this);
}

void CGameDefaultUI::Save(FILE* File)
{
	CUIWindow::Save(File);
}

void CGameDefaultUI::Load(FILE* File)
{
	CUIWindow::Load(File);
}

void CGameDefaultUI::CreateMenuUI()
{
	Resolution	RS = CDevice::GetInst()->GetResolution();

	float CenterX = RS.Width / 2.f;

	CUIText* StateText = CreateWidget<CUIText>("StateText");

	float TextSizeX = 300.f;
	float TextSizeY = 50.f;
	float TextPosX = CenterX - TextSizeX / 2.f;
	float TextPosY = RS.Height - TextSizeY * 2.f;

	StateText->SetSize(TextSizeX, TextSizeY);
	StateText->SetAlignH(Text_Align_H::Center);
	StateText->SetPos(CenterX - TextSizeX / 2.f, TextPosY);
	StateText->SetFontSize(30.f);
	StateText->SetColor(Vector4::White);
	StateText->SetText(TEXT("Back"));
	StateText->SetTransparency(false);
	StateText->SetShadowEnable(false);

	m_mapUI.insert(std::make_pair("StateText", StateText));



	CUIButton* BackButton = CreateWidget<CUIButton>("BackButton");

	float ButtonSize = 100.f;

	float ButtonSpace = 50.f;

	float ButtonPosX = CenterX - ButtonSize * 1.5f - ButtonSpace;
	float ButtonPosY = TextPosY - TextSizeY * 3.f;

	BackButton->SetSize(ButtonSize, ButtonSize);
	BackButton->SetPos(ButtonPosX, ButtonPosY);

	float xIdx = 0.f;
	float yIdx = 304.f;
	float standard = 32.f;


	BackButton->SetTexture(EButtonState::Normal, "BackButton", TEXT("IMG/player.png"));
	BackButton->AddFrameData(EButtonState::Normal, Vector2(xIdx, yIdx), Vector2(xIdx + standard, yIdx + standard));

	xIdx += standard;

	BackButton->SetTexture(EButtonState::Hovered, "BackButton", TEXT("IMG/player.png"));
	BackButton->AddFrameData(EButtonState::Hovered, Vector2(xIdx, yIdx), Vector2(xIdx + standard, yIdx + standard));

	BackButton->SetSound(EButtonEventState::Hovered, "UI", "BackButtonHovered", false, "UI/Move.ogg");
	BackButton->SetSound(EButtonEventState::Click, "UI", "BackButtonClick", false, "UI/Select.ogg");

	BackButton->SetCallback<CGameDefaultUI>(EButtonEventState::Hovered, this, &CGameDefaultUI::MenuButtonHoveredBack);
	BackButton->SetCallback<CGameDefaultUI>(EButtonEventState::Click, this, &CGameDefaultUI::MenuUIClose);

	m_mapUI.insert(std::make_pair("BackButton", BackButton));



	CUIButton* RetryButton = CreateWidget<CUIButton>("RetryButton");

	ButtonPosX += ButtonSpace + ButtonSize;

	RetryButton->SetSize(ButtonSize, ButtonSize);
	RetryButton->SetPos(ButtonPosX, ButtonPosY);

	xIdx += standard;

	RetryButton->SetTexture(EButtonState::Normal, "RetryButton", TEXT("IMG/player.png"));
	RetryButton->AddFrameData(EButtonState::Normal, Vector2(xIdx, yIdx), Vector2(xIdx + standard, yIdx + standard));

	xIdx += standard;

	RetryButton->SetTexture(EButtonState::Hovered, "RetryButton", TEXT("IMG/player.png"));
	RetryButton->AddFrameData(EButtonState::Hovered, Vector2(xIdx, yIdx), Vector2(xIdx + standard, yIdx + standard));

	RetryButton->SetSound(EButtonEventState::Hovered, "UI", "RetryButtonHovered", false, "UI/Move.ogg");
	RetryButton->SetSound(EButtonEventState::Click, "UI", "RetryButtonClick", false, "UI/Select.ogg");

	RetryButton->SetCallback<CGameDefaultUI>(EButtonEventState::Hovered, this, &CGameDefaultUI::MenuButtonHoveredRetry);
	RetryButton->SetCallback<CGameDefaultUI>(EButtonEventState::Click, this, &CGameDefaultUI::SelectRetryUI);

	m_mapUI.insert(std::make_pair("RetryButton", RetryButton));



	CUIButton* ExitButton = CreateWidget<CUIButton>("ExitButton");

	ButtonPosX += ButtonSpace + ButtonSize;

	ExitButton->SetSize(ButtonSize, ButtonSize);
	ExitButton->SetPos(ButtonPosX, ButtonPosY);

	xIdx += standard;

	ExitButton->SetTexture(EButtonState::Normal, "ExitButton", TEXT("IMG/player.png"));
	ExitButton->AddFrameData(EButtonState::Normal, Vector2(xIdx, yIdx), Vector2(xIdx + standard, yIdx + standard));

	xIdx += standard;

	ExitButton->SetTexture(EButtonState::Hovered, "ExitButton", TEXT("IMG/player.png"));
	ExitButton->AddFrameData(EButtonState::Hovered, Vector2(xIdx, yIdx), Vector2(xIdx + standard, yIdx + standard));

	ExitButton->SetSound(EButtonEventState::Hovered, "UI", "ExitButtonHovered", false, "UI/Move.ogg");
	ExitButton->SetSound(EButtonEventState::Click, "UI", "ExitButtonClick", false, "UI/Select.ogg");

	ExitButton->SetCallback<CGameDefaultUI>(EButtonEventState::Hovered, this, &CGameDefaultUI::MenuButtonHoveredExit);
	ExitButton->SetCallback<CGameDefaultUI>(EButtonEventState::Click, this, &CGameDefaultUI::SelectExitUI);

	m_mapUI.insert(std::make_pair("ExitButton", ExitButton));
}

void CGameDefaultUI::CreateDeadUI()
{
	Resolution	RS = CDevice::GetInst()->GetResolution();

	float CenterX = RS.Width / 2.f;

	CUIText* StateText = CreateWidget<CUIText>("DeadStateText");

	float TextSizeX = 300.f;
	float TextSizeY = 50.f;
	float TextPosX = CenterX - TextSizeX / 2.f;
	float TextPosY = RS.Height - TextSizeY * 2.f;

	StateText->SetSize(TextSizeX, TextSizeY);
	StateText->SetAlignH(Text_Align_H::Center);
	StateText->SetPos(CenterX - TextSizeX / 2.f, TextPosY);
	StateText->SetFontSize(30.f);
	StateText->SetColor(Vector4::White);
	StateText->SetText(TEXT("Back"));
	StateText->SetTransparency(false);
	StateText->SetShadowEnable(false);

	m_mapDeadUI.insert(std::make_pair("DeadStateText", StateText));


	float ButtonSize = 100.f;
	float ButtonSpace = 50.f;

	float ButtonPosX = CenterX - ButtonSize * 1.5f - ButtonSpace;
	float ButtonPosY = TextPosY - TextSizeY * 3.f;


	float standard = 32.f;
	float xIdx = standard * 2.f;
	float yIdx = 304.f;

	CUIButton* RetryButton = CreateWidget<CUIButton>("DeadRetryButton");

	RetryButton->SetSize(ButtonSize, ButtonSize);
	RetryButton->SetPos(ButtonPosX, ButtonPosY);

	RetryButton->SetTexture(EButtonState::Normal, "DeadRetryButton", TEXT("IMG/player.png"));
	RetryButton->AddFrameData(EButtonState::Normal, Vector2(xIdx, yIdx), Vector2(xIdx + standard, yIdx + standard));

	xIdx += standard;

	RetryButton->SetTexture(EButtonState::Hovered, "DeadRetryButton", TEXT("IMG/player.png"));
	RetryButton->AddFrameData(EButtonState::Hovered, Vector2(xIdx, yIdx), Vector2(xIdx + standard, yIdx + standard));

	RetryButton->SetSound(EButtonEventState::Hovered, "UI", "RetryButtonHovered", false, "UI/Move.ogg");
	RetryButton->SetSound(EButtonEventState::Click, "UI", "RetryButtonClick", false, "UI/Select.ogg");

	RetryButton->SetCallback<CGameDefaultUI>(EButtonEventState::Hovered, this, &CGameDefaultUI::MenuButtonHoveredRetry);
	RetryButton->SetCallback<CGameDefaultUI>(EButtonEventState::Click, this, &CGameDefaultUI::SelectRetryUI);

	m_mapDeadUI.insert(std::make_pair("DeadRetryButton", RetryButton));



	CUIButton* ExitButton = CreateWidget<CUIButton>("DeadExitButton");

	ButtonPosX += ButtonSpace + ButtonSize;
	ButtonPosX += ButtonSpace + ButtonSize;

	ExitButton->SetSize(ButtonSize, ButtonSize);
	ExitButton->SetPos(ButtonPosX, ButtonPosY);

	xIdx += standard;

	ExitButton->SetTexture(EButtonState::Normal, "DeadExitButton", TEXT("IMG/player.png"));
	ExitButton->AddFrameData(EButtonState::Normal, Vector2(xIdx, yIdx), Vector2(xIdx + standard, yIdx + standard));

	xIdx += standard;

	ExitButton->SetTexture(EButtonState::Hovered, "DeadExitButton", TEXT("IMG/player.png"));
	ExitButton->AddFrameData(EButtonState::Hovered, Vector2(xIdx, yIdx), Vector2(xIdx + standard, yIdx + standard));

	ExitButton->SetSound(EButtonEventState::Hovered, "UI", "ExitButtonHovered", false, "UI/Move.ogg");
	ExitButton->SetSound(EButtonEventState::Click, "UI", "ExitButtonClick", false, "UI/Select.ogg");

	ExitButton->SetCallback<CGameDefaultUI>(EButtonEventState::Hovered, this, &CGameDefaultUI::MenuButtonHoveredExit);
	ExitButton->SetCallback<CGameDefaultUI>(EButtonEventState::Click, this, &CGameDefaultUI::SelectExitUI);

	m_mapDeadUI.insert(std::make_pair("DeadExitButton", ExitButton));
}

void CGameDefaultUI::CreateDebugUI()
{
	// Fps 수치, 타이머
	Resolution	RS = CDevice::GetInst()->GetResolution();

	float CenterX = RS.Width / 2.f;

	float Standard = 25.f;
	float FontSize = 20.f;

	float TextSizeX = 300.f;
	float TextSizeY = 50.f;
	float TextPosX = Standard;
	float TextPosY = RS.Height - Standard * 2;

	CUIText* StateText = CreateWidget<CUIText>("DebugFps");

	StateText->SetSize(TextSizeX, TextSizeY);
	StateText->SetAlignH(Text_Align_H::Left);
	StateText->SetPos(TextPosX, TextPosY);
	StateText->SetFontSize(FontSize);
	StateText->SetColor(Vector4::Red);
	StateText->SetText(TEXT("FPS : \n"));
	StateText->SetTransparency(false);
	StateText->SetShadowEnable(false);

	m_mapDebugUI.insert(std::make_pair("DebugFps", StateText));


	// 씬 정보


	// 플레이어 정보
	CUIText* PlayerText = CreateWidget<CUIText>("DebugPlayer");
	TextPosY -= FontSize * 2.f;

	PlayerText->SetSize(TextSizeX, TextSizeY);
	PlayerText->SetAlignH(Text_Align_H::Left);
	PlayerText->SetPos(TextPosX, TextPosY);
	PlayerText->SetFontSize(FontSize);
	PlayerText->SetColor(Vector4::Red);
	PlayerText->SetText(TEXT("Player Info"));
	PlayerText->SetTransparency(false);
	PlayerText->SetShadowEnable(false);

	m_mapDebugUI.insert(std::make_pair("DebugPlayer", PlayerText));



	PlayerText = CreateWidget<CUIText>("DebugPlayerInvincible");

	PlayerText->SetSize(TextSizeX, TextSizeY);
	PlayerText->SetAlignH(Text_Align_H::Left);
	PlayerText->SetPos(TextPosX + 200.f, TextPosY);
	PlayerText->SetFontSize(FontSize);
	PlayerText->SetColor(Vector4::Red);
	PlayerText->SetText(TEXT("     "));
	PlayerText->SetTransparency(false);
	PlayerText->SetShadowEnable(false);

	m_mapDebugUI.insert(std::make_pair("DebugPlayerInvincible", PlayerText));



	PlayerText = CreateWidget<CUIText>("DebugPlayerWorldPos");
	TextPosY -= FontSize;

	PlayerText->SetSize(TextSizeX, TextSizeY);
	PlayerText->SetAlignH(Text_Align_H::Left);
	PlayerText->SetPos(TextPosX, TextPosY);
	PlayerText->SetFontSize(FontSize);
	PlayerText->SetColor(Vector4::Red);
	PlayerText->SetText(TEXT("World Pos : x 0000.00, y 0000.00, z 0000.00\n"));
	PlayerText->SetTransparency(false);
	PlayerText->SetShadowEnable(false);

	m_mapDebugUI.insert(std::make_pair("DebugPlayerWorldPos", PlayerText));



	PlayerText = CreateWidget<CUIText>("DebugPlayerAimTimer");
	TextPosY -= FontSize;

	PlayerText->SetSize(TextSizeX, TextSizeY);
	PlayerText->SetAlignH(Text_Align_H::Left);
	PlayerText->SetPos(TextPosX, TextPosY);
	PlayerText->SetFontSize(FontSize);
	PlayerText->SetColor(Vector4::Red);
	PlayerText->SetText(TEXT("Aim Timer : "));
	PlayerText->SetTransparency(false);
	PlayerText->SetShadowEnable(false);

	m_mapDebugUI.insert(std::make_pair("DebugPlayerAimTimer", PlayerText));



	PlayerText = CreateWidget<CUIText>("DebugPlayerState");
	TextPosY -= FontSize;

	PlayerText->SetSize(TextSizeX, TextSizeY);
	PlayerText->SetAlignH(Text_Align_H::Left);
	PlayerText->SetPos(TextPosX, TextPosY);
	PlayerText->SetFontSize(FontSize);
	PlayerText->SetColor(Vector4::Red);
	PlayerText->SetText(TEXT("Player State : test"));
	PlayerText->SetTransparency(false);
	PlayerText->SetShadowEnable(false);

	m_mapDebugUI.insert(std::make_pair("DebugPlayerState", PlayerText));


	// 화살 정보
	CUIText* ArrowText = CreateWidget<CUIText>("DebugArrow");
	TextPosY -= FontSize * 2.f;

	ArrowText->SetSize(TextSizeX, TextSizeY);
	ArrowText->SetAlignH(Text_Align_H::Left);
	ArrowText->SetPos(TextPosX, TextPosY);
	ArrowText->SetFontSize(FontSize);
	ArrowText->SetColor(Vector4::Red);
	ArrowText->SetText(TEXT("Arrow Info"));
	ArrowText->SetTransparency(false);
	ArrowText->SetShadowEnable(false);

	m_mapDebugUI.insert(std::make_pair("DebugArrow", ArrowText));


	ArrowText = CreateWidget<CUIText>("DebugArrowPos");
	TextPosY -= FontSize;

	ArrowText->SetSize(TextSizeX, TextSizeY);
	ArrowText->SetAlignH(Text_Align_H::Left);
	ArrowText->SetPos(TextPosX, TextPosY);
	ArrowText->SetFontSize(FontSize);
	ArrowText->SetColor(Vector4::Red);
	ArrowText->SetText(TEXT("Arrow World Pos : "));
	ArrowText->SetTransparency(false);
	ArrowText->SetShadowEnable(false);

	m_mapDebugUI.insert(std::make_pair("DebugArrowPos", ArrowText));


	ArrowText = CreateWidget<CUIText>("DebugArrowSpeed");
	TextPosY -= FontSize;

	ArrowText->SetSize(TextSizeX, TextSizeY);
	ArrowText->SetAlignH(Text_Align_H::Left);
	ArrowText->SetPos(TextPosX, TextPosY);
	ArrowText->SetFontSize(FontSize);
	ArrowText->SetColor(Vector4::Red);
	ArrowText->SetText(TEXT("Speed : 000"));
	ArrowText->SetTransparency(false);
	ArrowText->SetShadowEnable(false);

	m_mapDebugUI.insert(std::make_pair("DebugArrowSpeed", ArrowText));


	ArrowText = CreateWidget<CUIText>("DebugArrowAccel");

	ArrowText->SetSize(TextSizeX, TextSizeY);
	ArrowText->SetAlignH(Text_Align_H::Left);
	ArrowText->SetPos(TextPosX + 150.f, TextPosY);
	ArrowText->SetFontSize(FontSize);
	ArrowText->SetColor(Vector4::Red);
	ArrowText->SetText(TEXT("Accel : 000"));
	ArrowText->SetTransparency(false);
	ArrowText->SetShadowEnable(false);

	m_mapDebugUI.insert(std::make_pair("DebugArrowAccel", ArrowText));


	ArrowText = CreateWidget<CUIText>("DebugArrowRot");
	TextPosY -= FontSize;

	ArrowText->SetSize(TextSizeX, TextSizeY);
	ArrowText->SetAlignH(Text_Align_H::Left);
	ArrowText->SetPos(TextPosX, TextPosY);
	ArrowText->SetFontSize(FontSize);
	ArrowText->SetColor(Vector4::Red);
	ArrowText->SetText(TEXT("Rot : 000"));
	ArrowText->SetTransparency(false);
	ArrowText->SetShadowEnable(false);

	m_mapDebugUI.insert(std::make_pair("DebugArrowRot", ArrowText));
}

void CGameDefaultUI::SelectRetryUI()
{
	// 현재 씬을 찾아서 재시작한다.
	CSceneManager::GetInst()->CreateNextScene(true);

	if (m_NowScene == ESceneList::None)
		return;

	m_Scene->GetResource()->SoundAllStop();

	switch (m_NowScene)
	{
	case ESceneList::StartingRuins:
		CSceneManager::GetInst()->CreateSceneInfo<CStartingRuinSceneInfo>(false);
		break;
	case ESceneList::Yeti:
		CSceneManager::GetInst()->CreateSceneInfo<CYetiSceneInfo>(false);
		break;
	case ESceneList::SludgeHeart:
		CSceneManager::GetInst()->CreateSceneInfo<CSlimeSceneInfo>(false);
		break;
	case ESceneList::Knight:
		CSceneManager::GetInst()->CreateSceneInfo<CKnightSceneInfo>(false);
		break;
	}
}

void CGameDefaultUI::SelectExitUI()
{
	// 메인 메뉴로 되돌아간다.
	CSceneManager::GetInst()->CreateNextScene(true);
	CSceneManager::GetInst()->CreateSceneInfo<CTitleScene>(false);
}

void CGameDefaultUI::MenuUIActive()
{
	for (auto iter : m_mapUI) {
		iter.second->SetEnable(true);
	}

	m_IsMenuActive = true;
}

void CGameDefaultUI::MenuUIInActive()
{
	for (auto iter : m_mapUI) {
		iter.second->SetEnable(false);
	}

	m_IsMenuActive = false;
}

void CGameDefaultUI::DeadUIActive()
{
	for (auto iter : m_mapDeadUI) {
		iter.second->SetEnable(true);
	}

	m_IsDeadActive = true;
}

void CGameDefaultUI::DeadUIInActive()
{
	for (auto iter : m_mapDeadUI) {
		iter.second->SetEnable(false);
	}

	m_IsDeadActive = false;
}

void CGameDefaultUI::DebugUIActive()
{
	for (auto iter : m_mapDebugUI) {
		iter.second->SetEnable(true);
	}

	m_IsDebugActive = true;
}

void CGameDefaultUI::DebugUIInActive()
{
	for (auto iter : m_mapDebugUI) {
		iter.second->SetEnable(false);
	}

	m_IsDebugActive = false;
}

void CGameDefaultUI::MenuUIOpen()
{
	if (m_IsDeadActive) {
		return;
	}


	MenuUIActive();

	CEngine::GetInst()->TimeStop();
}

void CGameDefaultUI::MenuUIClose()
{
	MenuUIInActive();

	CEngine::GetInst()->TimePlay();
}

void CGameDefaultUI::DefaultUIEsc()
{
	if (m_IsMenuActive) 
		MenuUIClose();
	else 
		MenuUIOpen();
}

void CGameDefaultUI::MenuButtonHoveredBack()
{
	m_MenuUISelected = EMenuUISelect::Back;
}

void CGameDefaultUI::MenuButtonHoveredRetry()
{
	m_MenuUISelected = EMenuUISelect::Retry;
}

void CGameDefaultUI::MenuButtonHoveredExit()
{
	m_MenuUISelected = EMenuUISelect::Exit;
}

void CGameDefaultUI::DeadUIOpen()
{
	DeadUIActive();
}

void CGameDefaultUI::DeadUIClose()
{
	DeadUIInActive();
}

void CGameDefaultUI::DeadUIButtonHoveredRetry()
{
	m_DeadUISelected = EDeadUISelect::Retry;
}

void CGameDefaultUI::DeadUIButtonHoveredExit()
{
	m_DeadUISelected = EDeadUISelect::Exit;
}

void CGameDefaultUI::DebugUIOpen()
{
	DebugUIActive();
}

void CGameDefaultUI::DebugUIClose()
{
	DebugUIInActive();
}

void CGameDefaultUI::DebugUIF1()
{
	if (m_IsDebugActive)
		DebugUIClose();
	else 
		DebugUIOpen();
}
