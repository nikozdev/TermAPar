#ifndef lTermArgsExam2Cxx
#define lTermArgsExam2Cxx
// headers
#include "lTermArgs.hxx"
// actions
static void fCallback(const nTermArgs::tKey &rKey, nTermArgs::tCmd &rCmd)
{
	rCmd.fPrint();
}
int main(int vArgC, char **vArgV)
{
	nTermArgs::tCmd	   vCmd;
	nTermArgs::tCmdPtr pCmd = vCmd.fSetCmd("boo");
	pCmd->fSetCallback(fCallback);
	pCmd->fSetOpt("foo f", "none");
	pCmd->fSetOpt("bar b foo f", "default");
	vCmd.fSetOpt("var v", "none");
	vCmd.fSetOpt("bar b", "default");
	vCmd.fParse(vArgC, vArgV);
	vCmd.fPrint();
	return 0;
}
#endif // lTermArgsExam2Cxx
