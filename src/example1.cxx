#include "lTermArgs.hxx"

int main(int argc, char **argv)
{
	nTermArgs::ArgParser parser("Usage: example...", "1.0");

	parser.flag("foo f");
	parser.option("bar b", "default");

	parser.parse(argc, argv);
	parser.print();

	return 0;
}
