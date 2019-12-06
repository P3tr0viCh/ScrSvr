// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include <Vcl.Graphics.hpp>

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

	Bitmap = new TBitmap;

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
		Color = FileIni->ReadInteger("Common", "BackColor", Color);

		DateFont->Color = FileIni->ReadInteger("Date", "FontColor", 0x0001B9FE);
		DateFont->Name = FileIni->ReadString("Date", "FontName", "Arial");
		DateFont->Size = FileIni->ReadInteger("Date", "FontSize", 36);

		TimeFont->Color = FileIni->ReadInteger("Time", "FontColor", clYellow);
		TimeFont->Name = FileIni->ReadString("Time", "FontName", "Arial");
		TimeFont->Size = FileIni->ReadInteger("Time", "FontSize", 100);
	}
	__finally {
		delete FileIni;
	}

	GetCursorPos(&CursorStartPoint);

	Application->OnMessage = AppOnMessage;

	ShowCursor(false);

	UpdatePosition();
	UpdateScreen();

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

	delete Bitmap;

	ShowCursor(true);
}

// ---------------------------------------------------------------------------
void __fastcall TMain::FormResize(TObject *Sender) {
	Bitmap->Width = ClientWidth;
	Bitmap->Height = ClientHeight;
}

// ---------------------------------------------------------------------------
void __fastcall TMain::TimerDateTimeTimer(TObject *Sender) {
	UpdateScreen();
}

// ---------------------------------------------------------------------------
void __fastcall TMain::TimerPositionTimer(TObject *Sender) {
	UpdatePosition();
}

// ---------------------------------------------------------------------------
void TMain::UpdatePosition() {
	TextLeft = LeftRight + Random(Width - TextWidth - 2 * LeftRight);
	TextTop = TopBottom + Random(Height - TextHeight - 2 * TopBottom);
}

String FormatDate(String Format, TSystemTime SystemTime) {
	LPCTSTR lpFormat;
	DWORD Flags;
	WCHAR Buffer[255];

	if (Format == NULL || Format.IsEmpty()) {
		lpFormat = NULL;
		Flags = DATE_LONGDATE;
	}
	else {
		lpFormat = Format.w_str();
		Flags = 0;
	}

	GetDateFormat(LOCALE_USER_DEFAULT, Flags, &SystemTime, lpFormat, Buffer,
		sizeof(Buffer));

	return Buffer;
}

String FormatTime(String Format, TSystemTime SystemTime) {
	LPCTSTR lpFormat;
	DWORD Flags;
	WCHAR Buffer[255];

	Flags = 0;
	if (Format == NULL || Format.IsEmpty())
		lpFormat = NULL;
	else
		lpFormat = Format.w_str();

	GetTimeFormat(LOCALE_USER_DEFAULT, Flags, &SystemTime, lpFormat, Buffer,
		sizeof(Buffer));

	return Buffer;
}

void TMain::UpdateScreen() {
	GetLocalTime(&DateTime);

	sTime = FormatTime("HH':'mm':'ss", DateTime);
	sDate = FormatDate("d' 'MMMM', 'dddd", DateTime);

	Bitmap->Canvas->Font->Assign(TimeFont);
	TextSize = Bitmap->Canvas->TextExtent(sTime);
	TimeWidth = TextSize.cx;
	TimeHeight = TextSize.cy;

	Bitmap->Canvas->Font->Assign(DateFont);
	TextSize = Bitmap->Canvas->TextExtent(sDate);
	DateWidth = TextSize.cx;
	DateHeight = TextSize.cy;

	TextHeight = TimeHeight + DateHeight + 10;
	if ((TextTop + TextHeight + TopBottom) > Height)
		TextTop = Height - TextHeight - TopBottom;

	TimeTop = TextTop;
	DateTop = TextTop + TimeHeight + 10;

	if (DateWidth > TimeWidth)
		TextWidth = DateWidth;
	else
		TextWidth = TimeWidth;

	if ((TextLeft + TextWidth + LeftRight) > Width)
		TextLeft = Width - TextWidth - LeftRight;

	if (DateWidth > TimeWidth) {
		DateLeft = TextLeft;
		TimeLeft = TextLeft + ((TextWidth - TimeWidth) / 2);
	}
	else {
		TimeLeft = TextLeft;
		DateLeft = TextLeft + ((TextWidth - DateWidth) / 2);
	}

	Bitmap->Canvas->Brush->Style = bsSolid;
	Bitmap->Canvas->Brush->Color = Color;

	Bitmap->Canvas->FillRect(Bitmap->Canvas->ClipRect);

	Bitmap->Canvas->Font->Assign(TimeFont);
	Bitmap->Canvas->TextOut(TimeLeft, TimeTop, sTime);

	Bitmap->Canvas->Font->Assign(DateFont);
	Bitmap->Canvas->TextOut(DateLeft, DateTop, sDate);

#ifdef _DEBUG
	Bitmap->Canvas->Brush->Color = clWhite;

	Bitmap->Canvas->FrameRect(Rect(LeftRight, TopBottom, Width - LeftRight,
		Height - TopBottom));

	Bitmap->Canvas->FrameRect(Rect(TimeLeft, TimeTop, TimeLeft + TimeWidth,
		TimeTop + TimeHeight));

	Bitmap->Canvas->FrameRect(Rect(DateLeft, DateTop, DateLeft + DateWidth,
		DateTop + DateHeight));
#endif

	BitBlt(Canvas->Handle, 0, 0, Width, Height, Bitmap->Canvas->Handle, 0, 0,
		SRCCOPY);
}
