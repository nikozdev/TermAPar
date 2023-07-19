#ifndef lTermArgsExamCmdCxx
#define lTermArgsExamCmdCxx
// headers
#include <lTermArgs.hxx>
// datadef
static constexpr auto vHelpText	   = dTermArgsProjName R"(
- i am a command parser
> missing options
> reading commands
- you can call me like this ...
> {0} [(cmd|c)] <cmd-things>
)";
static constexpr auto vHelpTextCmd = dTermArgsProjName R"(
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
)"; // HelpTextCmd
// actions
static void fCallback(nTermArgs::tCmd &rCmd)
{
	nTermArgs::nFormat::println("[0]={1}=[0]", "Cmd", rCmd);
}
int main(int vArgC, char **vArgV)
{
	nTermArgs::tArgParser vArgParser;
	vArgParser.fSetHelpText(vHelpText);
	nTermArgs::tCmdPtr pArgParserCmd = vArgParser.fSetCmd("c cmd");
	pArgParserCmd->fSetHelpText(vHelpTextCmd);
	pArgParserCmd->fSetCallback(fCallback);
	pArgParserCmd->fSetOpt("o opt", "val");
	vArgParser.fParse(vArgC, vArgV);
	nTermArgs::nFormat::println("[0]={1}=[0]", "ArgParser", vArgParser);
	return 0;
}
#endif // lTermArgsExamCmdCxx
