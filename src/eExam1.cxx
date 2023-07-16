#ifndef lTermArgsExam1Cxx
#define lTermArgsExam1Cxx
// headers
#include "lTermArgs.hxx"
// actions
int main(int vArgC, char **vArgV)
{
	nTermArgs::tParser vParser;

	vParser.fRegFlag("foo f");
	vParser.fRegOpt("bar b", "default");

	vParser.fParse(vArgC, vArgV);
	vParser.fPrint();

	return 0;
} // main
#endif // lTermArgsExam1Cxx
