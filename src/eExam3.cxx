#ifndef lTermArgsExam3Cxx
#define lTermArgsExam3Cxx
// headers
#include "lTermArgs.hxx"
// datadef
// actions
int main(int vArgC, char **vArgV)
{
	nTermArgs::tCmd vCmd;
	vCmd.fSetOpt("config-file config-path config-file-path");
	vCmd.fParse(vArgC, vArgV);
	vCmd.fPrint();
	return 0;
} // main
#endif // lTermArgsExam3Cxx
