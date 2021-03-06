// ---------------------------------------------------------------------------

#ifndef ScrSvrMainH
#define ScrSvrMainH
// ---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>

#include <vector>

#include "ScrSvrScreen.h"

// ---------------------------------------------------------------------------
class TMain : public TForm {
__published: // IDE-managed Components

	TTimer *TimerDateTime;
	TTimer *TimerPosition;

	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall TimerDateTimeTimer(TObject *Sender);
	void __fastcall TimerPositionTimer(TObject *Sender);

private:
	TPoint CursorStartPoint;

	TSystemTime DateTime;

	bool CheckMessage(tagMSG &Msg);
	void __fastcall AppOnMessage(tagMSG &Msg, bool &Handled);

	void UpdateDateTime();

	void UpdatePositions();
	void UpdateScreens();

public:
	int MaxMouseMove;
	int LeftRight, TopBottom;

	String sTime, sDate;

	TFont * TimeFont;
	TFont * DateFont;

	std::vector<TfrmScreen*>ScreenForms;

	__fastcall TMain(TComponent* Owner);
};

// ---------------------------------------------------------------------------
extern PACKAGE TMain *Main;
// ---------------------------------------------------------------------------
#endif
