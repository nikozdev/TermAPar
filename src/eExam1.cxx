#include "lTermArgs.hxx"

int main(int vArgC, char **vArgV)
{
	nTermArgs::tParser vParser("Usage: example...", "1.0");

	vParser.fRegFlag("foo f");
	vParser.fRegOpt("bar b", "default");

	vParser.fParse(vArgC, vArgV);
	vParser.fPrint();

	return 0;
}
