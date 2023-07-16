#ifndef lTermArgsExam3Cxx
#define lTermArgsExam3Cxx
// headers
#include "lTermArgs.hxx"
// datadef
static constexpr auto vHelpText = R"([usage]=(
[flags]=()
[options]=()
[commands]=()
)=[usage])";
// actions
int main(int vArgC, char **vArgV)
{
	nTermArgs::tParser vParser;
	vParser.fSetHelpText(vHelpText);
	vParser.fParse(vArgC, vArgV);
	vParser.fPrint();
	return 0;
} // main
#endif // lTermArgsExam3Cxx
