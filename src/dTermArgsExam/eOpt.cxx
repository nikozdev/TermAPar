#ifndef lTermArgsExamOptCxx
#define lTermArgsExamOptCxx
// headers
#include <lTermArgs.hxx>
// datadef
static constexpr auto vHelpText = dTermArgsProjName R"(
- i am an argument and option parser
> reading arguments
> reading options
> missing commands
- i have some rules
> only arguments after --
> > options can be used before it and after it's paired with another --
- you can call me like this ...
> {0} [arg1 ... argn] [(opt|o)(=| )<val>]
> {0}  [(opt|o)(=| )<val>] [arg1 ... argn]
> {0} [(opt|o)(=| )<val>] -- [arg1 ... argn]
> {0} [arg1 ... argn] [(opt|o)(=| )<val>] -- [arg1 ... argn]
> {0} [(opt|o)(=| )<val>] [arg1 ... argn] -- [arg1 ... argn]
> {0} -- [arg1 ... argn] -- [(opt|o)(=| )<val>]
> whatever else you can imagine
)"; // HelpText
// actions
int main(int vArgC, char **vArgV)
{
	nTermArgs::tArgParser vArgParser;
	vArgParser.fSetHelpText(vHelpText);
	vArgParser.fSetOpt("opt o", "val");
	vArgParser.fParse(vArgC, vArgV);
	fmt::println("[0]={1}=[0]", "ArgParser", vArgParser);
	return 0;
} // main
#endif // lTermArgsExamOptCxx
