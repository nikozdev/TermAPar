#ifndef lTermArgsExam2Cxx
#define lTermArgsExam2Cxx
// headers
#include "lTermArgs.hxx"
// actions
static void fCallback(const nTermArgs::tKey &rKey, nTermArgs::tParser &rParser)
{
	std::cout << "---------- boo! ----------" << std::endl;
	rParser.fPrint();
	std::cout << "--------------------------" << std::endl << std::endl;
}
int main(int vArgC, char **vArgV)
{
	nTermArgs::tParser vParser;
	auto			   pParserCmd = vParser.fRegCmd("boo");
	pParserCmd->fSetCallback(fCallback);
	vParser.fRegFlag("var v");
	vParser.fRegOpt("bar b");
	pParserCmd->fRegFlag("foo f");
	pParserCmd->fRegOpt("bar b foo f", "default");
	vParser.fParse(vArgC, vArgV);
	vParser.fPrint();
	return 0;
}
#endif // lTermArgsExam2Cxx
