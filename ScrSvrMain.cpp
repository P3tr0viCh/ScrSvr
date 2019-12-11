// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include <Vcl.Graphics.hpp>

#include "UtilsDate.h"
#include "UtilsFiles.h"
#include "UtilsFileIni.h"

#include "ScrSvrMain.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TMain *Main;

// ---------------------------------------------------------------------------
__fastcall TMain::TMain(TComponent* Owner) : TForm(Owner) {
}

bool TMain::CheckMessage(tagMSG &Msg) {
	switch (Msg.message) {
	case WM_MOUSEMOVE:
		return abs(LOWORD(Msg.lParam) - CursorStartPoint.x) > MaxMouseMove ||
			abs(HIWORD(Msg.lParam) - CursorStartPoint.y) > MaxMouseMove;

	case WM_KEYUP:

	case WM_LBUTTONUP:
	case WM_MBUTTONUP:
	case WM_RBUTTONUP:
	case WM_MOUSEWHEEL:

	case WM_ACTIVATE:
	case WM_ACTIVATEAPP:
	case WM_NCACTIVATE:
		return true;
	default:
		return false;
	}
}

void __fastcall TMain::AppOnMessage(tagMSG &Msg, bool &Handled) {
	if (CheckMessage(Msg)) {
		Close();
	}
}

// ---------------------------------------------------------------------------
void __fastcall TMain::FormCreate(TObject *Sender) {
	MaxMouseMove = 200;
	LeftRight = 50;
	TopBottom = 50;

	TimerDateTime->Interval = 1000;
	TimerPosition->Interval = 3000;

	Randomize();

	TimeFont = new TFont;
	DateFont = new TFont;

	TimeFont->Quality = TFontQuality::fqAntialiased;
	DateFont->Quality = TFontQuality::fqAntialiased;

	String FileNameIni = IncludeTrailingPathDelimiter
		(IncludeTrailingPathDelimiter(GetSpecialFolderPath(CSIDL_LOCAL_APPDATA))
		+ "P3tr0viCh");

	FileNameIni = FileNameIni + ChangeFileExt
		(ExtractFileName(Application->ExeName), ".ini");

	TFileIni * FileIni = TFileIni::GetNewInstance(FileNameIni);

	try {
		Color = TColor(FileIni->ReadInteger("Common", "BackColor", Color));

		DateFont->Color =
			TColor(FileIni->ReadInteger("Date", "FontColor", 0x0001B9FE));
		DateFont->Name = FileIni->ReadString("Date", "FontName", "Arial");
		DateFont->Size = FileIni->ReadInteger("Date", "FontSize", 36);

		TimeFont->Color =
			TColor(FileIni->ReadInteger("Time", "FontColor", clYellow));
		TimeFont->Name = FileIni->ReadString("Time", "FontName", "Arial");
		TimeFont->Size = FileIni->ReadInteger("Time", "FontSize", 100);
	}
	__finally {
		delete FileIni;
	}

	GetCursorPos(&CursorStartPoint);

	Application->OnMessage = AppOnMessage;

	ShowCursor(false);

	for (int i = 0; i < Screen->MonitorCount; i++) {
		ScreenForms.push_back(new TfrmScreen(this));

		SetWindowPos(ScreenForms[i]->Handle, HWND_TOP,
			Screen->Monitors[i]->Left, Screen->Monitors[i]->Top,
			Screen->Monitors[i]->Width, Screen->Monitors[i]->Height,
			SWP_SHOWWINDOW);
	}

	UpdatePositions();
	UpdateDateTime();
	UpdateScreens();

	TimerDateTime->Enabled = true;
	TimerPosition->Enabled = true;
}

// ---------------------------------------------------------------------------
void __fastcall TMain::FormDestroy(TObject *Sender) {
	TimerPosition->Enabled = false;
	TimerDateTime->Enabled = false;

	Application->OnMessage = NULL;

	delete DateFont;
	delete TimeFont;

	for (std::vector<TfrmScreen*>::iterator ScreenForm = ScreenForms.begin(),
		end = ScreenForms.end(); ScreenForm != end; ++ScreenForm) {
		delete *ScreenForm;
	}
	ScreenForms.clear();

	ShowCursor(true);
}

// ---------------------------------------------------------------------------
void __fastcall TMain::TimerDateTimeTimer(TObject *Sender) {
	UpdateDateTime();

	UpdateScreens();
}

// ---------------------------------------------------------------------------
void __fastcall TMain::TimerPositionTimer(TObject *Sender) {
	UpdatePositions();
}

// ---------------------------------------------------------------------------
void TMain::UpdateDateTime() {
	GetLocalTime(&DateTime);

	sTime = FormatTime("HH':'mm':'ss", DateTime);
	sDate = FormatDate("d' 'MMMM', 'dddd", DateTime);
}

// ---------------------------------------------------------------------------
void TMain::UpdatePositions() {
	for (std::vector<TfrmScreen*>::iterator ScreenForm = ScreenForms.begin(),
		end = ScreenForms.end(); ScreenForm != end; ++ScreenForm) {
		(*ScreenForm)->UpdatePosition();
	}
}

// ---------------------------------------------------------------------------
void TMain::UpdateScreens() {
	for (std::vector<TfrmScreen*>::iterator ScreenForm = ScreenForms.begin(),
		end = ScreenForms.end(); ScreenForm != end; ++ScreenForm) {
		(*ScreenForm)->UpdateScreen();
	}
}
// ---------------------------------------------------------------------------
