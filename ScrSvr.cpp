// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <tchar.h>

#include <windows.h>

// ---------------------------------------------------------------------------
USEFORM("ScrSvrMain.cpp", Main);

// ---------------------------------------------------------------------------
bool AlreadyRun() {
	return FindWindow(TEXT("TApplication"), TEXT("Заставка")) != 0;
}

void ShowOptions(String sWnd) {
	HWND Handle;

	if (sWnd == NULL || sWnd.IsEmpty())
		Handle = 0;
	else {
		try {
			Handle = (HWND) StrToInt(sWnd);
		}
		catch (...) {
			Handle = 0;
		}
	}
	MessageBox(Handle, L"Эта заставка не имеет настраиваемых параметров",
		L"Программа-заставка", MB_OK | MB_ICONEXCLAMATION);
};

// ---------------------------------------------------------------------------
int WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int) {
	try {
		if (AlreadyRun())
			return 0;

		Application->Initialize();
		Application->MainFormOnTaskBar = false;
		Application->Title = "Заставка";

		if (ParamCount() > 0) {
			String S = UpperCase(ParamStr(1));

			if (S == "/S") {
				Application->CreateForm(__classid(TMain), &Main);
				Application->Run();
			}
			else if (S.SubString(1, 2) == "/C") {
				ShowOptions(S.SubString(4, MAXINT));
			}
		}
		else
			ShowOptions(NULL);

	}
	catch (Exception &exception) {
		Application->ShowException(&exception);
	}
	catch (...) {
		try {
			throw Exception("");
		}
		catch (Exception &exception) {
			Application->ShowException(&exception);
		}
	}
	return 0;
}
// ---------------------------------------------------------------------------
