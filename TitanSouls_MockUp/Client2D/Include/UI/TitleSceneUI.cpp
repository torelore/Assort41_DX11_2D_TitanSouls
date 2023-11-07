#include "TitleSceneUI.h"

#include "UI/UIButton.h"
#include "UI/UIText.h"
#include "UI/UIImage.h"

#include "Scene/SceneManager.h"
#include "Scene/Scene.h"
#include "Engine.h"
#include "Input.h"
#include "Resource/ResourceManager.h"

#include "../Scene/TitleScene.h"
#include "../Scene/StartingRuinSceneInfo.h"


CTitleSceneUI::CTitleSceneUI() :
	m_NowUIMode(ETitleUIMode::Main),
	m_MainSelected(EMainMenu::Start),
	m_SaveSelected(ESaveSelect::First),
	m_QuitSelected(EQuitMenu::Yes)
{
	m_WindowTypeName = "TitleSceneUI";
}

CTitleSceneUI::CTitleSceneUI(const CTitleSceneUI& Window) :
    CUIWindow(Window),
	m_NowUIMode(ETitleUIMode::Main),
	m_MainSelected(EMainMenu::Start),
	m_SaveSelected(ESaveSelect::First),
	m_QuitSelected(EQuitMenu::Yes)
{
}

CTitleSceneUI::~CTitleSceneUI()
{
}

void CTitleSceneUI::Start()
{
	CUIWindow::Start();

	CInput::GetInst()->AddBindFunction<CTitleSceneUI>("TitleKeyUp", Input_Type::Up, this, &CTitleSceneUI::TitleKeyUp, m_Scene);
	CInput::GetInst()->AddBindFunction<CTitleSceneUI>("TitleKeyDown", Input_Type::Up, this, &CTitleSceneUI::TitleKeyDown, m_Scene);
	CInput::GetInst()->AddBindFunction<CTitleSceneUI>("TitleKeyEnter", Input_Type::Up, this, &CTitleSceneUI::TitleKeyEnter, m_Scene);
	CInput::GetInst()->AddBindFunction<CTitleSceneUI>("TitleKeyBack", Input_Type::Up, this, &CTitleSceneUI::TitleKeyBack, m_Scene);
}

bool CTitleSceneUI::Init()
{
	CUIWindow::Init();

	// 메뉴별 UI 생성
	CreateTitleMainUI();
	CreateSaveSelectUI();
	CreateOptionUI();
	CreateQuitUI();

	// 시작 시점에는 메인 메뉴만 활성화 되어있게 한다.
	UIActive();

	return true;
}

void CTitleSceneUI::Update(float DeltaTime)
{
	CUIWindow::Update(DeltaTime);

	switch (m_NowUIMode)
	{
	case ETitleUIMode::Main:
	{

		CUIText* StartText = (CUIText*)(m_mapMainUI.find("StartText")->second).Get();
		CUIText* OptionText = (CUIText*)(m_mapMainUI.find("OptionText")->second).Get();
		CUIText* QuitText = (CUIText*)(m_mapMainUI.find("QuitText")->second).Get();
		
		CUIImage* SelectLeftImage = (CUIImage*)(m_mapMainUI.find("TitleSelectLeftImage")->second).Get();
		CUIImage* SelectRightImage = (CUIImage*)(m_mapMainUI.find("TitleSelectRightImage")->second).Get();


		switch (m_MainSelected)
		{
		case EMainMenu::Start:
			StartText->SetColor(Vector4::White);
			OptionText->SetColor(Vector4::Gray);
			QuitText->SetColor(Vector4::Gray);

			SelectLeftImage->SetPos(StartText->GetPos().x - SelectLeftImage->GetSize().x, StartText->GetPos().y);
			SelectRightImage->SetPos(StartText->GetPos().x + StartText->GetSize().x, StartText->GetPos().y);
			break;

		case EMainMenu::Option:
			StartText->SetColor(Vector4::Gray);
			OptionText->SetColor(Vector4::White);
			QuitText->SetColor(Vector4::Gray);

			SelectLeftImage->SetPos(OptionText->GetPos().x - SelectLeftImage->GetSize().x, OptionText->GetPos().y);
			SelectRightImage->SetPos(OptionText->GetPos().x + OptionText->GetSize().x, OptionText->GetPos().y);
			break;

		case EMainMenu::Quit:
			StartText->SetColor(Vector4::Gray);
			OptionText->SetColor(Vector4::Gray);
			QuitText->SetColor(Vector4::White);

			SelectLeftImage->SetPos(QuitText->GetPos().x - SelectLeftImage->GetSize().x, QuitText->GetPos().y);
			SelectRightImage->SetPos(QuitText->GetPos().x + QuitText->GetSize().x, QuitText->GetPos().y);
			break;
		}
		break;
	}
	case ETitleUIMode::SelectSave:
	{
		CUIText* BackText = (CUIText*)(m_mapSaveSelectUI.find("BackText")->second).Get();

		switch (m_SaveSelected)
		{
		case ESaveSelect::First:
			for (int i = 0; i < 4; i++) {
				int SaveSlotNum = i + 1;

				char SaveBoxName[40] = "SaveSelectSaveBox_";
				strcat_s(SaveBoxName, std::to_string(SaveSlotNum).c_str());
				CUIImage* SaveBox = (CUIImage*)(m_mapSaveSelectUI.find(SaveBoxName)->second).Get();

				char SaveBoxSlotName[40] = "SaveSelectSaveSlotNum_";
				strcat_s(SaveBoxSlotName, std::to_string(SaveSlotNum).c_str());
				CUIImage* SaveSlot = (CUIImage*)(m_mapSaveSelectUI.find(SaveBoxSlotName)->second).Get();

				char SaveBoxTextName[40] = "SaveSelectSaveText_";
				strcat_s(SaveBoxTextName, std::to_string(SaveSlotNum).c_str());
				CUIText* SaveBoxSlotText = (CUIText*)(m_mapSaveSelectUI.find(SaveBoxTextName)->second).Get();

				if (i == 0) {
					SaveBox->SetImageTint(Vector4::White);
					SaveSlot->SetImageTint(Vector4::White);
					SaveBoxSlotText->SetColor(Vector4::White);
				}
				else {
					SaveBox->SetImageTint(Vector4::Gray);
					SaveSlot->SetImageTint(Vector4::Gray);
					SaveBoxSlotText->SetColor(Vector4::Gray);
				}

				for (int BossNum = 0; BossNum < 4; BossNum++) {
					char SaveBoxBossIconName[40] = "SaveSelectBossIcon";
					strcat_s(SaveBoxBossIconName, std::to_string(BossNum).c_str());
					strcat_s(SaveBoxBossIconName, "_");
					strcat_s(SaveBoxBossIconName, std::to_string(SaveSlotNum).c_str());
					CUIImage* SaveBoxBossIcon = (CUIImage*)(m_mapSaveSelectUI.find(SaveBoxBossIconName)->second).Get();

					if (i == 0) {
						SaveBoxBossIcon->SetImageTint(Vector4::White);
					}
					else {
						SaveBoxBossIcon->SetImageTint(Vector4::Gray);
					}
				}
			}

			BackText->SetColor(Vector4::Gray);
			break;

		case ESaveSelect::Second:
			for (int i = 0; i < 4; i++) {
				int SaveSlotNum = i + 1;

				char SaveBoxName[40] = "SaveSelectSaveBox_";
				strcat_s(SaveBoxName, std::to_string(SaveSlotNum).c_str());
				CUIImage* SaveBox = (CUIImage*)(m_mapSaveSelectUI.find(SaveBoxName)->second).Get();

				char SaveBoxSlotName[40] = "SaveSelectSaveSlotNum_";
				strcat_s(SaveBoxSlotName, std::to_string(SaveSlotNum).c_str());
				CUIImage* SaveSlot = (CUIImage*)(m_mapSaveSelectUI.find(SaveBoxSlotName)->second).Get();

				char SaveBoxTextName[40] = "SaveSelectSaveText_";
				strcat_s(SaveBoxTextName, std::to_string(SaveSlotNum).c_str());
				CUIText* SaveBoxSlotText = (CUIText*)(m_mapSaveSelectUI.find(SaveBoxTextName)->second).Get();

				if (i == 1) {
					SaveBox->SetImageTint(Vector4::White);
					SaveSlot->SetImageTint(Vector4::White);
					SaveBoxSlotText->SetColor(Vector4::White);
				}
				else {
					SaveBox->SetImageTint(Vector4::Gray);
					SaveSlot->SetImageTint(Vector4::Gray);
					SaveBoxSlotText->SetColor(Vector4::Gray);
				}

				for (int BossNum = 0; BossNum < 4; BossNum++) {
					char SaveBoxBossIconName[40] = "SaveSelectBossIcon";
					strcat_s(SaveBoxBossIconName, std::to_string(BossNum).c_str());
					strcat_s(SaveBoxBossIconName, "_");
					strcat_s(SaveBoxBossIconName, std::to_string(SaveSlotNum).c_str());
					CUIImage* SaveBoxBossIcon = (CUIImage*)(m_mapSaveSelectUI.find(SaveBoxBossIconName)->second).Get();

					if (i == 1) {
						SaveBoxBossIcon->SetImageTint(Vector4::White);
					}
					else {
						SaveBoxBossIcon->SetImageTint(Vector4::Gray);
					}
				}
			}

			BackText->SetColor(Vector4::Gray);
			break;

		case ESaveSelect::Third:
			for (int i = 0; i < 4; i++) {
				int SaveSlotNum = i + 1;

				char SaveBoxName[40] = "SaveSelectSaveBox_";
				strcat_s(SaveBoxName, std::to_string(SaveSlotNum).c_str());
				CUIImage* SaveBox = (CUIImage*)(m_mapSaveSelectUI.find(SaveBoxName)->second).Get();

				char SaveBoxSlotName[40] = "SaveSelectSaveSlotNum_";
				strcat_s(SaveBoxSlotName, std::to_string(SaveSlotNum).c_str());
				CUIImage* SaveSlot = (CUIImage*)(m_mapSaveSelectUI.find(SaveBoxSlotName)->second).Get();

				char SaveBoxTextName[40] = "SaveSelectSaveText_";
				strcat_s(SaveBoxTextName, std::to_string(SaveSlotNum).c_str());
				CUIText* SaveBoxSlotText = (CUIText*)(m_mapSaveSelectUI.find(SaveBoxTextName)->second).Get();

				if (i == 2) {
					SaveBox->SetImageTint(Vector4::White);
					SaveSlot->SetImageTint(Vector4::White);
					SaveBoxSlotText->SetColor(Vector4::White);
				}
				else {
					SaveBox->SetImageTint(Vector4::Gray);
					SaveSlot->SetImageTint(Vector4::Gray);
					SaveBoxSlotText->SetColor(Vector4::Gray);
				}

				for (int BossNum = 0; BossNum < 4; BossNum++) {
					char SaveBoxBossIconName[40] = "SaveSelectBossIcon";
					strcat_s(SaveBoxBossIconName, std::to_string(BossNum).c_str());
					strcat_s(SaveBoxBossIconName, "_");
					strcat_s(SaveBoxBossIconName, std::to_string(SaveSlotNum).c_str());
					CUIImage* SaveBoxBossIcon = (CUIImage*)(m_mapSaveSelectUI.find(SaveBoxBossIconName)->second).Get();

					if (i == 2) {
						SaveBoxBossIcon->SetImageTint(Vector4::White);
					}
					else {
						SaveBoxBossIcon->SetImageTint(Vector4::Gray);
					}
				}
			}

			BackText->SetColor(Vector4::Gray);
			break;

		case ESaveSelect::Fourth:
			for (int i = 0; i < 4; i++) {
				int SaveSlotNum = i + 1;

				char SaveBoxName[40] = "SaveSelectSaveBox_";
				strcat_s(SaveBoxName, std::to_string(SaveSlotNum).c_str());
				CUIImage* SaveBox = (CUIImage*)(m_mapSaveSelectUI.find(SaveBoxName)->second).Get();

				char SaveBoxSlotName[40] = "SaveSelectSaveSlotNum_";
				strcat_s(SaveBoxSlotName, std::to_string(SaveSlotNum).c_str());
				CUIImage* SaveSlot = (CUIImage*)(m_mapSaveSelectUI.find(SaveBoxSlotName)->second).Get();

				char SaveBoxTextName[40] = "SaveSelectSaveText_";
				strcat_s(SaveBoxTextName, std::to_string(SaveSlotNum).c_str());
				CUIText* SaveBoxSlotText = (CUIText*)(m_mapSaveSelectUI.find(SaveBoxTextName)->second).Get();

				if (i == 3) {
					SaveBox->SetImageTint(Vector4::White);
					SaveSlot->SetImageTint(Vector4::White);
					SaveBoxSlotText->SetColor(Vector4::White);
				}
				else {
					SaveBox->SetImageTint(Vector4::Gray);
					SaveSlot->SetImageTint(Vector4::Gray);
					SaveBoxSlotText->SetColor(Vector4::Gray);
				}

				for (int BossNum = 0; BossNum < 4; BossNum++) {
					char SaveBoxBossIconName[40] = "SaveSelectBossIcon";
					strcat_s(SaveBoxBossIconName, std::to_string(BossNum).c_str());
					strcat_s(SaveBoxBossIconName, "_");
					strcat_s(SaveBoxBossIconName, std::to_string(SaveSlotNum).c_str());
					CUIImage* SaveBoxBossIcon = (CUIImage*)(m_mapSaveSelectUI.find(SaveBoxBossIconName)->second).Get();

					if (i == 3) {
						SaveBoxBossIcon->SetImageTint(Vector4::White);
					}
					else {
						SaveBoxBossIcon->SetImageTint(Vector4::Gray);
					}
				}
			}

			BackText->SetColor(Vector4::Gray);
			break;

		case ESaveSelect::Back:
			for (int i = 0; i < 4; i++) {
				int SaveSlotNum = i + 1;

				char SaveBoxName[40] = "SaveSelectSaveBox_";
				strcat_s(SaveBoxName, std::to_string(SaveSlotNum).c_str());
				CUIImage* SaveBox = (CUIImage*)(m_mapSaveSelectUI.find(SaveBoxName)->second).Get();

				char SaveBoxSlotName[40] = "SaveSelectSaveSlotNum_";
				strcat_s(SaveBoxSlotName, std::to_string(SaveSlotNum).c_str());
				CUIImage* SaveSlot = (CUIImage*)(m_mapSaveSelectUI.find(SaveBoxSlotName)->second).Get();

				char SaveBoxTextName[40] = "SaveSelectSaveText_";
				strcat_s(SaveBoxTextName, std::to_string(SaveSlotNum).c_str());
				CUIText* SaveBoxSlotText = (CUIText*)(m_mapSaveSelectUI.find(SaveBoxTextName)->second).Get();

				char SaveBoxBossIcon[40] = "SaveSelectBossIcon";
				strcat_s(SaveBoxBossIcon, std::to_string(SaveSlotNum).c_str());
				strcat_s(SaveBoxBossIcon, "_");
				strcat_s(SaveBoxBossIcon, std::to_string(SaveSlotNum).c_str());

				SaveBox->SetImageTint(Vector4::Gray);
				SaveSlot->SetImageTint(Vector4::Gray);
				SaveBoxSlotText->SetColor(Vector4::Gray);


				for (int BossNum = 0; BossNum < 4; BossNum++) {
					char SaveBoxBossIconName[40] = "SaveSelectBossIcon";
					strcat_s(SaveBoxBossIconName, std::to_string(BossNum).c_str());
					strcat_s(SaveBoxBossIconName, "_");
					strcat_s(SaveBoxBossIconName, std::to_string(SaveSlotNum).c_str());
					CUIImage* SaveBoxBossIcon = (CUIImage*)(m_mapSaveSelectUI.find(SaveBoxBossIconName)->second).Get();

					SaveBoxBossIcon->SetImageTint(Vector4::Gray);
				}
			}

			BackText->SetColor(Vector4::White);
			break;

		}
		break;
	}
	case ETitleUIMode::Option:

		break;
	case ETitleUIMode::Quit:
	{
		CUIText* YesText = (CUIText*)(m_mapQuitUI.find("YesText")->second).Get();
		CUIText* NoText = (CUIText*)(m_mapQuitUI.find("NoText")->second).Get();

		CUIImage* SelectLeftImage = (CUIImage*)(m_mapQuitUI.find("QuitSelectLeftImage")->second).Get();
		CUIImage* SelectRightImage = (CUIImage*)(m_mapQuitUI.find("QuitSelectRightImage")->second).Get();
		switch (m_QuitSelected)
		{
		case EQuitMenu::Yes:
			YesText->SetColor(Vector4::White);
			NoText->SetColor(Vector4::Gray);

			SelectLeftImage->SetPos(YesText->GetPos().x - SelectLeftImage->GetSize().x, YesText->GetPos().y);
			SelectRightImage->SetPos(YesText->GetPos().x + YesText->GetSize().x, YesText->GetPos().y);
			break;

		case EQuitMenu::No:
			YesText->SetColor(Vector4::Gray);
			NoText->SetColor(Vector4::White);

			SelectLeftImage->SetPos(NoText->GetPos().x - SelectLeftImage->GetSize().x, NoText->GetPos().y);
			SelectRightImage->SetPos(NoText->GetPos().x + NoText->GetSize().x, NoText->GetPos().y);
			break;
		}

		break;
	}
	}
}

void CTitleSceneUI::PostUpdate(float DeltaTime)
{
	CUIWindow::PostUpdate(DeltaTime);
}

void CTitleSceneUI::Render()
{
	CUIWindow::Render();
}

CTitleSceneUI* CTitleSceneUI::Clone()
{
	return new CTitleSceneUI(*this);
}

void CTitleSceneUI::Save(FILE* File)
{
	CUIWindow::Save(File);
}

void CTitleSceneUI::Load(FILE* File)
{
	CUIWindow::Load(File);
}

void CTitleSceneUI::UIActive()
{
	switch (m_NowUIMode)
	{
	case ETitleUIMode::Main:
		for (auto iter : m_mapMainUI) {
			iter.second->SetEnable(true);
		}

		for (auto iter : m_mapSaveSelectUI) {
			iter.second->SetEnable(false);
		}

		for (auto iter : m_mapOptionUI) {
			iter.second->SetEnable(false);
		}

		for (auto iter : m_mapQuitUI) {
			iter.second->SetEnable(false);
		}
		break;
	case ETitleUIMode::SelectSave:
		for (auto iter : m_mapMainUI) {
			iter.second->SetEnable(false);
		}

		for (auto iter : m_mapSaveSelectUI) {
			iter.second->SetEnable(true);
		}

		for (auto iter : m_mapOptionUI) {
			iter.second->SetEnable(false);
		}

		for (auto iter : m_mapQuitUI) {
			iter.second->SetEnable(false);
		}
		break;
	case ETitleUIMode::Option:
		for (auto iter : m_mapMainUI) {
			iter.second->SetEnable(false);
		}

		for (auto iter : m_mapSaveSelectUI) {
			iter.second->SetEnable(false);
		}

		for (auto iter : m_mapOptionUI) {
			iter.second->SetEnable(true);
		}

		for (auto iter : m_mapQuitUI) {
			iter.second->SetEnable(false);
		}
		break;
	case ETitleUIMode::Quit:
		for (auto iter : m_mapMainUI) {
			iter.second->SetEnable(false);
		}

		for (auto iter : m_mapSaveSelectUI) {
			iter.second->SetEnable(false);
		}

		for (auto iter : m_mapOptionUI) {
			iter.second->SetEnable(false);
		}

		for (auto iter : m_mapQuitUI) {
			iter.second->SetEnable(true);
		}
		break;
	}
}

void CTitleSceneUI::CreateTitleMainUI()
{
	// 타이틀 이미지. TitanSouls 타이틀 이미지를 가져다가 사용한다.
	CUIImage* TitleImage = CreateWidget<CUIImage>("TitleImage");

	TitleImage->SetTexture("TitleImageLogo", TEXT("IMG/TITLE/title.png"));
	TitleImage->AddFrameData(Vector2(0.f, 0.f), Vector2(275.f, 125.f));

	TitleImage->SetSize(550.f, 250.f);
	TitleImage->SetPos(1280.f/2.f - TitleImage->GetSize().x/2.f, 400.f);

	m_mapMainUI.insert(std::make_pair("TitleImage", TitleImage));


	// 메뉴에서 셀렉트된 메뉴 양측에 표시될 문양
	CUIImage* SelectLeftImage = CreateWidget<CUIImage>("TitleSelectLeftImage");
	SelectLeftImage->SetSize(50.f, 50.f);
	SelectLeftImage->SetPos(540.f, 300.f);
	SelectLeftImage->SetTexture("TitleSelectLeftImage", TEXT("IMG/TITLE/title.png"));
	SelectLeftImage->AddFrameData(Vector2(65.f, 290.f), Vector2(80.f, 305.f));

	m_mapMainUI.insert(std::make_pair("TitleSelectLeftImage", SelectLeftImage));

	CUIImage* SelectRightImage = CreateWidget<CUIImage>("TitleSelectRightImage");
	SelectRightImage->SetSize(50.f, 50.f);
	SelectRightImage->SetPos(740.f, 300.f);
	SelectRightImage->SetTexture("TitleSelectRightImage", TEXT("IMG/TITLE/title.png"));
	SelectRightImage->AddFrameData(Vector2(80.f, 290.f), Vector2(95.f, 305.f));

	m_mapMainUI.insert(std::make_pair("TitleSelectRightImage", SelectRightImage));



	CUIText* StartText = CreateWidget<CUIText>("TitleStart");

	StartText->SetSize(200.f, 50.f);
	StartText->SetAlignH(Text_Align_H::Center);
	StartText->SetPos(540.f, 300.f);
	StartText->SetFontSize(30.f);
	StartText->SetText(TEXT("START"));
	StartText->SetTransparency(false);
	StartText->SetShadowEnable(false);

	m_mapMainUI.insert(std::make_pair("StartText", StartText));


	CUIText* OptionText = CreateWidget<CUIText>("TitleOption");

	OptionText->SetSize(200.f, 50.f);
	OptionText->SetAlignH(Text_Align_H::Center);
	OptionText->SetPos(540.f, 250.f);
	OptionText->SetFontSize(30.f);
	OptionText->SetText(TEXT("OPTION"));
	OptionText->SetTransparency(false);
	OptionText->SetShadowEnable(false);

	m_mapMainUI.insert(std::make_pair("OptionText", OptionText));


	CUIText* QuitText = CreateWidget<CUIText>("TitleQuit");

	QuitText->SetSize(200.f, 50.f);
	QuitText->SetAlignH(Text_Align_H::Center);
	QuitText->SetPos(540.f, 200.f);
	QuitText->SetFontSize(30.f);
	QuitText->SetText(TEXT("QUIT"));
	QuitText->SetTransparency(false);
	QuitText->SetShadowEnable(false);

	m_mapMainUI.insert(std::make_pair("QuitText", QuitText));
}

void CTitleSceneUI::CreateSaveSelectUI()
{
	CUIText* TitleText = CreateWidget<CUIText>("SaveSelectTitle");

	TitleText->SetSize(200.f, 50.f);
	TitleText->SetAlignH(Text_Align_H::Center);
	TitleText->SetAlignV(Text_Align_V::Bottom);
	TitleText->SetPos(540.f, 650.f);
	TitleText->SetFontSize(20.f);
	TitleText->SetText(TEXT("Select a Save Slot"));

	m_mapSaveSelectUI.insert(std::make_pair("SaveSelectTitle", TitleText));


	CUIImage* TitleWing = CreateWidget<CUIImage>("SaveSelectTItleLeftImage");

	TitleWing->SetTexture("SaveSelectTItleLeftImage", TEXT("IMG/TITLE/title.png"));
	TitleWing->AddFrameData(Vector2(0.f, 274.f), Vector2(112.f, 287.f));
	TitleWing->SetSize(224.f, 26.f);
	TitleWing->SetPos(TitleText->GetPos().x - TitleWing->GetSize().x, TitleText->GetPos().y);

	m_mapSaveSelectUI.insert(std::make_pair("SaveSelectTItleLeftImage", TitleWing));


	TitleWing = CreateWidget<CUIImage>("SaveSelectTItleRightImage");

	TitleWing->SetTexture("SaveSelectTItleRightImage", TEXT("IMG/TITLE/title.png"));
	TitleWing->AddFrameData(Vector2(112.f, 274.f), Vector2(224.f, 287.f));
	TitleWing->SetSize(224.f, 26.f);
	TitleWing->SetPos(TitleText->GetPos().x + TitleText->GetSize().x, TitleText->GetPos().y);

	m_mapSaveSelectUI.insert(std::make_pair("SaveSelectTItleRightImage", TitleWing));




	for (int i = 0; i < 4; i++) {
		int SlotNum = i + 1;

		char SaveBoxName[40] = "SaveSelectSaveBox_";
		strcat_s(SaveBoxName, std::to_string(SlotNum).c_str());

		CUIImage* SaveBox = CreateWidget<CUIImage>(SaveBoxName);

		SaveBox->SetTexture(SaveBoxName, TEXT("IMG/TITLE/title.png"));
		SaveBox->AddFrameData(Vector2(0.f, 140.f), Vector2(225.f, 195.f));

		SaveBox->SetSize(450.f, 110.f);

		float PosX = 640.f - SaveBox->GetSize().x / 2.f;
		float PosY = TitleText->GetPos().y - TitleText->GetSize().y - 100.f - SaveBox->GetSize().y * 1.1f * i;
		SaveBox->SetPos(PosX, PosY);

		m_mapSaveSelectUI.insert(std::make_pair(SaveBoxName, SaveBox));



		char SaveSlotNumName[40] = "SaveSelectSaveSlotNum_";
		strcat_s(SaveSlotNumName, std::to_string(SlotNum).c_str());

		CUIImage* SaveSlotNum = CreateWidget<CUIImage>(SaveSlotNumName);

		SaveSlotNum->SetTexture(SaveBoxName, TEXT("IMG/TITLE/title.png"));
		SaveSlotNum->AddFrameData(Vector2(50.f * i, 215.f), Vector2(50.f * SlotNum, 250.f));

		SaveSlotNum->SetSize(100.f, 70.f);

		PosX = SaveBox->GetPos().x + 50.f;
		PosY = SaveBox->GetPos().y + (SaveBox->GetSize().y - SaveSlotNum->GetSize().y)/2.f;
		SaveSlotNum->SetPos(PosX, PosY);

		m_mapSaveSelectUI.insert(std::make_pair(SaveSlotNumName, SaveSlotNum));


		char SaveSlotTextName[40] = "SaveSelectSaveText_";
		strcat_s(SaveSlotTextName, std::to_string(SlotNum).c_str());

		// 세이브가 있으면 불러와서 체크.
		// 없으면 new Game. 일단은 New 게임만 구현
		if (true) { // 분기처리를 위해 일단 true로 무조건 세이브가 없는 경우를 처리하게끔 작업.
			CUIText* SaveText = CreateWidget<CUIText>(SaveSlotTextName);
			SaveText->SetSize(SaveBox->GetSize());
			SaveText->SetPos(SaveBox->GetPos());
			SaveText->SetFontSize(23.f);
			SaveText->SetAlignH(Text_Align_H::Center);
			SaveText->SetAlignV(Text_Align_V::Middle);
			SaveText->SetText(TEXT("NEW GAME"));

			m_mapSaveSelectUI.insert(std::make_pair(SaveSlotTextName, SaveText));
		}
		else { // 세이브가 있는 경우를 이곳에 구현

		}



		for (int BossNum = 0; BossNum < 4; BossNum++) {
			char SaveBoxBossIcon[40] = "SaveSelectBossIcon";

			strcat_s(SaveBoxBossIcon, std::to_string(BossNum).c_str());
			strcat_s(SaveBoxBossIcon, "_");
			strcat_s(SaveBoxBossIcon, std::to_string(SlotNum).c_str());


			CUIImage* BossIcon = CreateWidget<CUIImage>(SaveBoxBossIcon);

			BossIcon->SetTexture(SaveBoxBossIcon, TEXT("IMG/TITLE/title.png"));

			float FramePosX = 240.f + 16.f * (BossNum % 2);
			float FramePosY = 145.f + 16.f * (BossNum / 2);
			BossIcon->AddFrameData(Vector2(FramePosX, FramePosY), Vector2(FramePosX + 16.f, FramePosY + 16.f));

			PosX = SaveBox->GetPos().x + SaveBox->GetSize().x / 5.f * 3.5f;
			PosX += 32.f * (BossNum % 2);
			PosY = SaveSlotNum->GetPos().y + 32.f * (BossNum / 2);

			BossIcon->SetSize(32.f, 32.f);
			BossIcon->SetPos(PosX, PosY);

			m_mapSaveSelectUI.insert(std::make_pair(SaveBoxBossIcon, BossIcon));
		}

	}


	CUIText* BackText = CreateWidget<CUIText>("SaveSelectBackText");

	BackText->SetSize(300.f, 100.f);
	BackText->SetAlignH(Text_Align_H::Center);
	BackText->SetAlignV(Text_Align_V::Middle);
	BackText->SetPos(490.f, 50.f);
	BackText->SetFontSize(50.f);
	BackText->SetText(TEXT("Back"));
	BackText->SetTransparency(false);
	BackText->SetShadowEnable(false);

	m_mapSaveSelectUI.insert(std::make_pair("BackText", BackText));
}

void CTitleSceneUI::CreateOptionUI()
{
}

void CTitleSceneUI::CreateQuitUI()
{
	CUIText* TitleText = CreateWidget<CUIText>("QuitTitle");

	TitleText->SetSize(200.f, 50.f);
	TitleText->SetAlignH(Text_Align_H::Center);
	TitleText->SetAlignV(Text_Align_V::Bottom);
	TitleText->SetPos(540.f, 500.f);
	TitleText->SetFontSize(20.f);
	TitleText->SetText(TEXT("Exit Game?"));
	TitleText->SetTransparency(false);
	TitleText->SetShadowEnable(false);

	m_mapQuitUI.insert(std::make_pair("QuitTitle", TitleText));


	CUIImage* TitleLeftImage = CreateWidget<CUIImage>("QuitTitleLeftImage");

	TitleLeftImage->SetTexture("QuitTitleLeftImage", TEXT("IMG/TITLE/title.png"));
	TitleLeftImage->AddFrameData(Vector2(0.f, 274.f), Vector2(112.f, 287.f));
	TitleLeftImage->SetSize(224.f, 26.f);
	TitleLeftImage->SetPos(TitleText->GetPos().x - TitleLeftImage->GetSize().x, TitleText->GetPos().y);

	m_mapQuitUI.insert(std::make_pair("QuitTitleLeftImage", TitleLeftImage));


	CUIImage* TitleRightImage = CreateWidget<CUIImage>("QuitTitleRightImage");

	TitleRightImage->SetTexture("QuitTitleRightImage", TEXT("IMG/TITLE/title.png"));
	TitleRightImage->AddFrameData(Vector2(112.f, 274.f), Vector2(224.f, 287.f));
	TitleRightImage->SetSize(224.f, 26.f);
	TitleRightImage->SetPos(TitleText->GetPos().x + TitleText->GetSize().x, TitleText->GetPos().y);

	m_mapQuitUI.insert(std::make_pair("QuitTitleRightImage", TitleRightImage));



	// 메뉴에서 셀렉트된 메뉴 양측에 표시될 문양
	CUIImage* SelectLeftImage = CreateWidget<CUIImage>("QuitSelectLeftImage");
	SelectLeftImage->SetSize(25.f, 25.f);
	SelectLeftImage->SetTexture("QuitSelectLeftImage", TEXT("IMG/TITLE/title.png"));
	SelectLeftImage->AddFrameData(Vector2(65.f, 290.f), Vector2(80.f, 305.f));

	m_mapQuitUI.insert(std::make_pair("QuitSelectLeftImage", SelectLeftImage));

	CUIImage* SelectRightImage = CreateWidget<CUIImage>("QuitSelectRightImage");
	SelectRightImage->SetSize(25.f, 25.f);
	SelectRightImage->SetTexture("QuitSelectRightImage", TEXT("IMG/TITLE/title.png"));
	SelectRightImage->AddFrameData(Vector2(80.f, 290.f), Vector2(95.f, 305.f));

	m_mapQuitUI.insert(std::make_pair("QuitSelectRightImage", SelectRightImage));



	CUIText* YesText = CreateWidget<CUIText>("QuitYes");

	YesText->SetSize(200.f, 50.f);
	YesText->SetAlignH(Text_Align_H::Center);
	YesText->SetAlignV(Text_Align_V::Bottom);
	YesText->SetPos(540.f, 325.f);
	YesText->SetFontSize(20.f);
	YesText->SetText(TEXT("YES"));

	m_mapQuitUI.insert(std::make_pair("YesText", YesText));


	CUIText* NoText = CreateWidget<CUIText>("QuitNo");

	NoText->SetSize(200.f, 50.f);
	NoText->SetAlignH(Text_Align_H::Center);
	NoText->SetAlignV(Text_Align_V::Bottom);
	NoText->SetPos(540.f, 250.f);
	NoText->SetFontSize(20.f);
	NoText->SetText(TEXT("NO"));

	m_mapQuitUI.insert(std::make_pair("NoText", NoText));
}

void CTitleSceneUI::TitleKeyUp()
{
	m_Scene->GetResource()->FindSound("Move")->Play();

	switch (m_NowUIMode)
	{
	case ETitleUIMode::Main:
		switch (m_MainSelected)
		{
		case EMainMenu::Start:
			m_MainSelected = EMainMenu::Quit;
			break;
		case EMainMenu::Option:
			m_MainSelected = EMainMenu::Start;
			break;
		case EMainMenu::Quit:
			m_MainSelected = EMainMenu::Option;
			break;
		}
		break;
	case ETitleUIMode::SelectSave:
		switch (m_SaveSelected)
		{
		case ESaveSelect::First:
			m_SaveSelected = ESaveSelect::Back;
			break;
		case ESaveSelect::Second:
			m_SaveSelected = ESaveSelect::First;
			break;
		case ESaveSelect::Third:
			m_SaveSelected = ESaveSelect::Second;
			break;
		case ESaveSelect::Fourth:
			m_SaveSelected = ESaveSelect::Third;
			break;
		case ESaveSelect::Back:
			m_SaveSelected = ESaveSelect::Fourth;
			break;
		}
		break;
	case ETitleUIMode::Option:
		break;
	case ETitleUIMode::Quit:
		if (m_QuitSelected == EQuitMenu::Yes) {
			m_QuitSelected = EQuitMenu::No;
		}
		else {
			m_QuitSelected = EQuitMenu::Yes;
		}
		break;
	}
}

void CTitleSceneUI::TitleKeyDown()
{
	m_Scene->GetResource()->FindSound("Move")->Play();

	switch (m_NowUIMode)
	{
	case ETitleUIMode::Main:
		switch (m_MainSelected)
		{
		case EMainMenu::Start:
			m_MainSelected = EMainMenu::Option;
			break;
		case EMainMenu::Option:
			m_MainSelected = EMainMenu::Quit;
			break;
		case EMainMenu::Quit:
			m_MainSelected = EMainMenu::Start;
			break;
		}
		break;
	case ETitleUIMode::SelectSave:
		switch (m_SaveSelected)
		{
		case ESaveSelect::First:
			m_SaveSelected = ESaveSelect::Second;
			break;
		case ESaveSelect::Second:
			m_SaveSelected = ESaveSelect::Third;
			break;
		case ESaveSelect::Third:
			m_SaveSelected = ESaveSelect::Fourth;
			break;
		case ESaveSelect::Fourth:
			m_SaveSelected = ESaveSelect::Back;
			break;
		case ESaveSelect::Back:
			m_SaveSelected = ESaveSelect::First;
			break;
		}
		break;
	case ETitleUIMode::Option:
		break;
	case ETitleUIMode::Quit:
		if (m_QuitSelected == EQuitMenu::Yes) {
			m_QuitSelected = EQuitMenu::No;
		}
		else {
			m_QuitSelected = EQuitMenu::Yes;
		}
		break;
	}
}

void CTitleSceneUI::TitleKeyEnter()
{

	switch (m_NowUIMode)
	{
	case ETitleUIMode::Main:
		switch (m_MainSelected)
		{
		case EMainMenu::Start:
			m_Scene->GetResource()->FindSound("Select")->Play();

			// 세이브를 고르는 UI로 변경
			m_NowUIMode = ETitleUIMode::SelectSave;
			UIActive();
			break;
		case EMainMenu::Option:
			m_Scene->GetResource()->FindSound("Select")->Play();

			// 옵션 UI로 변경
			m_NowUIMode = ETitleUIMode::Option;
			UIActive();
			break;
		case EMainMenu::Quit:
			m_Scene->GetResource()->FindSound("Select")->Play();

			// 종료 UI로 변경
			m_NowUIMode = ETitleUIMode::Quit;
			UIActive();
			break;
		}
		break;
	case ETitleUIMode::SelectSave:
		//
		switch (m_SaveSelected)
		{
		case ESaveSelect::First:
		{
			m_Scene->GetResource()->FindSound("StartGame")->Play();

			// 첫 번째 세이브를 불러온다.
			CSceneManager::GetInst()->CreateNextScene(true);
			CSceneManager::GetInst()->CreateSceneInfo<CStartingRuinSceneInfo>(false);

			break;
		}
		case ESaveSelect::Second:
			m_Scene->GetResource()->FindSound("StartGame")->Play();

			// 두 번째 세이브를 불러온다.
			CSceneManager::GetInst()->CreateNextScene(true);
			CSceneManager::GetInst()->CreateSceneInfo<CStartingRuinSceneInfo>(false);

			break;
		case ESaveSelect::Third:
			m_Scene->GetResource()->FindSound("StartGame")->Play();

			// 세 번째 세이브를 불러온다.
			CSceneManager::GetInst()->CreateNextScene(true);
			CSceneManager::GetInst()->CreateSceneInfo<CStartingRuinSceneInfo>(false);

			break;
		case ESaveSelect::Fourth:
			m_Scene->GetResource()->FindSound("StartGame")->Play();

			// 네 번째 세이브를 불러온다.
			CSceneManager::GetInst()->CreateNextScene(true);
			CSceneManager::GetInst()->CreateSceneInfo<CStartingRuinSceneInfo>(false);

			break;
		case ESaveSelect::Back:
			// 메인 메뉴로 되돌아간다.
			m_NowUIMode = ETitleUIMode::Main;
			UIActive();
			break;
		}

		break;
	case ETitleUIMode::Option:
		// 옵션 메뉴 구현

		break;
	case ETitleUIMode::Quit:
		switch (m_QuitSelected)
		{
		case EQuitMenu::Yes:
			m_Scene->GetResource()->FindSound("Select")->Play();

			// 게임을 종료한다.

			break;
		case EQuitMenu::No:
			m_Scene->GetResource()->FindSound("No")->Play();

			// 메인 메뉴로 되돌아간다.
			m_NowUIMode = ETitleUIMode::Main;
			UIActive();
			break;
		}
		break;
	}
}

void CTitleSceneUI::TitleKeyBack()
{
	m_Scene->GetResource()->FindSound("No")->Play();

	if (m_NowUIMode != ETitleUIMode::Main) {
		m_NowUIMode = ETitleUIMode::Main;

		// 타이틀 메인 UI 활성화.
		UIActive();
	}
}
