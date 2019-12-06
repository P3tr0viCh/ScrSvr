// ---------------------------------------------------------------------------

#ifndef ScrSvrMainH
#define ScrSvrMainH
// ---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>

// ---------------------------------------------------------------------------
class TMain : public TForm {
__published: // IDE-managed Components

	TTimer *TimerDateTime;
	TTimer *TimerPosition;

	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall TimerDateTimeTimer(TObject *Sender);
	void __fastcall TimerPositionTimer(TObject *Sender);

private:
	TPoint CursorStartPoint;

	TBitmap * Bitmap;

	int TextLeft, TextTop, TextHeight, TextWidth;
	int TimeHeight, TimeWidth, TimeTop, TimeLeft;
	int DateHeight, DateWidth, DateTop, DateLeft;

	String sTime, sDate;

	TFont * TimeFont;
	TFont * DateFont;

	TSystemTime DateTime;

	TSize TextSize;

	bool CheckMessage(tagMSG &Msg);
	void __fastcall AppOnMessage(tagMSG &Msg, bool &Handled);

	void UpdatePosition();
	void UpdateScreen();

public:
	int MaxMouseMove;
	int LeftRight, TopBottom;

	__fastcall TMain(TComponent* Owner);
};

// ---------------------------------------------------------------------------
extern PACKAGE TMain *Main;
// ---------------------------------------------------------------------------
#endif
