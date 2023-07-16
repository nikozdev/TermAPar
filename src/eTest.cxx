#ifndef lTermArgsTestCxx
#define lTermArgsTestCxx
// headers
#include <cassert>
#include "lTermArgs.hxx"
// defines
#define dPrintPrefix()                                         \
	({                                                         \
		std::printf("%s[%s]=(", &vOffsetStr[0], __FUNCTION__); \
		std::printf("\n");                                     \
		vOffsetStr[vOffsetNum++] = ' ';                        \
	}) // dPrintPrefix
#define dPrintSuffix()                                         \
	({                                                         \
		vOffsetStr[--vOffsetNum] = '\0';                       \
		std::printf("%s)=[%s]", &vOffsetStr[0], __FUNCTION__); \
		std::printf("\n");                                     \
	}) // dPrintSuffix
#define dPrintAssert(dExpr)                                        \
	({                                                             \
		std::printf("%s[%s]=(%d)", &vOffsetStr[0], #dExpr, dExpr); \
		std::printf("\n");                                         \
		assert(dExpr);                                             \
	}) // dPrintAssert
#define dPrintBlock(dBlock)                                             \
	({                                                                  \
		std::printf("%s[Block]=(%s)=[Block]", &vOffsetStr[0], #dBlock); \
		std::printf("\n");                                              \
		dBlock;                                                         \
	}) // dPrintBlock
// datadef
static char vOffsetStr[0xff];
static int	vOffsetNum = 0;
// actions
// // flag
void fTestFlagEmptyInput()
{
	dPrintPrefix();
	nTermArgs::tParser vParser;
	dPrintBlock({
		vParser.fRegFlag("foo f");
		vParser.fParse({});
	});
	dPrintAssert(vParser.fVetFlag("foo") == false);
	dPrintAssert(vParser.fGetOptCount("foo") == 0);
	dPrintSuffix();
} // fTestFlagEmptyInput
void fTestFlagNotFound()
{
	dPrintPrefix();
	nTermArgs::tParser vParser;
	dPrintBlock({
		vParser.fRegFlag("foo f");
		vParser.fParse({ "abc", "def" });
	});
	dPrintAssert(vParser.fVetFlag("foo") == false);
	dPrintAssert(vParser.fGetFlagCount("foo") == 0);
	dPrintSuffix();
} // fTestFlagNotFound
void fTestFlagL()
{
	dPrintPrefix();
	nTermArgs::tParser vParser;
	dPrintBlock({
		vParser.fRegFlag("foo f");
		vParser.fParse({ "--foo" });
	});
	dPrintAssert(vParser.fVetFlag("foo") == true);
	dPrintAssert(vParser.fGetFlagCount("foo") == 1);
	dPrintSuffix();
} // fTestFlagL
void fTestFlagS()
{
	dPrintPrefix();
	nTermArgs::tParser vParser;
	dPrintBlock({
		vParser.fRegFlag("foo f");
		vParser.fParse({ "-f" });
	});
	dPrintAssert(vParser.fVetFlag("foo") == true);
	dPrintAssert(vParser.fGetFlagCount("foo") == 1);
	dPrintSuffix();
} // fTestFlagS
void fTestFlagCondense()
{
	dPrintPrefix();
	nTermArgs::tParser vParser;
	dPrintBlock({
		vParser.fRegFlag("foo f");
		vParser.fParse({ "-fff" });
	});
	dPrintAssert(vParser.fVetFlag("foo") == true);
	dPrintAssert(vParser.fGetFlagCount("foo") == 3);
	dPrintSuffix();
} // fTestFlagCondense
void fTestFlagMultiple()
{
	dPrintPrefix();
	nTermArgs::tParser vParser;
	dPrintBlock({
		vParser.fRegFlag("foo f");
		vParser.fParse({ "-fff", "--foo", "-f" });
	});
	dPrintAssert(vParser.fVetFlag("foo") == true);
	dPrintAssert(vParser.fGetFlagCount("foo") == 5);
	dPrintSuffix();
} // fTestFlagMultiple
void fTestFlag()
{
	dPrintPrefix();
	fTestFlagEmptyInput();
	fTestFlagNotFound();
	fTestFlagL();
	fTestFlagS();
	fTestFlagCondense();
	fTestFlagMultiple();
	dPrintSuffix();
} // fTestFlag
// // Opt
void fTestOptNotFound()
{
	dPrintPrefix();
	nTermArgs::tParser vParser;
	dPrintBlock({
		vParser.fRegOpt("foo f", "default");
		vParser.fParse({ "abc", "def" });
	});
	dPrintAssert(vParser.fVetOpt("foo") == false);
	dPrintAssert(vParser.fGetOptCount("foo") == 0);
	dPrintAssert(vParser.fGetOpt("foo") == "default");
	dPrintSuffix();
} // fTestOptNotFound
void fTestOptL()
{
	dPrintPrefix();
	nTermArgs::tParser vParser;
	dPrintBlock({
		vParser.fRegOpt("foo f", "default");
		vParser.fParse({ "--foo", "bar" });
	});
	dPrintAssert(vParser.fVetOpt("foo") == true);
	dPrintAssert(vParser.fGetOptCount("foo") == 1);
	dPrintAssert(vParser.fGetOpt("foo") == "bar");
	dPrintSuffix();
} // fTestOptL
void fTestOptS()
{
	dPrintPrefix();
	nTermArgs::tParser vParser;
	dPrintBlock({
		vParser.fRegOpt("foo f", "default");
		vParser.fParse({ "-f", "bar" });
	});
	dPrintAssert(vParser.fVetOpt("foo") == true);
	dPrintAssert(vParser.fGetOptCount("foo") == 1);
	dPrintAssert(vParser.fGetOpt("foo") == "bar");
	dPrintSuffix();
} // fTestOptS
void fTestOptCondense()
{
	dPrintPrefix();
	nTermArgs::tParser vParser;
	dPrintBlock({
		vParser.fRegOpt("foo f", "default");
		vParser.fParse({ "-ff", "bar", "baz" });
	});
	dPrintAssert(vParser.fVetOpt("foo") == true);
	dPrintAssert(vParser.fGetOptCount("foo") == 2);
	dPrintAssert(vParser.fGetOpt("foo") == "baz");
	dPrintSuffix();
} // fTestOptCondense
void fTestOptMultiple()
{
	dPrintPrefix();
	nTermArgs::tParser vParser;
	dPrintBlock({
		vParser.fRegOpt("foo f", "default");
		vParser.fParse({ "-ff", "bar", "baz", "--foo", "bam" });
	});
	dPrintAssert(vParser.fVetOpt("foo") == true);
	dPrintAssert(vParser.fGetOptCount("foo") == 3);
	dPrintAssert(vParser.fGetOpt("foo") == "bam");
	dPrintSuffix();
} // fTestOptMultiple
void fTestOptParseSwitch()
{
	dPrintPrefix();
	nTermArgs::tParser vParser;
	dPrintBlock({ vParser.fParse({ "foo", "--", "--bar", "--baz" }); });
	dPrintAssert(vParser.fGetArgPak().size() == 3);
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
// // Arg
void fTestArgPak()
{
	dPrintPrefix();
	nTermArgs::tParser vParser;
	dPrintBlock({ vParser.fParse({ "abc", "def" }); });
	dPrintAssert(vParser.fGetArgPak().size() == 2);
	dPrintAssert(vParser.fGetArgPak()[0] == "abc");
	dPrintAssert(vParser.fGetArgPak()[1] == "def");
	dPrintSuffix();
} // fTestArgPak
void fTestArg()
{
	dPrintPrefix();
	fTestArgPak();
	dPrintSuffix();
} // fTestArg
// // Cmd
void fTestCmd()
{
	dPrintPrefix();
	nTermArgs::tParser vParser;
	auto			   pParserCmd = vParser.fRegCmd("boo");
	dPrintBlock({
		pParserCmd->fRegFlag("foo");
		pParserCmd->fRegOpt("bar", "default");
		vParser.fParse({ "boo", "abc", "def", "--foo", "--bar", "baz" });
	});
	dPrintAssert(vParser.fVetCmd("boo"));
	dPrintAssert(pParserCmd->fGetArgPak().size() == 2);
	dPrintAssert(pParserCmd->fVetFlag("foo"));
	dPrintAssert(pParserCmd->fGetOpt("bar") == "baz");
	dPrintSuffix();
} // fTestCmd
// // main
int fTest()
{
	dPrintPrefix();
	fTestFlag();
	fTestOpt();
	fTestArg();
	fTestCmd();
	dPrintSuffix();
	return 0;
} // fTest
int main()
{
	return fTest();
} // main
#endif // lTermArgsTestCxx
