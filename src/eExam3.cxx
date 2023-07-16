#ifndef lTermArgsExam3Cxx
#define lTermArgsExam3Cxx
// headers
#include "lTermArgs.hxx"
// datadef
// actions
int main(int vArgC, char **vArgV)
{
	nTermArgs::tParser vParser;
	vParser.fRegOpt("config-file config-path config-file-path");
	vParser.fParse(vArgC, vArgV);
	vParser.fPrint();
	return 0;
} // main
#endif // lTermArgsExam3Cxx
