#ifndef lTermAParExamArgCxx
#define lTermAParExamArgCxx
// headers
#include <dTermAPar.dir/fMain.hxx>
// datadef
static constexpr auto vHelpText = dTermAParProjName R"(
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
	nTermAPar::tArgParser vArgParser;
	vArgParser.fSetHelpText(vHelpText);
	vArgParser.fParse(vArgC, vArgV);
	nTermAPar::nFormat::println("[0]={1}=[0]", "ArgParser", vArgParser);
	return 0;
} // main
#endif // lTermAParExamArgCxx
