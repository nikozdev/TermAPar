#ifndef lTermArgsExam1Cxx
#define lTermArgsExam1Cxx
// headers
#include "lTermArgs.hxx"
// actions
int main(int vArgC, char **vArgV)
{
	nTermArgs::tCmd vCmd;
	vCmd.fSetOpt("foo f", "none");
	vCmd.fSetOpt("bar b", "default");
	vCmd.fParse(vArgC, vArgV);
	vCmd.fPrint();
	return 0;
} // main
#endif // lTermArgsExam1Cxx
