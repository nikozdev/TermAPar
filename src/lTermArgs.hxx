#ifndef dTermArgsHxx
#define dTermArgsHxx

#include <algorithm>
#include <cctype>

#include <map>
#include <vector>
#include <deque>
#include <memory>

#include <string>
#include <iostream>

#ifdef dTermArgsLibsPickFormatFmt
#	include "fmt/format.h"
namespace nTermArgs
{
namespace nFormat = ::fmt;
} // namespace nTermArgs
#endif // dTermArgsLibsPickFormatFmt
#ifdef dTermArgsLibsPickFormatStd
#	include <format>
namespace nTermArgs
{
namespace nFormat = std;
} // namespace nTermArgs
#endif // dTermArgsLibsPickFormatStd

namespace nTermArgs
{

// operats

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
} // operator<<(...tVector)

// typedef

using tStr = std::string_view;

using tArg	  = tStr;
using tKey	  = tArg;
using tOpt	  = tArg;
using tArgArr = std::vector<tArg>;

typedef struct tArgStream
{
public: // codetor
	tArgStream() = default;
	template<typename... tArgPack>
	tArgStream(tArgPack &&...vArgPack): vArgQueue{vArgPack...}
	{
	}
public: // setters
	void fAdd(const tArg &vArg)
	{
		this->vArgQueue.push_back(vArg);
	}	// fAdd
public: // getters
	tArg fPop()
	{
		tArg vArg = this->vArgQueue.front();
		this->vArgQueue.pop_front();
		return vArg;
	}	// fPop
public: // vetters
	bool fVet()
	{
		return this->vArgQueue.size();
	}	 // fVet
private: // datadef
	std::deque<tArg> vArgQueue;
} tArgStream; // argument stream type
typedef class tCmd
{
public: // typedef
	typedef void(tCallback)(tCmd &rCmd);
	using tCmdPtr = std::shared_ptr<tCmd>;
	using tCmdTab = std::map<tKey, tCmdPtr>;
	using tOptPtr = std::shared_ptr<tOpt>;
	using tOptTab = std::map<tKey, tOptPtr>;
	friend class nFormat::formatter<tCmd>;
public: // codetor
	tCmd()	= default;
	~tCmd() = default;
public: // getters
	tCmdPtr fGetCmdPtr(const tKey vKey) const
	{
		auto rI = this->vCmdTab.find(vKey);
		return (rI == this->vCmdTab.end()) ? rI->second : tCmdPtr{};
	} // fGetCmdPtr
	tOptPtr fGetOptPtr(const tKey vKey) const
	{
		auto rI = this->vOptTab.find(vKey);
		return (rI == this->vOptTab.end()) ? rI->second : tOptPtr{};
	} // fGetOptPtr
	tOpt fGetOptVal(const tKey vKey) const
	{
		return this->fVetOptKey(vKey) ? *this->vOptTab.find(vKey)->second : tOpt{};
	} // fGetOptVal
	auto fGetArgVal(const size_t vKey) const
	{
		return this->vArgArr[vKey];
	} // fGetArgVal
	auto fGetArgArr() const
	{
		return this->vArgArr;
	}	// fGetArgArr
public: // vetters
	bool fVetCmdTab(const bool vVet = 1) const
	{
		return this->vCmdTab.empty() != vVet;
	} // fVetCmdTab
	bool fVetCmdKey(const tKey vKey, const bool vVet = 1) const
	{
		return (this->vCmdTab.find(vKey) != this->vCmdTab.end()) == vVet;
	} // fVetCmdKey
	bool fVetOptTab(const bool vVet = 1) const
	{
		return this->vOptTab.empty() != vVet;
	} // fVetOptTab
	bool fVetOptKey(const tKey vKey, const bool vSet = 1) const
	{
		return (this->vOptTab.find(vKey) != this->vOptTab.end()) == vSet;
	} // fVetOptKey
	bool fVetOptVal(const tKey vKey, const tOpt &rOpt, const bool vVet = 1) const
	{
		auto  rI   = this->vOptTab.find(vKey);
		auto &rPtr = (rI->second);
		return (rI == this->vOptTab.end() ? 0 : (*rPtr == rOpt)) == vVet;
	} // fVetOptVal
	bool fVetArgArr(size_t count, const bool vVet = 1) const
	{
		return (this->vArgArr.size() >= count) == vVet;
	}
	bool fVetArgVal(const tArg &rArg) const
	{
		return std::find(this->vArgArr.begin(), this->vArgArr.end(), rArg)
			!= this->vArgArr.end();
	}	// fVetArgVal
public: // setters
	auto &fSetHelpText(const tStr &rHelpText)
	{
		this->vHelpText = rHelpText;
		return *this;
	} // fSetHelpText
	auto &fSetCallback(tCallback *pCallback)
	{
		this->pCallback = pCallback;
		return *this;
	} // fSetCallback
	auto fSetCmd(const tKey vKey, tCallback *pCallback = nullptr)
	{
		tCmdPtr pCmd = std::make_shared<tCmd>();
		pCmd->fSetCallback(pCallback);
		tKey::size_type vPosWas = 0, vPosNow = 0;
		while(vPosNow != std::string::npos)
		{
			vPosNow		  = vKey.find(" ", vPosWas);
			tKey vSub	  = vKey.substr(vPosWas, vPosNow - vPosWas);
			vCmdTab[vSub] = pCmd;
			vPosWas		  = vPosNow + 1;
		}
		return pCmd;
	} // fSetCmd
	auto fSetOpt(const tKey vKey, const tStr &rDef = "")
	{
		tOptPtr			pOpt	= std::make_shared<tOpt>(rDef);
		tKey::size_type vPosWas = 0, vPosNow;
		while(vPosNow != std::string::npos)
		{
			vPosNow		  = vKey.find(" ", vPosWas);
			tKey vSub	  = vKey.substr(vPosWas, vPosNow - vPosWas);
			vOptTab[vSub] = pOpt;
			vPosWas		  = vPosNow + 1;
		}
		return pOpt;
	}	 // fSetOpt
private: // actions
	auto fExitHelp(int vCode = 0)
	{
		nFormat::print(stdout, "{}", this->vHelpText);
		return exit(vCode);
	} // fExitHelp
	bool fParseOptE(const tArg &rPfx, const tKey &vKey, const tArg &rArg)
	{
		if(this->fVetOptKey(vKey))
		{
			if(rArg.empty())
			{
				std::cerr
					<< "missing value for " << rPfx << vKey << ";" << __LINE__
					<< std::endl;
				this->fExitHelp(1);
				return 0;
			}
			else
			{
				*this->vOptTab[vKey] = rArg;
			}
		} // found option
		else
		{
			std::cerr
				<< rPfx << vKey << " - is not an option;" << __LINE__
				<< std::endl;
			this->fExitHelp(1);
			return 0;
		} // not an option
		return 1;
	} // fParseOptE
	bool fParseOptL(const tArg &rArg, tArgStream &rArgStream)
	{
		if(size_t vPos = rArg.find("="); vPos != std::string::npos)
		{
			tKey vKey = rArg.substr(0, vPos);
			tArg vArg = rArg.substr(vPos + 1);
			return this->fParseOptE("--", vKey, vArg);
		} // --opt=val
		else if(this->fVetOptKey(rArg))
		{
			if(rArgStream.fVet())
			{
				*this->vOptTab[rArg] = rArgStream.fPop();
				return 1;
			}
			else
			{
				std::cerr
					<< "missing argument for --" << rArg << ";" << __LINE__
					<< std::endl;
				this->fExitHelp(1);
				return 0;
			}
		} // found option
		else if(rArg == "help" && this->vHelpText != "")
		{
			this->fExitHelp(0);
			return 1;
		} // no option found - try the default help option
		else
		{
			std::cerr
				<< "--" << rArg << " - is not an option;" << __LINE__
				<< std::endl;
			this->fExitHelp(1);
			return 0;
		} // not an option
		return 0;
	} // fParseOptL
	bool fParseOptS(const tArg &rArg, tArgStream &rArgStream)
	{
		if(size_t vPos = rArg.find("="); vPos != tStr::npos)
		{
			tKey vKey = rArg.substr(0, vPos);
			tArg vVal = rArg.substr(vPos + 1);
			return fParseOptE("-", vKey, vVal);
		} // -o=v
		for(auto vI = 0; vI < rArg.length(); vI++)
		{ // iterate over all short options
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
						std::cerr
							<< "missing argument for '" << vKey << "' in -"
							<< rArg << ";" << __LINE__ << std::endl;
					}
					else
					{
						std::cerr
							<< "missing argument for -" << vKey << ";"
							<< __LINE__ << std::endl;
					}
					this->fExitHelp(1);
					return 0;
				}
			} // found option
			else if(vKey == "h" && this->vHelpText != "")
			{
				this->fExitHelp(0);
				return 1;
			} // no option found - try default help option
			else if(rArg.size() > 1)
			{
				std::cerr
					<< vI << rArg << " is not an option;" << __LINE__
					<< std::endl;
			}
			else
			{
				std::cerr << vI << " - is not an option;" << __LINE__ << std::endl;
			}
			this->fExitHelp(1);
			return 0;
		} // iterate over all options
		return 1;
	} // fParseOptS
	bool fParse(tArgStream &rArgStream)
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
						return fParse(rArgStream);
					}
					this->vArgArr.push_back(vArg);
				}
				continue;
			} // not options, not commands - write to the argument array
			else if(vArg.compare(0, 2, "--") == 0)
			{
				this->fParseOptL(vArg.substr(2), rArgStream);
				continue;
			} // option in a long format
			else if(vArg.compare(0, 1, "-") == 0)
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
			} // option in a short format
			else if(vArgIs1st && this->vCmdTab.count(vArg))
			{
				tCmdPtr pCmd = this->vCmdTab[vArg];
				pCmd->fParse(rArgStream);
				if(pCmd->pCallback)
				{
					pCmd->pCallback(*pCmd);
				}
			} // command
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
						} // not a command
						else
						{
							std::cerr
								<< vKey << " - no help found for this;"
								<< __LINE__ << std::endl;
							this->fExitHelp(1);
							return 0;
						} // help for the command
						return 0;
					} // help for a command
					else
					{
						this->fExitHelp(0);
						return 1;
					} // just write our help
				}
				else if(this->vHelpText != "")
				{
					this->fExitHelp(0);
					return 1;
				} // no option found - try the default help option
				return 0;
			} // not an option, not a command - try default help
			this->vArgArr.push_back(vArg);
			vArgIs1st = 0;
		} // parse all arguments from the stream one by one
		return 1;
	}	// fParse
public: // actions
	template<typename tArgV = const char **>
	bool fParse(int vArgC, tArgV vArgV)
	{
		if(vArgC <= 1)
		{
			return 1;
		}
		tArgStream vArgStream;
		for(int vIter = 1; vIter < vArgC; vIter++)
		{
			vArgStream.fAdd(vArgV[vIter]);
		}
		this->fParse(vArgStream);
		return 1;
	} // fParse
	bool fParse(const tArgArr &rArr)
	{
		tArgStream vArgStream;
		for(const tArg &rArg: rArr)
		{
			vArgStream.fAdd(rArg);
		}
		this->fParse(vArgStream);
		return 1;
	} // fParse
	[[deprecated("use nFormat::format, nFormat::print instead")]] bool
	fPrint(std::string vIndentStr = "") const
	{
		std::cout << vIndentStr << "[CmdTab]=(" << std::endl;
		for(auto &rI: this->vCmdTab)
		{
			const tKey	vK = rI.first;
			const tCmd &rC = *rI.second;
			std::cout << vIndentStr << " [" << vK << "]=(" << std::endl;
			rC.fPrint(vIndentStr + "  ");
			std::cout << vIndentStr << " )=[" << vK << "]" << std::endl;
		}
		std::cout << vIndentStr << ")=[CmdTab]" << std::endl;
		std::cout << vIndentStr << "[OptTab]=(" << std::endl;
		for(auto &rI: this->vOptTab)
		{
			const tKey	vK = rI.first;
			const tOpt &rO = *rI.second;
			std::cout
				<< vIndentStr << " [" << vK << "]=(" << rO << ")" << std::endl;
		}
		std::cout << vIndentStr << ")=[OptTab]" << std::endl;
		std::cout << vIndentStr << "[ArgArr]=(" << std::endl;
		for(auto vI = 0; vI < this->vArgArr.size(); vI++)
		{
			const auto &rA = this->vArgArr[vI];
			std::cout
				<< vIndentStr << " [" << vI << "]=(" << rA << ")" << std::endl;
		}
		std::cout << vIndentStr << ")=[ArgArr]" << std::endl;
		return 1;
	}	 // fPrint
private: // datadef
	tStr	   vHelpText;
	tCallback *pCallback;
	tCmdTab	   vCmdTab;
	tOptTab	   vOptTab;
	tArgArr	   vArgArr;
} tArgParser, tCmd; // arg parser or the command type
using tOptPtr = tCmd::tOptPtr;
using tCmdPtr = tCmd::tCmdPtr;

} // namespace nTermArgs

// typedef
template<>
class ::nTermArgs::nFormat::formatter<::nTermArgs::tCmd>
{
public: // typedef
	using tVal = ::nTermArgs::tCmd;
public: // actions
	template<typename tCtx>
	constexpr auto parse(tCtx &rCtx)
	{
		for(auto vI = rCtx.begin(); vI != rCtx.end(); vI++)
		{
		}
		return rCtx.end();
	} // parse
	template<typename tCtx>
	constexpr auto format(const tVal &rVal, tCtx &rCtx) const
	{
		using namespace nTermArgs;
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
		return nFormat::format_to(rCtx.out(), ")");
	}	 // format
private: // datadef
	bool _;
}; // cmd formatter

#endif // dTermArgsHxx
