#include "pch.h"
#include "FileHandler.h"
#include "TreeStructure.h"
#include "Regex.h"

int main()
{
	FileHandler mib("MIBS/RFC-1213-MIB.txt");
	Regex rgx;
	string rawFile, cleanedFile;

	rawFile = mib.FileRead();
	cleanedFile = regex_replace(rawFile, rgx.whitespaces(), " ");


	//\\w*\\s*OBJECT-TYPE\\s*SYNTAX.*ACCESS.*STATUS.*DESCRIPTION\\s*.*\\s*::=\\s*\{.*\}

	std::regex e("((\\w*)(\\s*)(OBJECT-TYPE)(\\s*)(SYNTAX)(.*)(ACCESS)(.*)(STATUS)(.*)(DESCRIPTION)(.*)(::=)(\\s*)(.)(\\s*)(\\w*)(\\s*)(\\d)(\\s*)(.))");

	std::sregex_iterator iter(cleanedFile.begin(), cleanedFile.end(), e);
	std::sregex_iterator end;

	while (iter != end)
	{
		std::cout << "expression match #" << 0 << ": " << (*iter)[0] << std::endl;
		for (unsigned i = 1; i < iter->size(); ++i)
		{
			std::cout << "capture submatch #" << i << ": " << (*iter)[i] << std::endl;
		}
		++iter;
	}













	//smatch wynik;

	/*std::regex e("\\w*\\s*OBJECT-TYPE");

	sregex_iterator iter(cleanedFile.begin(), cleanedFile.end(), e);
	std::sregex_iterator end;

	while (iter != end)
	{
		for (unsigned i = 0; i < iter->size(); ++i)
		{
			std::cout << "the " << i + 1 << "th match" << ": " << (*iter)[i] << std::endl;
		}
		++iter;
	}*/

	istringstream iss(cleanedFile);
	vector<string> results(istream_iterator<string>{iss}, istream_iterator<string>());

	system("PAUSE");
	return(0);
}