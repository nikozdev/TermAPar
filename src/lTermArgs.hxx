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
#include <sstream>

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

using tStr		 = std::string;
using tStrStream = std::stringstream;

using tArg = tStr;
using tKey = tArg;
using tOpt = tArg;
using tArr = std::vector<tArg>;

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
	typedef void(tCallback)(const tKey &rKey, tCmd &rCmd);
	using tCmdPtr = std::shared_ptr<tCmd>;
	using tOptPtr = std::shared_ptr<tOpt>;
public: // codetor
	tCmd()	= default;
	~tCmd() = default;
public: // getters
	tCmdPtr fGetCmdPtr(const tKey &rKey) const
	{
		auto rI = this->vCmdTab.find(rKey);
		return (rI == this->vCmdTab.end()) ? rI->second : tCmdPtr{};
	} // fGetCmdPtr
	tOptPtr fGetOptPtr(const tKey &rKey) const
	{
		auto rI = this->vOptTab.find(rKey);
		return (rI == this->vOptTab.end()) ? rI->second : tOptPtr{};
	} // fGetOptPtr
	tOpt fGetOptVal(const tKey &rKey) const
	{
		return this->fVetOptKey(rKey) ? *this->vOptTab.find(rKey)->second : tOpt{};
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
	bool fVetCmdKey(const tKey &rKey, const bool vVet = 1) const
	{
		return (this->vCmdTab.find(rKey) != this->vCmdTab.end()) == vVet;
	} // fVetCmdKey
	bool fVetOptTab(const bool vVet = 1) const
	{
		return this->vOptTab.empty() != vVet;
	} // fVetOptTab
	bool fVetOptKey(const tKey &rKey, const bool vSet = 1) const
	{
		return (this->vOptTab.find(rKey) != this->vOptTab.end()) == vSet;
	} // fVetOptKey
	bool
	fVetOptVal(const tKey &rKey, const tOpt &rOpt, const bool vVet = 1) const
	{
		auto  rI   = this->vOptTab.find(rKey);
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
	auto fSetCmd(const tKey &rKey, tCallback *pCallback = nullptr)
	{
		tCmdPtr pCmd = std::make_shared<tCmd>();
		pCmd->fSetCallback(pCallback);
		tStrStream vStream(rKey);
		tStr	   vKey;
		while(vStream >> vKey)
		{
			this->vCmdTab[vKey] = pCmd;
		}
		return pCmd;
	} // fSetCmd
	auto fSetOpt(const tKey &rKey, const tStr &rDef = "")
	{
		tOptPtr	   pOpt = std::make_shared<tOpt>(rDef);
		tStrStream vStream(rKey);
		tStr	   vKey;
		while(vStream >> vKey)
		{
			this->vOptTab[vKey] = pOpt;
		}
		return pOpt;
	}	 // fSetOpt
private: // actions
	auto fExitHelp(int vCode = 0)
	{
		std::cout << this->vHelpText << std::endl;
		return exit(vCode);
	} // fExitHelp
	bool fParseOptE(const tArg &rPfx, const tKey &rKey, const tArg &rArg)
	{
		if(this->fVetOptKey(rKey))
		{
			if(rArg.empty())
			{
				std::cerr
					<< "missing value for " << rPfx << rKey << ";" << __LINE__
					<< std::endl;
				this->fExitHelp(1);
				return 0;
			}
			else
			{
				*this->vOptTab[rKey] = rArg;
			}
		} // found option
		else
		{
			std::cerr
				<< rPfx << rKey << " - is not an option;" << __LINE__
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
		for(auto &rI: rArg)
		{ // iterate over all short options
			tKey vKey = tStr(1, rI);
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
							<< "missing argument for '" << rI << "' in -"
							<< rArg << ";" << __LINE__ << std::endl;
					}
					else
					{
						std::cerr
							<< "missing argument for -" << rI << ";" << __LINE__
							<< std::endl;
					}
					this->fExitHelp(1);
					return 0;
				}
			} // found option
			else if(rI == 'h' && this->vHelpText != "")
			{
				this->fExitHelp(0);
				return 1;
			} // no option found - try default help option
			else if(rArg.size() > 1)
			{
				std::cerr
					<< rI << rArg << " is not an option;" << __LINE__
					<< std::endl;
			}
			else
			{
				std::cerr << rI << " - is not an option;" << __LINE__ << std::endl;
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
					pCmd->pCallback(vArg, *pCmd);
				}
			} // command
			else if(vArgIs1st && vArg == "help" && this->fVetCmdTab())
			{
				if(rArgStream.fVet())
				{
					tKey vKey = rArgStream.fPop();
					if(this->fVetCmdKey(vKey))
					{
						std::cerr
							<< vKey << " - does not have help;" << __LINE__
							<< std::endl;
						this->fExitHelp(1);
						return 0;
					} // not a command
					else
					{
						this->vCmdTab[vKey]->fExitHelp(0);
						return 1;
					} // help for the command
					return 0;
				} // help for a command
				else
				{
					this->fExitHelp(0);
					return 1;
				} // just write our help
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
	bool fParse(const tArr &rArr)
	{
		tArgStream vArgStream;
		for(const tArg &rArg: rArr)
		{
			vArgStream.fAdd(rArg);
		}
		this->fParse(vArgStream);
		return 1;
	} // fParse
	bool fPrint(tStr vIndentStr = "") const
	{
		std::cout << vIndentStr << "[CmdTab]=(" << std::endl;
		for(auto &rI: this->vCmdTab)
		{
			const tKey &rK = rI.first;
			const tCmd &rC = *rI.second;
			std::cout << vIndentStr << " [" << rK << "]=(" << std::endl;
			rC.fPrint(vIndentStr + "  ");
			std::cout << vIndentStr << " )=[" << rK << "]" << std::endl;
		}
		std::cout << vIndentStr << ")=[CmdTab]" << std::endl;
		std::cout << vIndentStr << "[OptTab]=(" << std::endl;
		for(auto &rI: this->vOptTab)
		{
			const tKey &rK = rI.first;
			const tOpt &rO = *rI.second;
			std::cout << vIndentStr << " [" << rK << "]=(" << std::endl;
			std::cout << vIndentStr << "  " << rO << std::endl;
			std::cout << vIndentStr << " )=[" << rK << "]" << std::endl;
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
	tStr					vHelpText;
	tCallback			   *pCallback;
	std::map<tKey, tCmdPtr> vCmdTab;
	std::map<tKey, tOptPtr> vOptTab;
	tArr					vArgArr;
} tCmd; // command type
using tOptPtr = tCmd::tOptPtr;
using tCmdPtr = tCmd::tCmdPtr;

} // namespace nTermArgs

#endif // dTermArgsHxx
