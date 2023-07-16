#include <iostream>
#include "lTermArgs.hxx"

static void fCallback(const nTermArgs::tKey &rKey, nTermArgs::tParser &rParser)
{
	std::cout << "---------- boo! ----------" << std::endl;
	rParser.fPrint();
	std::cout << "--------------------------" << std::endl << std::endl;
}

int main(int vArgC, char **vArgV)
{
	nTermArgs::tParser	vParser{ "Usage: example...", "1.0" };
	nTermArgs::tParser &rParserCmd
		= vParser.fRegCmd("boo", "Usage: example boo...", fCallback);
	rParserCmd.fRegFlag("foo f");
	rParserCmd.fRegOpt("bar b foo f", "default");
	vParser.fParse(vArgC, vArgV);
	vParser.fPrint();
	return 0;
}
