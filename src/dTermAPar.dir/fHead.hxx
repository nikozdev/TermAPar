#ifndef dTermAParHeadHxx
#define dTermAParHeadHxx

#include <algorithm>
#include <cctype>

#include <deque>
#include <map>
#include <memory>
#include <vector>

#include <iostream>
#include <string>

#include <functional>

#ifdef dTermAParLibsPickFormatFmt
#include "fmt/format.h"
namespace nTermAPar
{
namespace nFormat = ::fmt;
}//namespace nTermAPar
#endif//dTermAParLibsPickFormatFmt
#ifdef dTermAParLibsPickFormatStd
#include <format>
namespace nTermAPar
{
namespace nFormat = std;
}//namespace nTermAPar
#endif//dTermAParLibsPickFormatStd

namespace nTermAPar
{

//operats

template<typename t>
std::ostream &operator<<(std::ostream &vStream, const std::vector<t> &vVector)
{
	vStream << "[";
	for(size_t vIter = 0; vIter < vVector.size(); ++vIter)
	{
		if(vIter)
		{
			vStream << ", ";
		}
		vStream << vVector[vIter];
	}
	vStream << "]";
	return vStream;
}//operator<<(...tVector)

//typedef

using tSize = size_t;

using tStr = std::string_view;

using tArg	  = tStr;
using tArgKey = tSize;
using tArgVal = tStr;
using tKey	  = tArg;
using tCmdKey = tArg;
using tOpt	  = tArg;
using tOptKey = tArg;
using tOptVal = tArg;
using tArgArr = std::vector<tArg>;

typedef struct tArgStream
{
public://codetor

	tArgStream() = default;
	template<typename... tArgPack>
	tArgStream(tArgPack &&...vArgPack): vArgQueue{vArgPack...}
	{
	}

public://setters

	void fAdd(const tArg &vArg)
	{
		this->vArgQueue.push_back(vArg);
	}//fAdd

public://getters

	tArg fPop()
	{
		tArg vArg = this->vArgQueue.front();
		this->vArgQueue.pop_front();
		return vArg;
	}//fPop

public://vetters

	bool fVet()
	{
		return this->vArgQueue.size();
	}//fVet

private://datadef

	std::deque<tArg> vArgQueue;
} tArgStream;//argument stream type
typedef class tCmd
{
public://typedef

	using tCmdFun = bool(tCmd &rCmd);
	using tCmdAct = std::function<tCmdFun>;
	using tCmdPtr = std::shared_ptr<tCmd>;
	using tCmdTab = std::map<tKey, tCmdPtr>;
	using tOptPtr = std::shared_ptr<tOpt>;
	using tOptTab = std::map<tKey, tOptPtr>;
	friend class nFormat::formatter<tCmd>;

public://codetor

	tCmd(): vHelpText{""}, vParsed{0} {};
	tCmd(const tCmd &copy) = default;
	tCmd(tCmd &&move)	   = default;

	~tCmd() = default;

public://getters

	[[nodiscard("get the help message")]]
	auto fGetHelpText() const
	{
		return this->vHelpText;
	}//fGetHelpText
	[[nodiscard("get command key detected by argument parsing")]]
	auto fGetCmdKey() const
	{
		return this->vCmdKey;
	}//fGetCmdKey
	[[nodiscard("get command pointer detected by argument parsing")]]
	auto fGetCmdPtr() const
	{
		return this->vCmdPtr;
	}//fGetCmdPtr
	[[nodiscard("get command pointer by key")]]
	auto fGetCmdPtr(const tKey vKey) const
	{
		auto rI = this->vCmdTab.find(vKey);
		return (rI == this->vCmdTab.end()) ? rI->second : tCmdPtr{};
	}//fGetCmdPtr
	[[nodiscard("get option pointer by key")]]
	auto fGetOptPtr(const tOptKey vKey) const
	{
		auto rI = this->vOptTab.find(vKey);
		return (rI == this->vOptTab.end()) ? rI->second : tOptPtr{};
	}//fGetOptPtr
	[[nodiscard("get option value by key")]]
	auto fGetOptVal(const tOptKey vKey) const
	{
		return this->fVetOptKey(vKey) ? *this->vOptTab.find(vKey)->second : tOpt{};
	}//fGetOptVal
	[[nodiscard("get argument value by key")]]
	auto fGetArgVal(const tArgKey vKey) const
	{
		return this->vArgArr[vKey];
	}//fGetArgVal
	[[nodiscard("get argument array reference")]]
	auto fGetArgArr() const
	{
		return this->vArgArr;
	}//fGetArgArr

public://vetters

	[[nodiscard("command table is not empty")]]
	bool fVetCmdTab(bool vVet = 1) const
	{
		return this->vCmdTab.empty() != vVet;
	}//fVetCmdTab
	[[nodiscard("command table contains the given key")]]
	bool fVetCmdKey(tCmdKey vKey, bool vVet = 1) const
	{
		return (this->vCmdTab.find(vKey) != this->vCmdTab.end()) == vVet;
	}//fVetCmdKey
	[[nodiscard("command was passed as an argument")]]
	bool fVetCmdKey(bool vVet = 1) const
	{
		return this->fVetCmdKey(this->fGetCmdKey(), vVet);
	}//fVetCmdKey
	[[nodiscard("option table is not empty")]]
	bool fVetOptTab(bool vVet = 1) const
	{
		return this->vOptTab.empty() != vVet;
	}//fVetOptTab
	[[nodiscard("option table contains the given key")]]
	bool fVetOptKey(tOptKey vKey, bool vVet = 1) const
	{
		return (this->vOptTab.find(vKey) != this->vOptTab.end()) == vVet;
	}//fVetOptKey
	[[nodiscard("option table finds the given value by the given key")]]
	bool fVetOptVal(tOptKey vKey, tOptVal vOpt, bool vVet = 1) const
	{
		auto  rI   = this->vOptTab.find(vKey);
		auto &rPtr = (rI->second);
		return (rI == this->vOptTab.end() ? 0 : (*rPtr == vOpt)) == vVet;
	}//fVetOptVal
	[[nodiscard("argument array has contains given count of arguments")]]
	bool fVetArgArr(tSize vSize = 1, bool vVet = 1) const
	{
		return (this->vArgArr.size() >= vSize) == vVet;
	}//fVetArgArrCount
	[[nodiscard("argument array contains the given argument value")]]
	bool fVetArgVal(tArg vArg) const
	{
		return std::find(this->vArgArr.begin(), this->vArgArr.end(), vArg)
			!= this->vArgArr.end();
	}//fVetArgVal
	[[nodiscard(
		"arguments were successfully parsed"
		"no reset calls were since then"
	)]]
	bool fVetParsed() const
	{
		return this->vParsed;
	}//fVetParsed

public://setters

	auto fSetHelpText(tStr vHelpText)
	{
		this->vHelpText = vHelpText;
		return *this;
	}//fSetHelpText
	auto fSetCmdAct(const tCmdAct &fCmdAct)
	{
		this->fCmdAct = fCmdAct;
		return *this;
	}//fSetCmdAct
	auto fSetCmd(tCmdKey vKey, const tCmdAct &fCmdAct = fCmdFun)
	{
		tCmdPtr pCmd = std::make_shared<tCmd>();
		pCmd->fSetCmdAct(fCmdAct);
		tKey::size_type vPosWas = 0, vPosNow = 0;
		while(vPosNow != std::string::npos)
		{
			vPosNow		  = vKey.find(" ", vPosWas);
			tKey vSub	  = vKey.substr(vPosWas, vPosNow - vPosWas);
			vCmdTab[vSub] = pCmd;
			vPosWas		  = vPosNow + 1;
		}
		return pCmd;
	}//fSetCmd
	auto fSetOpt(tOptKey vKey, tOptVal vVal = "")
	{
		tOptPtr			pOpt	= std::make_shared<tOpt>(vVal);
		tKey::size_type vPosWas = 0, vPosNow;
		while(vPosNow != std::string::npos)
		{
			vPosNow		  = vKey.find(" ", vPosWas);
			tKey vSub	  = vKey.substr(vPosWas, vPosNow - vPosWas);
			vOptTab[vSub] = pOpt;
			vPosWas		  = vPosNow + 1;
		}
		return pOpt;
	}//fSetOpt

private://actions

	[[maybe_unused]]
	auto fExitHelp(int vCode = 0)
	{
		nFormat::println(stdout, "{0}", this->vHelpText);
		return exit(vCode);
	}//fExitHelp
	[[maybe_unused]]
	bool fParseOptE(const tArg &rPfx, const tKey &vKey, const tArg &rArg)
	{
		if(this->fVetOptKey(vKey))
		{
			if(rArg.empty())
			{
				nFormat::println(
					stderr, "[{0}{1}] option is missing value", rPfx, vKey
				);
				this->fExitHelp(1);
				return this->fReset();
			}
			else
			{
				*this->vOptTab[vKey] = rArg;
			}
		}//found option
		else
		{
			nFormat::
				println(stderr, "[{0}{1}] option is not defined", rPfx, vKey);
			this->fExitHelp(1);
			return this->fReset();
		}//not an option
		return 1;
	}//fParseOptE
	[[maybe_unused]]
	bool fParseOptL(const tArg &rArg, tArgStream &rArgStream)
	{
		if(tSize vPos = rArg.find("="); vPos != tArg::npos)
		{
			tKey vKey = rArg.substr(0, vPos);
			tArg vArg = rArg.substr(vPos + 1);
			return this->fParseOptE("--", vKey, vArg);
		}//--opt=val
		else if(this->fVetOptKey(rArg))
		{
			if(rArgStream.fVet())
			{
				*this->vOptTab[rArg] = rArgStream.fPop();
				return 1;
			}
			else
			{
				nFormat::println(stderr, "[{0}] option is missing a value", rArg);
				this->fExitHelp(1);
				return this->fReset();
			}
		}//found option
		else if(rArg == "help" && this->vHelpText != "")
		{
			this->fExitHelp(0);
			return 1;
		}//no option found - try the default help option
		else
		{
			nFormat::println(
				stderr, "[{0}] option is not defined; {1}", rArg, __LINE__
			);
			this->fExitHelp(1);
			return this->fReset();
		}//not an option
		return this->fReset();
	}//fParseOptL
	[[maybe_unused]]
	bool fParseOptS(const tArg &rArg, tArgStream &rArgStream)
	{
		if(tSize vPos = rArg.find("="); vPos != tStr::npos)
		{
			tKey vKey = rArg.substr(0, vPos);
			tArg vVal = rArg.substr(vPos + 1);
			return fParseOptE("-", vKey, vVal);
		}//-o=v
		for(auto vI = 0; vI < rArg.length(); vI++)
		{//iterate over all short options
			tKey vKey = rArg.substr(vI, 1);
			if(this->fVetOptKey(vKey))
			{
				if(rArgStream.fVet())
				{
					*this->vOptTab[vKey] = rArgStream.fPop();
					continue;
				}
				else
				{
					if(rArg.size() > 1)
					{
						nFormat::println(
							stderr,
							"[{0}] option is missing a value in [{1}] argument",
							vKey,
							rArg
						);
					}
					else
					{
						nFormat::println(
							stderr, "[{0}] option is missing a value", vKey
						);
					}
					this->fExitHelp(1);
					return this->fReset();
				}
			}//found option
			else if(vKey == "h" && this->vHelpText != "")
			{
				this->fExitHelp(0);
				return 1;
			}//no option found - try default help option
			else if(rArg.size() > 1)
			{
				nFormat::println(
					stderr, "[{0}] option in ({1}) is not defined", vKey, rArg
				);
			}
			else
			{
				nFormat::println(stderr, "[{0}] option is not defined", vKey);
			}
			this->fExitHelp(1);
			return this->fReset();
		}//iterate over all options
		return 1;
	}//fParseOptS
	[[maybe_unused]]
	bool fParseAll(tArgStream &rArgStream)
	{
		bool vArgIs1st = 1;
		while(rArgStream.fVet())
		{
			tArg vArg = rArgStream.fPop();
			if(vArg == "--")
			{
				while(rArgStream.fVet())
				{
					vArg = rArgStream.fPop();
					if(vArg == "--")
					{
						return fParseAll(rArgStream);
					}
					else
					{
						this->vArgArr.push_back(vArg);
					}
				}
				continue;
			}//not options, not command - write to the argument array
			else if(vArg.compare(0, 2, "--") == 0)
			{
				this->fParseOptL(vArg.substr(2), rArgStream);
				continue;
			}//option in a long format
			else if(vArg[0] == '-')
			{
				if(vArg.size() == 1 || std::isdigit(vArg[1]))
				{
					this->vArgArr.push_back(vArg);
				}
				else
				{
					this->fParseOptS(vArg.substr(1), rArgStream);
				}
				continue;
			}//option in a short format
			else if(vArgIs1st && this->vCmdTab.count(vArg))
			{
				tCmdPtr pCmd  = this->vCmdTab[vArg];
				this->vCmdKey = vArg;
				pCmd->fParseAll(rArgStream);
			}//command
			else if(vArgIs1st && vArg == "help")
			{
				if(this->fVetCmdTab())
				{
					if(rArgStream.fVet())
					{
						tKey vKey = rArgStream.fPop();
						if(this->fVetCmdKey(vKey))
						{
							this->vCmdTab[vKey]->fExitHelp(0);
							return 1;
						}//not a command
						else
						{
							nFormat::println(
								stderr,
								"[{0}] command does not have a help message",
								vKey
							);
							this->fExitHelp(1);
							return this->fReset();
						}//help for the command
						return this->fReset();
					}//help for a command
					else
					{
						this->fExitHelp(0);
						return 1;
					}//just write our help
				}
				else if(this->vHelpText != "")
				{
					this->fExitHelp(0);
					return 1;
				}//no option found - try the default help option
				return this->fReset();
			}//not an option, not a command - try default help
			this->vArgArr.push_back(vArg);
			vArgIs1st = 0;
		}//parse all arguments from the stream one by one
		return this->fCmdAct ? this->fCmdAct(*this) : 1;
	}//fParseAll
	[[maybe_unused]]
	bool fReset()
	{
		if(this->fVetParsed())
		{
			this->vCmdKey = "";
			this->vCmdPtr = 0;
			this->vCmdTab.clear();
			this->vOptTab.clear();
			this->vArgArr.clear();
			this->vParsed = 0;
		}
		else
		{
			nFormat::println(stderr, "cannot reset without parsing");
		}
		return this->fVetParsed() == 0;
	}//fReset
	[[maybe_unused]]
	inline static constexpr bool fCmdFun(tCmd &rCmd)
	{
		return 1;
	};

public://actions

	[[maybe_unused]]
	bool fParse(const int vArgC, char **vArgV)
	{
		if(this->fVetParsed())
		{
			if(this->fReset() == 0)
			{
				nFormat::println(stderr, "failed reset before parsing");
				return 0;
			}
		}
		else
		{
			this->vParsed = 1;
		}
		tArgStream vArgStream;
		for(int vIter = 1; vIter < vArgC; vIter++)
		{
			vArgStream.fAdd(vArgV[vIter]);
		}
		return this->fParseAll(vArgStream);
	}//fParse
	[[maybe_unused]]
	bool fParse(const int vArgC, const char **vArgV)
	{
		return this->fParse(vArgC, vArgV);
	}
	[[maybe_unused]]
	bool fParse(const tArgArr &rArr)
	{
		if(this->fVetParsed())
		{
			if(this->fReset() == 0)
			{
				nFormat::println(stderr, "failed reset before parsing");
				return this->fReset();
			}
		}
		else
		{
			this->vParsed = 1;
		}
		tArgStream vArgStream;
		for(const tArg &rArg: rArr)
		{
			vArgStream.fAdd(rArg);
		}
		return this->fParseAll(vArgStream);
	}//fParse
	[[maybe_unused]]
	auto fPrintHelp()
	{
		nFormat::println(stdout, "{0}", this->vHelpText);
		return 1;
	}//fPrintHelp

private://datadef

	tStr	vHelpText;//for "help" command or "--help|-h" option
	tCmdAct fCmdAct;  //command action or a function to call after parsing
	tCmdKey vCmdKey;  //command pointer taken from the parsed arguments
	tCmdPtr vCmdPtr;  //command key taken from the parsed arguments
	tCmdTab vCmdTab;  //command registry
	tOptTab vOptTab;  //option registry
	tArgArr vArgArr;  //the rest of the arguments in an array
	bool	vParsed;  //whether or not we were parsing

} tArgParser, tCmd;//arg parser or the command type
using tOptPtr = tCmd::tOptPtr;
using tCmdPtr = tCmd::tCmdPtr;

}//namespace nTermAPar

//typedef
template<>
class ::nTermAPar::nFormat::formatter<::nTermAPar::tCmd>
{
public://typedef

	using tVal = ::nTermAPar::tCmd;

public://actions

	template<typename tCtx>
	constexpr auto parse(tCtx &rCtx)
	{
		for(auto vI = rCtx.begin(); vI != rCtx.end(); vI++)
		{
		}
		return rCtx.end();
	}//parse
	template<typename tCtx>
	constexpr auto format(const tVal &rVal, tCtx &rCtx) const
	{
		using namespace nTermAPar;
		nFormat::format_to(rCtx.out(), "(");
		nFormat::format_to(rCtx.out(), "[{0}]=(", "ArgArr");
		for(auto vI = 0; vI < rVal.vArgArr.size(); vI++)
		{
			nFormat::format_to(
				rCtx.out(), "[{0}]=({1})=[{0}]", vI, rVal.vArgArr[vI]
			);
		}
		nFormat::format_to(rCtx.out(), ")=[{0}]", "ArgArr");
		nFormat::format_to(rCtx.out(), "[{0}]=(", "OptTab");
		for(auto vI: rVal.vOptTab)
		{
			nFormat::format_to(
				rCtx.out(), "[{0}]=({1})=[{0}]", vI.first, *vI.second
			);
		}
		nFormat::format_to(rCtx.out(), ")=[{0}]", "OptTab");
		nFormat::format_to(rCtx.out(), "[{0}]=(", "CmdTab");
		for(auto vI: rVal.vCmdTab)
		{
			nFormat::format_to(
				rCtx.out(), "[{0}]=({1})=[{0}]", vI.first, *vI.second
			);
		}
		nFormat::format_to(rCtx.out(), ")=[{0}]", "CmdTab");
		nFormat::
			format_to(rCtx.out(), "[{0}]=({1})=[{0}]", "CmdKey", rVal.vCmdKey);
		return nFormat::format_to(rCtx.out(), ")");
	}//format

private://datadef

	bool _;
};//tCmd formatter

#endif//dTermAParHeadHxx
