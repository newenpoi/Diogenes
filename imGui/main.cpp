#include <locale.h>
#include "UIWindow.h"

/*
	Auteur : Newen
	Derni�re Modification : Octobre 2021.

	Point d'entr�e de l'application Windows Diogenes.
*/

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	_wsetlocale(LC_ALL, L"fr-FR");

	// Lance l'application uniquement si l'initialisation a r�ussie.
	if (UIWINDOW.Init(1, 1, 1, 1)) UIWINDOW.Run();

	return 0;
}
