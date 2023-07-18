#ifndef lTermArgsExamArgCxx
#define lTermArgsExamArgCxx
// headers
#include <lTermArgs.hxx>
// datadef
static constexpr auto vHelpText = dTermArgsProjName R"(
- i am an argument parser
> reading arguments
> missing options
> missing commands
- you can call me like this ...
> {0} [arg1 ... argn]
> {0} -- [arg1 ... argn]
> {0} [arg1 ... argn] -- [arg1 ... argn]
)"; // HelpText
// actions
int main(int vArgC, char **vArgV)
{
	nTermArgs::tArgParser vArgParser;
	vArgParser.fSetHelpText(vHelpText);
	vArgParser.fParse(vArgC, vArgV);
	vArgParser.fPrint();
	return 0;
} // main
#endif // lTermArgsExamArgCxx
