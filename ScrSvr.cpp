// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <tchar.h>

#include <windows.h>

#include "UtilsFiles.h"
#include "UtilsFileIni.h"

// ---------------------------------------------------------------------------
USEFORM("ScrSvrMain.cpp", Main);
USEFORM("ScrSvrScreen.cpp", frmScreen);
//---------------------------------------------------------------------------
bool AlreadyRun() {
	return FindWindow(TEXT("TApplication"), TEXT("Заставка")) != 0;
}

void ShowOptions(String sWnd) {
	// HWND Handle;
	//
	// if (sWnd == NULL || sWnd.IsEmpty()) {
	// Handle = 0;
	// }
	// else {
	// try {
	// Handle = (HWND) StrToInt(sWnd);
	// }
	// catch (...) {
	// Handle = 0;
	// }
	// }

	String FileNameIni = IncludeTrailingPathDelimiter
		(IncludeTrailingPathDelimiter(GetSpecialFolderPath(CSIDL_LOCAL_APPDATA))
		+ "P3tr0viCh");

	if (!DirectoryExists(FileNameIni)) {
		CreateDir(FileNameIni);
	}

	FileNameIni = FileNameIni + ChangeFileExt
		(ExtractFileName(Application->ExeName), ".ini");

	if (!FileExists(FileNameIni)) {
		TFileIni * FileIni = TFileIni::GetNewInstance(FileNameIni);

		try {
			FileIni->WriteInteger("Common", "BackColor", clBlack);

			FileIni->WriteInteger("Date", "FontColor", 0x0001B9FE);
			FileIni->WriteString("Date", "FontName", "Arial");
			FileIni->WriteInteger("Date", "FontSize", 36);

			FileIni->WriteInteger("Time", "FontColor", clYellow);
			FileIni->WriteString("Time", "FontName", "Arial");
			FileIni->WriteInteger("Time", "FontSize", 100);
		}
		__finally {
			delete FileIni;
		}
	}

	ShellExec(FileNameIni);
};

// ---------------------------------------------------------------------------
int WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int) {
	try {
		if (AlreadyRun()) {
			return 0;
		}

		Application->Initialize();
		Application->ShowMainForm = false;
		Application->MainFormOnTaskBar = false;
		Application->Title = "Заставка";

		if (ParamCount() > 0) {
			String S = UpperCase(ParamStr(1));

			if (S == "/S") {
				Application->CreateForm(__classid(TMain), &Main);
		Application->Run();
			}
			else {
				if (S.SubString(1, 2) == "/C") {
					ShowOptions(S.SubString(4, MAXINT));
				}
			}
		}
		else {
			ShowOptions(NULL);
		}

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
