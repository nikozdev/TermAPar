#ifndef dTermArgsHxx
#define dTermArgsHxx

#include <map>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>
#include <iostream>
#include <sstream>
#include <deque>
#include <set>

namespace nTermArgs
{

using tStr = std::string;
using tArg = tStr;
using tPak = std::vector<tArg>;

typedef struct tStream
{
  public: // datadef
	std::deque<tArg> args;
  public: // actions
	void append(const tArg &vArg) { args.push_back(vArg); }
	tArg next()
	{
		tArg arg = args.front();
		args.pop_front();
		return arg;
	}
	bool hasNext() { return args.size() > 0; }
} tStream;
typedef struct tOption
{
	tPak values;
	tArg fallback;
} tOption;
typedef struct tFlag
{
	int count = 0;
} tFlag;

template<typename T>
std::ostream &operator<<(std::ostream &stream, const std::vector<T> &vec)
{
	stream << "[";
	for(size_t i = 0; i < vec.size(); ++i)
	{
		if(i) { std::cout << ", "; }
		stream << vec[i];
	}
	stream << "]";
	return stream;
}

typedef class ArgParser
{
  public: // typedef
  public: // codetor
	ArgParser(const tStr &helptext = "", const tStr &version = "")
		: helptext(helptext), version(version)
	{
	}
	~ArgParser()
	{
		std::set<tOption *> unique_options;
		for(auto element: options) { unique_options.insert(element.second); }
		for(auto pointer: unique_options) { delete pointer; }

		std::set<tFlag *> unique_flags;
		for(auto element: flags) { unique_flags.insert(element.second); }
		for(auto pointer: unique_flags) { delete pointer; }

		std::set<ArgParser *> unique_cmd_parsers;
		for(auto element: commands)
		{
			unique_cmd_parsers.insert(element.second);
		}
		for(auto pointer: unique_cmd_parsers) { delete pointer; }
	}
  public: // getters
  public: // vetters
	bool found(const std::string &name)
	{
		if(flags.count(name) > 0) { return flags[name]->count > 0; }
		if(options.count(name) > 0) { return options[name]->values.size() > 0; }
		return false;
	}
	bool commandFound() { return command_name != ""; }
  public: // setters
	// Register flags and options.
	void flag(const std::string &name)
	{
		tFlag			 *flag = new tFlag();
		std::stringstream stream(name);
		std::string		  alias;
		while(stream >> alias) { flags[alias] = flag; }
	}
	void option(const std::string &name, const std::string &fallback = "")
	{
		tOption *option	 = new tOption();
		option->fallback = fallback;
		std::stringstream stream(name);
		std::string		  alias;
		while(stream >> alias) { options[alias] = option; }
	}
	/* void parse(int argc, const char**argv)
	 * Parse an array of std::string arguments. We assume that [argc] and [argv]
	 * are the original parameters passed to main() and skip the first element.
	 * In some situations [argv] can be empty, i.e. [argc == 0]. This can lead
	 * to security vulnerabilities if not handled explicitly.
	 */
	void parse(int argc, const char **argv)
	{
		if(argc > 1)
		{
			tStream stream;
			for(int i = 1; i < argc; i++) { stream.append(argv[i]); }
			parse(stream);
		}
	} // parse
	void parse(int argc, char **argv)
	{
		if(argc > 1)
		{
			tStream stream;
			for(int i = 1; i < argc; i++) { stream.append(argv[i]); }
			parse(stream);
		}
	} // parse
	void parse(std::vector<std::string> args)
	{
		tStream stream;
		for(std::string &arg: args) { stream.append(arg); }
		parse(stream);
	} // parse
	int count(const std::string &name)
	{
		if(flags.count(name) > 0) { return flags[name]->count; }
		if(options.count(name) > 0) { return options[name]->values.size(); }
		return 0;
	}
	std::string value(const std::string &name)
	{
		if(options.count(name) > 0)
		{
			if(options[name]->values.size() > 0)
			{
				return options[name]->values.back();
			}
			return options[name]->fallback;
		}
		return std::string();
	}
	std::vector<std::string> values(const std::string &name)
	{
		if(options.count(name) > 0) { return options[name]->values; }
		return std::vector<std::string>();
	}
	// Register a command. Returns the command's ArgParser instance.
	ArgParser &command(
		const std::string &name,
		const std::string &helptext									  = "",
		void (*callback)(std::string cmd_name, ArgParser &cmd_parser) = nullptr
	)
	{
		ArgParser *parser = new ArgParser();
		parser->helptext  = helptext;
		parser->callback  = callback;

		std::stringstream stream(name);
		std::string		  alias;

		while(stream >> alias) { commands[alias] = parser; }

		return *parser;
	}
	std::string commandName() { return command_name; }
	ArgParser  &commandParser() { return *commands[command_name]; }
	void		print()
	{
		std::cout << "Options:\n";
		if(options.size() > 0)
		{
			for(auto element: options)
			{
				std::cout << "  " << element.first << ": ";
				tOption *option = element.second;
				std::cout << "(" << option->fallback << ") ";
				std::cout << option->values;
				std::cout << "\n";
			}
		}
		else { std::cout << "  [none]\n"; }

		std::cout << "\nFlags:\n";
		if(flags.size() > 0)
		{
			for(auto element: flags)
			{
				std::cout
					<< "  " << element.first << ": " << element.second->count
					<< "\n";
			}
		}
		else { std::cout << "  [none]\n"; }

		std::cout << "\nArguments:\n";
		if(args.size() > 0)
		{
			for(auto arg: args) { std::cout << "  " << arg << "\n"; }
		}
		else { std::cout << "  [none]\n"; }

		std::cout << "\nCommand:\n";
		if(commandFound()) { std::cout << "  " << command_name << "\n"; }
		else { std::cout << "  [none]\n"; }
	}
  private: // datadef
	std::map<std::string, tOption *>   options;
	std::map<std::string, tFlag *>	   flags;
	std::map<std::string, ArgParser *> commands;
	std::string						   command_name;
  private: // actions
	void parse(tStream &stream)
	{
		bool is_first_arg = true;

		while(stream.hasNext())
		{
			tArg arg = stream.next();

			// If we enounter a '--', turn off option parsing.
			if(arg == "--")
			{
				while(stream.hasNext()) { args.push_back(stream.next()); }
				continue;
			}

			// Is the argument a long-form option or flag?
			if(arg.compare(0, 2, "--") == 0)
			{
				parseLongOption(arg.substr(2), stream);
				continue;
			}

			// Is the argument a short-form option or flag? If the argument
			// consists of a single dash or a dash followed by a digit, we treat
			// it as a positional argument.
			if(arg[0] == '-')
			{
				if(arg.size() == 1 || isdigit(arg[1])) { args.push_back(arg); }
				else { parseShortOption(arg.substr(1), stream); }
				continue;
			}

			// Is the argument a registered command?
			if(is_first_arg && commands.count(arg) > 0)
			{
				ArgParser *command_parser = commands[arg];
				command_name			  = arg;
				command_parser->parse(stream);
				if(command_parser->callback != nullptr)
				{
					command_parser->callback(arg, *command_parser);
				}
				continue;
			}

			// Is the argument the automatic 'help' command?
			if(is_first_arg && arg == "help" && commands.size() > 0)
			{
				if(stream.hasNext())
				{
					std::string name = stream.next();
					if(commands.find(name) == commands.end())
					{
						std::cerr
							<< "Error: '" << name
							<< "' is not a recognised command.\n";
						exit(1);
					}
					else { commands[name]->exitHelp(); }
				}
				else
				{
					std::cerr
						<< "Error: the help command requires an argument.\n";
					exit(1);
				}
			}

			// Otherwise add the argument to our list of positional arguments.
			args.push_back(arg);
			is_first_arg = false;
		}
	}
	void registerOption(const std::string &name, tOption *option);
	void parseLongOption(tArg arg, tStream &stream)
	{
		size_t pos = arg.find("=");
		if(pos != std::string::npos)
		{
			parseEqualsOption("--", arg.substr(0, pos), arg.substr(pos + 1));
			return;
		}

		if(flags.count(arg) > 0)
		{
			flags[arg]->count++;
			return;
		}

		if(options.count(arg) > 0)
		{
			if(stream.hasNext())
			{
				options[arg]->values.push_back(stream.next());
				return;
			}
			else
			{
				std::cerr << "Error: missing argument for --" << arg << ".\n";
				exit(1);
			}
		}
		if(arg == "help" && this->helptext != "") { exitHelp(); }
		if(arg == "version" && this->version != "") { exitVersion(); }
		std::cerr
			<< "Error: --" << arg << " is not a recognised flag or option.\n";
		exit(1);
	}
	void parseShortOption(tArg arg, tStream &stream)
	{
		size_t pos = arg.find("=");
		if(pos != tStr::npos)
		{
			parseEqualsOption("-", arg.substr(0, pos), arg.substr(pos + 1));
			return;
		}

		for(char &c: arg)
		{
			tStr name = std::string(1, c);

			if(flags.count(name) > 0)
			{
				flags[name]->count++;
				continue;
			}

			if(options.count(name) > 0)
			{
				if(stream.hasNext())
				{
					options[name]->values.push_back(stream.next());
					continue;
				}
				else
				{
					if(arg.size() > 1)
					{
						std::cerr
							<< "Error: missing argument for '" << c << "' in -"
							<< arg << ".\n";
					}
					else
					{
						std::cerr
							<< "Error: missing argument for -" << c << ".\n";
					}
					exit(1);
				}
			}

			if(c == 'h' && this->helptext != "") { exitHelp(); }

			if(c == 'v' && this->version != "") { exitVersion(); }

			if(arg.size() > 1)
			{
				std::cerr
					<< "Error: '" << c << "' in -" << arg
					<< " is not a recognised flag or option.\n";
			}
			else
			{
				std::cerr
					<< "Error: -" << c
					<< " is not a recognised flag or option.\n";
			}
			exit(1);
		}
	}
	void
	parseEqualsOption(std::string prefix, std::string name, std::string value)
	{
		if(options.count(name) > 0)
		{
			if(value.size() > 0) { options[name]->values.push_back(value); }
			else
			{
				std::cerr
					<< "Error: missing value for " << prefix << name << ".\n";
				exit(1);
			}
		}
		else
		{
			std::cerr
				<< "Error: " << prefix << name
				<< " is not a recognised option.\n";
			exit(1);
		}
	}
	void exitHelp()
	{
		std::cout << helptext << std::endl;
		exit(0);
	}
	void exitVersion()
	{
		std::cout << version << std::endl;
		exit(0);
	}
  public: // datadef
	// Stores positional arguments.
	std::vector<std::string> args;

	// Application/command help text and version strings.
	std::string helptext;
	std::string version;

	// Callback function for command parsers.
	void (*callback)(std::string cmd_name, ArgParser &cmd_parser);
} ArgParser;

} // namespace nTermArgs

#endif // dTermArgsHxx
