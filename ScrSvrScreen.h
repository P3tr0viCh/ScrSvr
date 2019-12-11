// ---------------------------------------------------------------------------

#ifndef ScrSvrScreenH
#define ScrSvrScreenH
// ---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>

// ---------------------------------------------------------------------------
class TfrmScreen : public TForm {
__published: // IDE-managed Components

	void __fastcall FormResize(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);

private:

	TBitmap * Bitmap;

	TSize TextSize;

	int TextLeft, TextTop, TextHeight, TextWidth;
	int TimeHeight, TimeWidth, TimeTop, TimeLeft;
	int DateHeight, DateWidth, DateTop, DateLeft;

public:
	__fastcall TfrmScreen(TComponent* Owner);

	void UpdatePosition();
	void UpdateScreen();
};

// ---------------------------------------------------------------------------
extern PACKAGE TfrmScreen *frmScreen;
// ---------------------------------------------------------------------------
#endif
