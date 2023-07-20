#ifndef lTermAParTestTimeCxx
#define lTermAParTestTimeCxx
// headers
#include <dTermAPar.dir/fMain.hxx>
// datadef
static constexpr auto cICount = 1'000;
// actions
// // main
int fTime()
{
	for(auto vI = 0; vI < cICount; vI++)
	{
		nTermAPar::tArgParser vArgParser;
		for(auto vI = 0; vI < cICount; vI++)
		{
			vArgParser.fSetOpt("o opt option", "v");
			vArgParser.fSetCmd("c cmd comman");
		}
	}
	return 0;
} // fTime
int main(int vArgC, char **vArgV)
{
	return fTime();
} // main
#endif // lTermAParTestTimeCxx
