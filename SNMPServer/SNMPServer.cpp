#include "pch.h"
#include "FileHandler.h"
#include "TreeStructure.h"

int main()
{
	FileHandler file("MIBS/RFC-1213-MIB.txt");
	Tree drzewo;

	string test = "", test2 = "";

	test = file.FileGetLine();
	test2 = file.FileGetLine();

	drzewo.addRoot("korzen", 1);
	drzewo.addNode("lisc1", 2, drzewo.root);

	test = file.FileGetLine();
	test2 = file.FileGetLine();

	system("PAUSE");
	return(0);
}