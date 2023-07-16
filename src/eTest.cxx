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
	dPrintAssert(vParser.fVetArg("foo") == false);
	dPrintAssert(vParser.fGetArgCount("foo") == 0);
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
	dPrintAssert(vParser.fVetArg("foo") == false);
	dPrintAssert(vParser.fGetArgCount("foo") == 0);
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
	dPrintAssert(vParser.fVetArg("foo") == true);
	dPrintAssert(vParser.fGetArgCount("foo") == 1);
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
	dPrintAssert(vParser.fVetArg("foo") == true);
	dPrintAssert(vParser.fGetArgCount("foo") == 1);
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
	dPrintAssert(vParser.fVetArg("foo") == true);
	dPrintAssert(vParser.fGetArgCount("foo") == 3);
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
	dPrintAssert(vParser.fVetArg("foo") == true);
	dPrintAssert(vParser.fGetArgCount("foo") == 5);
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
	dPrintAssert(vParser.fVetArg("foo") == false);
	dPrintAssert(vParser.fGetArgCount("foo") == 0);
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
	dPrintAssert(vParser.fVetArg("foo") == true);
	dPrintAssert(vParser.fGetArgCount("foo") == 1);
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
	dPrintAssert(vParser.fVetArg("foo") == true);
	dPrintAssert(vParser.fGetArgCount("foo") == 1);
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
	dPrintAssert(vParser.fVetArg("foo") == true);
	dPrintAssert(vParser.fGetArgCount("foo") == 2);
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
	dPrintAssert(vParser.fVetArg("foo") == true);
	dPrintAssert(vParser.fGetArgCount("foo") == 3);
	dPrintAssert(vParser.fGetOpt("foo") == "bam");
	dPrintSuffix();
} // fTestOptMultiple
void fTestOptParseSwitch()
{
	dPrintPrefix();
	nTermArgs::tParser vParser;
	dPrintBlock({ vParser.fParse({ "foo", "--", "--bar", "--baz" }); });
	dPrintAssert(vParser.vArgPak.size() == 3);
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
	dPrintAssert(vParser.vArgPak.size() == 2);
	dPrintAssert(vParser.vArgPak[0] == "abc");
	dPrintAssert(vParser.vArgPak[1] == "def");
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
	nTermArgs::tParser	vParser;
	nTermArgs::tParser &rParserCmd = vParser.fRegCmd("boo");
	dPrintBlock({
		rParserCmd.fRegFlag("foo");
		rParserCmd.fRegOpt("bar", "default");
		vParser.fParse({ "boo", "abc", "def", "--foo", "--bar", "baz" });
	});
	dPrintAssert(vParser.fVetCmd());
	dPrintAssert(vParser.fGetCmdKey() == "boo");
	dPrintAssert(rParserCmd.vArgPak.size() == 2);
	dPrintAssert(rParserCmd.fVetArg("foo"));
	dPrintAssert(rParserCmd.fGetOpt("bar") == "baz");
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
