////////////////////////////////////////////////////////////////////////////////
//
// Przyklad jak odczytac dane zarejestrowanego uzytkownika z klucza licencyjnego
//
// Wersja         : PELock v2.0
// Jezyk          : C/C++
// Autor          : Bartosz W�jcik (support@pelock.com)
// Strona domowa  : https://www.pelock.com
//
////////////////////////////////////////////////////////////////////////////////

#include <windows.h>
#include <commctrl.h>
#include <stdio.h>
#include <math.h>
#include "gui.h"
#include "pelock.h"

// bufor, gdzie zostanie odczytana nazwa zarejestrowanego uzytkownika
unsigned char szUser[256] = { 0 };
int i = 0;
char *szToken = NULL;

BOOL CALLBACK DlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		// inicjalizacja glownego okna
		case WM_INITDIALOG:

			// kod pomiedzy makrami DEMO_START i DEMO_END bedzie zaszyfrowany
			// w zabezpieczonym pliku i nie bedzie dostepny (wykonany) bez
			// poprawnego klucza licencyjnego
			DEMO_START

			// odczytaj dane zarejestrowanego uzytkownika
			GetRegistrationName(szUser, sizeof(szUser));

			// koncowy marker
			DEMO_END

			// sprawdz dlugosc odczytanych danych rejestracyjnych
			// uzytkownika, jesli bedzie = 0, oznaczac to bedzie
			// brak klucza licencyjnego (lub klucz niepoprawny)
			if (strlen(szUser) != 0)
			{
				// podziel dane rejestracyjne na linie (puste linie sa ignorowane)
				szToken = strtok(szUser, "\r\n");

				i = 1;

				// wyswietl wszystkie linijki tekstu zapisane w kluczu (puste linie nie sa wyswietlane)
				while (szToken != NULL)
				{
					// wyswietl 3 linijke z nazwy zarejestrowanego uzytkownika
					if (i == 3)
					{
						// wyswietl dane zarejestrowanego uzytkownika
						SetDlgItemText(hDlg, IDC_REG, szToken);

						break;
					}

					szToken = strtok(NULL, "\r\n");
				}
			}
			else
			{
				SetDlgItemText(hDlg, IDC_REG, "Niezarejestrowany!");
			}

			// ustaw tytul glownego okna
			SetWindowText(hDlg, "PELock Test");

			return TRUE;

		case WM_COMMAND:

			switch (LOWORD (wParam))
			{
				case IDCANCEL:
					EndDialog(hDlg, 0);
					break;
			}

			break;
	}

	return FALSE;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	INITCOMMONCONTROLSEX iccControls = { sizeof(INITCOMMONCONTROLSEX), ICC_WIN95_CLASSES | ICC_DATE_CLASSES };

	// inicjalizuj kontrolki
	InitCommonControlsEx(&iccControls);

	DialogBox(hInstance, MAKEINTRESOURCE(DLG_MAIN), 0, DlgProc);

	return 0;
}
