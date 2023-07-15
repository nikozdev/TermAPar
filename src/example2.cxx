#include <iostream>
#include "lTermArgs.hxx"

void callback(std::string cmd_name, nTermArgs::ArgParser &cmd_parser)
{
	std::cout << "---------- boo! ----------\n";
	cmd_parser.print();
	std::cout << "--------------------------\n\n";
}

int main(int argc, char **argv)
{
	nTermArgs::ArgParser parser("Usage: example...", "1.0");

	nTermArgs::ArgParser &cmd_parser
		= parser.command("boo", "Usage: example boo...", callback);
	cmd_parser.flag("foo f");
	cmd_parser.option("bar b", "default");

	parser.parse(argc, argv);
	parser.print();

	return 0;
}
