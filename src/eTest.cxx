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
// // Opt
void fTestOptNotFound()
{
	dPrintPrefix();
	nTermArgs::tCmd vCmd;
	dPrintBlock({
		vCmd.fSetOpt("foo f", "default");
		vCmd.fParse({"abc", "def"});
	});
	dPrintAssert(vCmd.fVetOptKey("foo", 1));
	dPrintAssert(vCmd.fVetOptVal("foo", "default"));
	dPrintSuffix();
} // fTestOptNotFound
void fTestOptL()
{
	dPrintPrefix();
	nTermArgs::tCmd vCmd;
	dPrintBlock({
		vCmd.fSetOpt("foo f", "default");
		vCmd.fParse({"--foo", "bar"});
	});
	dPrintAssert(vCmd.fVetOptKey("foo", 1));
	dPrintAssert(vCmd.fVetOptVal("foo", "bar"));
	dPrintSuffix();
} // fTestOptL
void fTestOptS()
{
	dPrintPrefix();
	nTermArgs::tCmd vCmd;
	dPrintBlock({
		vCmd.fSetOpt("foo f", "default");
		vCmd.fParse({"-f", "bar"});
	});
	dPrintAssert(vCmd.fVetOptKey("foo", 1));
	dPrintAssert(vCmd.fVetOptVal("foo", "bar"));
	dPrintSuffix();
} // fTestOptS
void fTestOptCondense()
{
	dPrintPrefix();
	nTermArgs::tCmd vCmd;
	dPrintBlock({
		vCmd.fSetOpt("foo f", "default");
		vCmd.fParse({"-ff", "bar", "baz"});
	});
	dPrintAssert(vCmd.fVetOptKey("foo", 1));
	dPrintAssert(vCmd.fVetOptVal("foo", "baz"));
	dPrintSuffix();
} // fTestOptCondense
void fTestOptMultiple()
{
	dPrintPrefix();
	nTermArgs::tCmd vCmd;
	dPrintBlock({
		vCmd.fSetOpt("foo f", "default");
		vCmd.fParse({"-ff", "bar", "baz", "--foo", "bam"});
	});
	dPrintAssert(vCmd.fVetOptKey("foo", 1));
	dPrintAssert(vCmd.fVetOptVal("foo", "bam"));
	dPrintSuffix();
} // fTestOptMultiple
void fTestOptParseSwitch()
{
	dPrintPrefix();
	nTermArgs::tCmd vCmd;
	dPrintBlock({ vCmd.fParse({"foo", "--", "--bar", "--baz"}); });
	dPrintAssert(vCmd.fVetArgArr(3));
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
void fTestArgArr()
{
	dPrintPrefix();
	nTermArgs::tCmd vCmd;
	dPrintBlock({ vCmd.fParse({"abc", "def"}); });
	dPrintAssert(vCmd.fVetArgArr(2));
	dPrintAssert(vCmd.fVetArgVal("abc"));
	dPrintAssert(vCmd.fVetArgVal("def"));
	dPrintSuffix();
} // fTestArgArr
void fTestArg()
{
	dPrintPrefix();
	fTestArgArr();
	dPrintSuffix();
} // fTestArg
// // Cmd
void fTestCmd()
{
	dPrintPrefix();
	nTermArgs::tCmd vCmd;
	auto			pCmdCmd = vCmd.fSetCmd("boo");
	dPrintBlock({
		pCmdCmd->fSetOpt("foo", "none");
		pCmdCmd->fSetOpt("bar", "default");
		vCmd.fParse({"boo", "abc", "def", "--foo", "for", "--bar", "baz"});
	});
	dPrintAssert(vCmd.fVetCmdKey("boo", 1));
	dPrintAssert(pCmdCmd->fVetArgArr(2));
	dPrintAssert(pCmdCmd->fVetOptKey("foo", 1));
	dPrintAssert(pCmdCmd->fVetOptVal("bar", "baz"));
	dPrintSuffix();
} // fTestCmd
// // main
int fTest()
{
	dPrintPrefix();
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
