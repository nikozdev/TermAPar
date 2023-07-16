#ifndef lTermArgsTestCxx
#define lTermArgsTestCxx
// headers
#include <cassert>
#include <vector>
#include <string>
#include "lTermArgs.hxx"
// defines
#define dPrintPrefix()                                      \
	({                                                      \
		printf("%s[%s]=(\n", &vOffsetStr[0], __FUNCTION__); \
		vOffsetStr[vOffsetNum++] = ' ';                     \
	}) // dPrintPrefix
#define dPrintSuffix()                                      \
	({                                                      \
		vOffsetStr[--vOffsetNum] = '\0';                    \
		printf("%s)=[%s]\n", &vOffsetStr[0], __FUNCTION__); \
	}) // dPrintSuffix
// datadef
static char vOffsetStr[0xff];
static int	vOffsetNum = 0;
// actions
// // flag
void fTestFlagEmptyInput()
{
	dPrintPrefix();
	nTermArgs::tParser vParser;
	vParser.fRegFlag("foo f");
	vParser.fParse({});
	assert(vParser.fVetArg("foo") == false);
	assert(vParser.fGetArgCount("foo") == 0);
	dPrintSuffix();
} // fTestFlagEmptyInput
void fTestFlagNotFound()
{
	dPrintPrefix();
	nTermArgs::tParser vParser;
	vParser.fRegFlag("foo f");
	vParser.fParse({ "abc", "def" });
	assert(vParser.fVetArg("foo") == false);
	assert(vParser.fGetArgCount("foo") == 0);
	dPrintSuffix();
} // fTestFlagNotFound
void fTestFlagL()
{
	dPrintPrefix();
	nTermArgs::tParser vParser;
	vParser.fRegFlag("foo f");
	vParser.fParse({ "--foo" });
	assert(vParser.fVetArg("foo") == true);
	assert(vParser.fGetArgCount("foo") == 1);
	dPrintSuffix();
} // fTestFlagL
void fTestFlagS()
{
	dPrintPrefix();
	nTermArgs::tParser vParser;
	vParser.fRegFlag("foo f");
	vParser.fParse({ "-f" });
	assert(vParser.fVetArg("foo") == true);
	assert(vParser.fGetArgCount("foo") == 1);
	dPrintSuffix();
} // fTestFlagS
void fTestFlagCondense()
{
	dPrintPrefix();
	nTermArgs::tParser vParser;
	vParser.fRegFlag("foo f");
	vParser.fParse({ "-fff" });
	assert(vParser.fVetArg("foo") == true);
	assert(vParser.fGetArgCount("foo") == 3);
	dPrintSuffix();
} // fTestFlagCondense
void fTestFlagMultiple()
{
	dPrintPrefix();
	nTermArgs::tParser vParser;
	vParser.fRegFlag("foo f");
	vParser.fParse({ "-fff", "--foo", "-f" });
	assert(vParser.fVetArg("foo") == true);
	assert(vParser.fGetArgCount("foo") == 5);
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
	vParser.fRegOpt("foo f", "default");
	vParser.fParse({ "abc", "def" });
	assert(vParser.fVetArg("foo") == false);
	assert(vParser.fGetArgCount("foo") == 0);
	assert(vParser.fGetOpt("foo") == "default");
	dPrintSuffix();
} // fTestOptNotFound
void fTestOptL()
{
	dPrintPrefix();
	nTermArgs::tParser vParser;
	vParser.fRegOpt("foo f", "default");
	vParser.fParse({ "--foo", "bar" });
	assert(vParser.fVetArg("foo") == true);
	assert(vParser.fGetArgCount("foo") == 1);
	assert(vParser.fGetOpt("foo") == "bar");
	dPrintSuffix();
} // fTestOptL
void fTestOptS()
{
	dPrintPrefix();
	nTermArgs::tParser vParser;
	vParser.fRegOpt("foo f", "default");
	vParser.fParse({ "-f", "bar" });
	assert(vParser.fVetArg("foo") == true);
	assert(vParser.fGetArgCount("foo") == 1);
	assert(vParser.fGetOpt("foo") == "bar");
	dPrintSuffix();
} // fTestOptS
void fTestOptCondense()
{
	dPrintPrefix();
	nTermArgs::tParser vParser;
	vParser.fRegOpt("foo f", "default");
	vParser.fParse({ "-ff", "bar", "baz" });
	assert(vParser.fVetArg("foo") == true);
	assert(vParser.fGetArgCount("foo") == 2);
	assert(vParser.fGetOpt("foo") == "baz");
	dPrintSuffix();
} // fTestOptCondense
void fTestOptMultiple()
{
	dPrintPrefix();
	nTermArgs::tParser vParser;
	vParser.fRegOpt("foo f", "default");
	vParser.fParse({ "-ff", "bar", "baz", "--foo", "bam" });
	assert(vParser.fVetArg("foo") == true);
	assert(vParser.fGetArgCount("foo") == 3);
	assert(vParser.fGetOpt("foo") == "bam");
	dPrintSuffix();
} // fTestOptMultiple
void fTestOptParseSwitch()
{
	dPrintPrefix();
	nTermArgs::tParser vParser;
	vParser.fParse({ "foo", "--", "--bar", "--baz" });
	assert(vParser.vArgPak.size() == 3);
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
	vParser.fParse({ "abc", "def" });
	assert(vParser.vArgPak.size() == 2);
	assert(vParser.vArgPak[0] == "abc");
	assert(vParser.vArgPak[1] == "def");
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
	rParserCmd.fRegFlag("foo");
	rParserCmd.fRegOpt("bar", "default");
	vParser.fParse({ "boo", "abc", "def", "--foo", "--bar", "baz" });
	assert(vParser.fVetCmd());
	assert(vParser.fGetCmdKey() == "boo");
	assert(rParserCmd.vArgPak.size() == 2);
	assert(rParserCmd.fVetArg("foo"));
	assert(rParserCmd.fGetOpt("bar") == "baz");
	dPrintSuffix();
} // fTestCmd
// // main
int fTest()
{
	std::setbuf(stdout, NULL);
	dPrintPrefix();
	fTestFlag();
	fTestOpt();
	fTestArg();
	fTestCmd();
	dPrintSuffix();
	return 0;
}
int main()
{
	return fTest();
}
#endif // lTermArgs
