// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ScrSvrMain.h"
#include "ScrSvrScreen.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

// ---------------------------------------------------------------------------
__fastcall TfrmScreen::TfrmScreen(TComponent* Owner) : TForm(Owner) {
}

// ---------------------------------------------------------------------------
void __fastcall TfrmScreen::FormCreate(TObject *Sender) {
	Bitmap = new TBitmap;
}

// ---------------------------------------------------------------------------
void __fastcall TfrmScreen::FormDestroy(TObject *Sender) {
	delete Bitmap;
}

// ---------------------------------------------------------------------------
void __fastcall TfrmScreen::FormResize(TObject *Sender) {
	Bitmap->Width = ClientWidth;
	Bitmap->Height = ClientHeight;
}

// ---------------------------------------------------------------------------
void TfrmScreen::UpdatePosition() {
	TextLeft = Main->LeftRight +
		Random(Width - TextWidth - 2 * Main->LeftRight);
	TextTop = Main->TopBottom +
		Random(Height - TextHeight - 2 * Main->TopBottom);
}

// ---------------------------------------------------------------------------
void TfrmScreen::UpdateScreen() {
	Bitmap->Canvas->Font->Assign(Main->TimeFont);
	TextSize = Bitmap->Canvas->TextExtent(Main->sTime);
	TimeWidth = TextSize.cx;
	TimeHeight = TextSize.cy;

	Bitmap->Canvas->Font->Assign(Main->DateFont);
	TextSize = Bitmap->Canvas->TextExtent(Main->sDate);
	DateWidth = TextSize.cx;
	DateHeight = TextSize.cy;

	TextHeight = TimeHeight + DateHeight + 10;
	if ((TextTop + TextHeight + Main->TopBottom) > Height)
		TextTop = Height - TextHeight - Main->TopBottom;

	TimeTop = TextTop;
	DateTop = TextTop + TimeHeight + 10;

	if (DateWidth > TimeWidth)
		TextWidth = DateWidth;
	else
		TextWidth = TimeWidth;

	if ((TextLeft + TextWidth + Main->LeftRight) > Width)
		TextLeft = Width - TextWidth - Main->LeftRight;

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

	Bitmap->Canvas->Font->Assign(Main->TimeFont);
	Bitmap->Canvas->TextOut(TimeLeft, TimeTop, Main->sTime);

	Bitmap->Canvas->Font->Assign(Main->DateFont);
	Bitmap->Canvas->TextOut(DateLeft, DateTop, Main->sDate);

#ifdef _DEBUG
	Bitmap->Canvas->Brush->Color = clWhite;

	Bitmap->Canvas->FrameRect(Rect(Main->LeftRight, Main->TopBottom,
		Width - Main->LeftRight, Height - Main->TopBottom));

	Bitmap->Canvas->FrameRect(Rect(TimeLeft, TimeTop, TimeLeft + TimeWidth,
		TimeTop + TimeHeight));

	Bitmap->Canvas->FrameRect(Rect(DateLeft, DateTop, DateLeft + DateWidth,
		DateTop + DateHeight));
#endif

	BitBlt(Canvas->Handle, 0, 0, Width, Height, Bitmap->Canvas->Handle, 0, 0,
		SRCCOPY);
}
// ---------------------------------------------------------------------------
