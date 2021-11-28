#include <locale.h>
#include "UIWindow.h"

/*
	Auteur : Newen
	Derni�re Modification : Octobre 2021.

	Point d'entr�e de l'application Windows Diogenes.
	*Diogenes est un philosophe grec de l'antiquit�.
	
	UIWindow est en principe instanci� une seule fois gr�ce � la classe Singleton.
	La classe DX9 est instanci�e de la m�me mani�re.

	Elle sont instanci�es de cette fa�on pour �viter les conflits de gestion des ressources.
*/

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	// D�finit les param�tres r�gionaux et donc la page de codes ANSI par d�faut de l�utilisateur pour ce pays/cette r�gion, telle qu�elle est obtenue � partir du syst�me d�exploitation h�te.
	_wsetlocale(LC_ALL, L"fr-FR");

	// Lance l'application uniquement si l'initialisation a r�ussie.
	if (UIWINDOW.Init(1, 1, 1, 1)) UIWINDOW.Run();

	return 0;
}
