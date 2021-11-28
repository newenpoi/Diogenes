#include <locale.h>
#include "UIWindow.h"

/*
	Auteur : Newen
	Dernière Modification : Octobre 2021.

	Point d'entrée de l'application Windows Diogenes.
	*Diogenes est un philosophe grec de l'antiquité.
	
	UIWindow est en principe instancié une seule fois grâce à la classe Singleton.
	La classe DX9 est instanciée de la même manière.

	Elle sont instanciées de cette façon pour éviter les conflits de gestion des ressources.
*/

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	// Définit les paramètres régionaux et donc la page de codes ANSI par défaut de l’utilisateur pour ce pays/cette région, telle qu’elle est obtenue à partir du système d’exploitation hôte.
	_wsetlocale(LC_ALL, L"fr-FR");

	// Lance l'application uniquement si l'initialisation a réussie.
	if (UIWINDOW.Init(1, 1, 1, 1)) UIWINDOW.Run();

	return 0;
}
