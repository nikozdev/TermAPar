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
	nTermArgs::tParser	vParser;
	nTermArgs::tParser &rParserCmd = vParser.fRegCmd("boo");
	rParserCmd.fRegFlag("foo f");
	rParserCmd.fRegOpt("bar b foo f", "default");
	vParser.fParse(vArgC, vArgV);
	vParser.fPrint();
	return 0;
}
#endif // lTermArgsExam2Cxx
