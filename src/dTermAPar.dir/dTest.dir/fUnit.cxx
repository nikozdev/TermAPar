#ifndef lTermAParTestUnitCxx
#define lTermAParTestUnitCxx
// headers
#include <cassert>
#include <dTermAPar.dir/fMain.hxx>
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
// // Arg
void fTestArgArr()
{
	dPrintPrefix();
	nTermAPar::tArgParser vArgParser;
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
	nTermAPar::tArgParser vArgParser;
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
	nTermAPar::tArgParser vArgParser;
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
	nTermAPar::tArgParser vArgParser;
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
	nTermAPar::tArgParser vArgParser;
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
	nTermAPar::tArgParser vArgParser;
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
	nTermAPar::tArgParser vArgParser;
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
	nTermAPar::tArgParser vArgParser;
	auto				  pArgParserCmd = vArgParser.fSetCmd("boo");
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
int fTest()
{
	dPrintPrefix();
	fTestArg();
	fTestOpt();
	fTestCmd();
	dPrintSuffix();
	return 0;
} // fTest
int main()
{
	return fTest();
} // main
#endif // lTermAParTestUnitCxx
