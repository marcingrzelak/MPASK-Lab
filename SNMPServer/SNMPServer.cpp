#include "pch.h"
#include "FileHandler.h"
#include "TreeStructure.h"

int main()
{
	FileHandler file;

	string tekst;
	int line = 0;
	//regex wzorzec("\\w*\\s*OBJECT-TYPE\\s*SYNTAX.*ACCESS.*STATUS.*DESCRIPTION\\s*.*\\s*::=\\s*\{.*\}");

	tekst = file.FileRead("MIBS/RFC-1213-MIB.txt");

	cout << tekst;

	//while (getline(cin, tekst))
	//{
	//	smatch wynik; // tutaj będzie zapisany wynik
	//	++line;
	//	if (regex_search(tekst, wynik, wzorzec))
	//		cout << "Linia " << line << " : " << wynik[0] << '\n';

	//	cin >> tekst;
	//}
	//system("pause");
	//return 0;

	system("PAUSE");
	return(0);
}