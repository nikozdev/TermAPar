#ifndef dTermAParMainCxx
#define dTermAParMainCxx
// headers
#include <cassert>
#include <dTermAPar.dir/fHead.hxx>
// defines
using namespace nTermAPar;
using namespace nFormat;
#define dPrintPrefix()                                       \
	({                                                       \
		println("{0}[{1}]=(", &vOffsetStr[0], __FUNCTION__); \
		vOffsetStr[vOffsetNum++] = ' ';                      \
	}) // dPrintPrefix
#define dPrintSuffix()                                       \
	({                                                       \
		vOffsetStr[--vOffsetNum] = '\0';                     \
		println("{0})=[{1}]", &vOffsetStr[0], __FUNCTION__); \
	}) // dPrintSuffix
#define dPrintAssert(dExpr)                                       \
	({                                                            \
		println("{0}[{1}]=({2})", &vOffsetStr[0], #dExpr, dExpr); \
		assert(dExpr);                                            \
	}) // dPrintAssert
#define dPrintBlock(dBlock)                                           \
	({                                                                \
		println("{0}[Block]=({1})=[Block]", &vOffsetStr[0], #dBlock); \
		dBlock;                                                       \
	}) // dPrintBlock
// content
#ifdef dTermAParMakeProjExe
// datadef
static char vOffsetStr[0xff];
static int	vOffsetNum = 0;
#	 if 1
// // Arg
void fTestArgArr()
{
	dPrintPrefix();
	tArgParser vArgParser;
	dPrintBlock({ vArgParser.fParse({"abc", "def"}); });
	dPrintAssert(vArgParser.fVetArgArr(2));
	dPrintAssert(vArgParser.fVetArgVal("abc"));
	dPrintAssert(vArgParser.fVetArgVal("def"));
	dPrintSuffix();
} // fTestArgArr
void fTestArg()
{
	dPrintPrefix();
	fTestArgArr();
	dPrintSuffix();
} // fTestArg
// // Opt
void fTestOptNotFound()
{
	dPrintPrefix();
	tArgParser vArgParser;
	dPrintBlock({
		vArgParser.fSetOpt("foo f", "val");
		vArgParser.fParse({"abc", "def"});
	});
	dPrintAssert(vArgParser.fVetOptKey("foo", 1));
	dPrintAssert(vArgParser.fVetOptVal("foo", "val"));
	dPrintSuffix();
} // fTestOptNotFound
void fTestOptL()
{
	dPrintPrefix();
	tArgParser vArgParser;
	dPrintBlock({
		vArgParser.fSetOpt("foo f", "val");
		vArgParser.fParse({"--foo", "bar"});
	});
	dPrintAssert(vArgParser.fVetOptKey("foo", 1));
	dPrintAssert(vArgParser.fVetOptVal("foo", "bar"));
	dPrintSuffix();
} // fTestOptL
void fTestOptS()
{
	dPrintPrefix();
	tArgParser vArgParser;
	dPrintBlock({
		vArgParser.fSetOpt("foo f", "val");
		vArgParser.fParse({"-f", "bar"});
	});
	dPrintAssert(vArgParser.fVetOptKey("f", 1));
	dPrintAssert(vArgParser.fVetOptVal("f", "bar"));
	dPrintSuffix();
} // fTestOptS
void fTestOptCondense()
{
	dPrintPrefix();
	tArgParser vArgParser;
	dPrintBlock({
		vArgParser.fSetOpt("foo f", "val");
		vArgParser.fParse({"-ff", "bar", "baz"});
	});
	dPrintAssert(vArgParser.fVetOptKey("f", 1));
	dPrintAssert(vArgParser.fVetOptVal("f", "baz"));
	dPrintSuffix();
} // fTestOptCondense
void fTestOptMultiple()
{
	dPrintPrefix();
	tArgParser vArgParser;
	dPrintBlock({
		vArgParser.fSetOpt("foo f", "val");
		vArgParser.fParse({"-ff", "bar", "baz", "--foo", "bam"});
	});
	dPrintAssert(vArgParser.fVetOptKey("foo", 1));
	dPrintAssert(vArgParser.fVetOptVal("foo", "bam"));
	dPrintSuffix();
} // fTestOptMultiple
void fTestOptParseSwitch()
{
	dPrintPrefix();
	tArgParser vArgParser;
	dPrintBlock({ vArgParser.fParse({"foo", "--", "--bar", "--baz"}); });
	dPrintAssert(vArgParser.fVetArgArr(3));
	dPrintSuffix();
} // fTestOptParseSwitch
void fTestOpt()
{
	dPrintPrefix();
	fTestOptNotFound();
	fTestOptL();
	fTestOptS();
	fTestOptCondense();
	fTestOptMultiple();
	fTestOptParseSwitch();
	dPrintSuffix();
} // fTestOpt
// // Cmd
void fTestCmd()
{
	dPrintPrefix();
	tArgParser vArgParser;
	auto	   pArgParserCmd = vArgParser.fSetCmd("boo");
	dPrintBlock({
		pArgParserCmd->fSetOpt("foo", "val");
		pArgParserCmd->fSetOpt("bar", "val");
		vArgParser.fParse({"boo", "abc", "def", "--foo", "for", "--bar", "baz"}
		);
	});
	dPrintAssert(vArgParser.fVetCmdKey("boo", 1));
	dPrintAssert(pArgParserCmd->fVetArgArr(2));
	dPrintAssert(pArgParserCmd->fVetOptKey("foo", 1));
	dPrintAssert(pArgParserCmd->fVetOptVal("bar", "baz"));
	dPrintSuffix();
} // fTestCmd
// // main
bool fTestUnit(tCmd &rCmd)
{
	dPrintPrefix();
	fTestArg();
	fTestOpt();
	fTestCmd();
	dPrintSuffix();
	return 1;
} // fTestUnit
#	endif // dTermAParTestProjExeTestUnit
#	if 1
// datadef
static constexpr auto cTestTimeCount = 1'000;
// actions
// // main
bool fTestTime(tCmd &rCmd)
{
	for(auto vI = 0; vI < cTestTimeCount; vI++)
	{
		for(auto vI = 0; vI < cTestTimeCount; vI++)
		{
			tArgParser vArgParser;
			vArgParser.fSetOpt("o opt option", "v");
			vArgParser.fSetCmd("c cmd comman");
			vArgParser.fParse({
				"--",
				"a1",
				"a2",
				"--",
				"-o=value",
				"--",
				"a3",
				"a4",
				"--",
				"--opt=val",
			});
		}
	}
	return 1;
} // fTestTime
#	endif // dTermAParTestProjExeTestTime
#	if 1
// datadef
static auto cExamArgHelpText = format(
	R"({0}
- i am an argument parser
> reading arguments
> missing options
> missing commands
- you can call me like this ...
> {1} [arg1 ... argn]
> {1} -- [arg1 ... argn]
> {1} [arg1 ... argn] -- [arg1 ... argn]
)",
	dTermAParProjName,
	dTermAParProjNameExe
); // cExamArgHelpText
// actions
#	endif // dTermAParTestProjExeExamArg
#	if 1
// datadef
static auto cExamOptHelpText = format(
	R"({0}
- i am an argument and option parser
> reading arguments
> reading options
> missing commands
- i have some rules
> only arguments after --
> > options can be used before it and after it's paired with another --
- you can call me like this ...
> {1} [arg1 ... argn] [(opt)|o(=| )<val>]
> {1} [(opt)|o(=| )<val>] [arg1 ... argn]
> {1} [(opt)|o(=| )<val>] -- [arg1 ... argn]
> {1} [arg1 ... argn] [(opt)|o(=| )<val>] -- [arg1 ... argn]
> {1} [(opt)|o(=| )<val>] [arg1 ... argn] -- [arg1 ... argn]
> {1} -- [arg1 ... argn] -- [(opt)|o(=| )<val>]
> whatever else you can imagine
)",
	dTermAParProjName,
	dTermAParProjNameExe
); // cExamOptHelpText
// actions
#	endif // dTermAParTestProjExeExamOpt
#	if 1
// datadef
static auto cExamCmdHelpText = format(
	R"({0}
- i am a command parser
> missing options
> reading commands
- you can call me like this ...
> {1} [(cmd)|c] <cmd-args>
)",
	dTermAParProjName,
	dTermAParProjNameExe
); // cExamCmdHelpText
static auto cExamCmdHelpTextCmd = format(
	R"({0}
- i am an argument and option parser
> reading arguments
> reading options
> missing commands
- i have some rules
> only arguments after --
> > options can be used before it and after it's paired with another --
- you can call me like this ...
> {1} [arg1 ... argn] [(opt)|o(=| )<val>]
> {1} [(opt)|o(=| )<val>] [arg1 ... argn]
> {1} [(opt)|o(=| )<val>] -- [arg1 ... argn]
> {1} [arg1 ... argn] [(opt)|o(=| )<val>] -- [arg1 ... argn]
> {1} [(opt)|o(=| )<val>] [arg1 ... argn] -- [arg1 ... argn]
> {1} -- [arg1 ... argn] -- [(opt)|o(=| )<val>]
> whatever else you can imagine
)",
	dTermAParProjName,
	dTermAParProjNameExe
); // cExamCmdHelpTextCmd
// actions
#	endif // dTermAParTestProjExeExamCmd
// datadef
static auto cHelpText = format(
	R"({0}
- i am a command parser
> missing options
> reading commands
- you can call me like this ...
> {1} [(TestUnit)(TestTime)(ExamArg)|(ExamOpt)|(ExamCmd)] <cmd-args>
)",
	dTermAParProjName,
	dTermAParProjNameExe
); // cHelpText
// actions
static bool fCmdFun(tCmd &rCmd)
{
	println("[{0}]={1}=[{0}]", (size_t)&rCmd, rCmd);
	return 1;
}
int main(int vArgC, char *vArgV[])
{
	tArgParser vArgParser;
	vArgParser.fSetHelpText(cHelpText);
	vArgParser.fSetCmdAct(
		[](tCmd &rCmd)
		{
		if(rCmd.fVetCmdKey())
		{
			return 1;
		}
		else
		{
			rCmd.fPrintHelp();
			return 0;
		}
	});
	vArgParser.fSetCmd("TestUnit", fTestUnit);
	vArgParser.fSetCmd("TestTime", fTestTime);
	auto pExamArgParser = vArgParser.fSetCmd("ExamArg", fCmdFun);
	pExamArgParser->fSetHelpText(cExamArgHelpText);
	auto pExamOptParser = vArgParser.fSetCmd("ExamOpt", fCmdFun);
	pExamOptParser->fSetHelpText(cExamOptHelpText);
	pExamOptParser->fSetOpt("opt o", "val");
	auto pExamCmdParser = vArgParser.fSetCmd("ExamCmd", fCmdFun);
	pExamCmdParser->fSetHelpText(cExamCmdHelpText);
	auto pExamCmdParserCmd = pExamCmdParser->fSetCmd("c cmd", fCmdFun);
	pExamCmdParserCmd->fSetHelpText(cExamCmdHelpTextCmd);
	pExamCmdParserCmd->fSetOpt("o opt", "val");
	vArgParser.fParse(vArgC, vArgV);
} // main
#endif // dTermAParMakeProjExe
#endif // dTermAParMainCxx
