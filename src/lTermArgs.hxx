#ifndef dTermArgsHxx
#define dTermArgsHxx

#include <algorithm>
#include <cctype>

#include <map>
#include <vector>
#include <deque>
#include <set>

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
using tKey = tStr;
using tPak = std::vector<tArg>;

typedef struct tStream
{
  public: // setters
	void fAppend(const tArg &vArg)
	{
		this->vPak.push_back(vArg);
	}	  // fAppend
  public: // getters
	tArg fPopNext()
	{
		tArg vArg = this->vPak.front();
		this->vPak.pop_front();
		return vArg;
	}	  // fPopNext
  public: // vetters
	bool fVetNext()
	{
		return this->vPak.size();
	}	  // fVetNext
  public: // datadef
	std::deque<tArg> vPak;
} tStream;
typedef struct tOption
{
  public: // datadef
	tPak vPak;
	tArg vDef;
} tOption;
typedef struct tFlag
{
	int vCount = 0;
} tFlag;
typedef class tParser
{
  public: // typedef
	typedef void(tCallback)(const tKey &vCmdKey, tParser &rParser);
  public: // codetor
	tParser(const tStr &vHelpText = "", const tStr &vVnumText = "")
		: vHelpText(vHelpText), vVnumText(vVnumText)
	{
	} // tParser
	~tParser()
	{
		std::set<tParser *> vCmdSet;
		for(auto &r: vCmdTab)
		{
			vCmdSet.insert(r.second);
		}
		for(auto *p: vCmdSet)
		{
			delete p;
		}
		std::set<tOption *> vOptSet;
		for(auto &r: vOptTab)
		{
			vOptSet.insert(r.second);
		}
		for(auto *p: vOptSet)
		{
			delete p;
		}
		std::set<tFlag *> vFlagSet;
		for(auto &r: vFlagTab)
		{
			vFlagSet.insert(r.second);
		}
		for(auto *p: vFlagSet)
		{
			delete p;
		}
	}	  // ~tParser
  public: // getters
	auto fGetCmdKey() const
	{
		return *(&this->vCmdKey);
	} // fGetCmdKey
	auto fGetCmdParser()
	{
		return *this->vCmdTab[this->vCmdKey];
	} // fGetCmdParser
	auto fGetCmdParser(const tKey &rKey)
	{
		return *this->vCmdTab[rKey];
	} // fGetCmdParser
	auto fGetArgCount(const tKey &rKey)
	{
		if(this->vFlagTab.count(rKey))
		{
			return static_cast<unsigned>(this->vFlagTab[rKey]->vCount);
		}
		if(this->vOptTab.count(rKey))
		{
			return static_cast<unsigned>(this->vOptTab[rKey]->vPak.size());
		}
		return 0u;
	} // fGetArgCount
	auto fGetArgArray(const tKey &rKey)
	{
		if(this->vOptTab.count(rKey))
		{
			return this->vOptTab[rKey]->vPak;
		}
		return tPak{};
	} // fGetArgArray
	auto fGetOpt(const tKey &rKey)
	{
		if(this->vOptTab.count(rKey))
		{
			if(this->vOptTab[rKey]->vPak.size())
			{
				return this->vOptTab[rKey]->vPak.back();
			}
			return this->vOptTab[rKey]->vDef;
		}
		return tArg{};
	}	  // fGetOpt
  public: // vetters
	bool fVetCmd() const
	{
		return vCmdKey != "";
	} // fVetCmd
	bool fVetCmd(const tKey &rKey) const
	{
		return vCmdKey == rKey;
	} // fVetCmd
	bool fVetArg(const tKey &rKey)
	{
		if(this->vFlagTab.count(rKey))
		{
			return this->vFlagTab[rKey]->vCount;
		}
		if(this->vOptTab.count(rKey))
		{
			return this->vOptTab[rKey]->vPak.size();
		}
		return 0;
	}	  // fVetArg
  public: // setters
	auto &fRegCmd(
		const tKey &rKey, const tStr &rHelpText = "", tCallback *pCallback = nullptr
	)
	{
		tParser *pParser   = new tParser();
		pParser->vHelpText = rHelpText;
		pParser->pCallback = pCallback;
		tStrStream vStream(rKey);
		tStr	   vAlias;
		while(vStream >> vAlias)
		{
			this->vCmdTab[vAlias] = pParser;
		}
		return *pParser;
	}
	auto &fRegOpt(const tKey &rKey, const tStr &rDef = "")
	{
		tOption *pOption = new tOption();
		pOption->vDef	 = rDef;
		tStrStream vStream(rKey);
		tStr	   vAlias;
		while(vStream >> vAlias)
		{
			this->vOptTab[vAlias] = pOption;
		}
		return *pOption;
	} // fRegOpt
	auto &fRegFlag(const tKey &rKey)
	{
		tFlag	  *pFlag = new tFlag();
		tStrStream vStream(rKey);
		tStr	   vAlias;
		while(vStream >> vAlias)
		{
			this->vFlagTab[vAlias] = pFlag;
		}
		return *pFlag;
	}	   // fRegFlag
  private: // actions
	auto fExitHelp(int vCode = 0)
	{
		std::cout << this->vHelpText << std::endl;
		return exit(vCode);
	} // fExitHelp
	auto fExitVnum(int vCode = 0)
	{
		std::cout << this->vVnumText << std::endl;
		return exit(vCode);
	} // fExitVnum
	bool fParseOptE(const tArg &rPfx, const tKey &rKey, const tArg &rArg)
	{
		if(this->vOptTab.count(rKey))
		{
			if(rArg.size())
			{
				this->vOptTab[rKey]->vPak.push_back(rArg);
			}
			else
			{
				std::cerr
					<< "Error: missing value for " << rPfx << rKey << std::endl;
				this->fExitHelp(1);
				return 0;
			}
		}
		else
		{
			std::cerr
				<< "Error: " << rPfx << rKey << " is not a recognised option"
				<< std::endl;
			this->fExitHelp(1);
			return 0;
		}
		return 1;
	} // fParseOptE
	bool fParseOptL(const tArg &rArg, tStream &rStream)
	{
		size_t vPos = rArg.find("=");
		if(vPos != std::string::npos)
		{
			tKey vKey = rArg.substr(0, vPos);
			tArg vVal = rArg.substr(vPos + 1);
			this->fParseOptE("--", vKey, vVal);
			return 1;
		}
		if(this->vFlagTab.count(rArg))
		{
			this->vFlagTab[rArg]->vCount++;
			return 1;
		}
		if(vOptTab.count(rArg))
		{
			if(rStream.fVetNext())
			{
				this->vOptTab[rArg]->vPak.push_back(rStream.fPopNext());
				return 1;
			}
			else
			{
				std::cerr << "Error: missing argument for --" << rArg << std::endl;
				this->fExitHelp(1);
				return 0;
			}
		}
		if(rArg == "help" && this->vHelpText != "")
		{
			this->fExitHelp(0);
			return 1;
		}
		if(rArg == "vnum" && this->vVnumText != "")
		{
			this->fExitVnum(0);
			return 1;
		}
		std::cerr
			<< "Error: --" << rArg << " is not a recognised flag or option"
			<< std::endl;
		this->fExitHelp(1);
		return 0;
	} // fParseOptL
	bool fParseOptS(const tArg &rArg, tStream &rStream)
	{
		size_t vPos = rArg.find("=");
		if(vPos != tStr::npos)
		{
			tKey vKey = rArg.substr(0, vPos);
			tArg vVal = rArg.substr(vPos + 1);
			return fParseOptE("-", vKey, vVal);
		}
		for(auto &rIter: rArg)
		{
			tKey vKey = tStr(1, rIter);
			if(this->vFlagTab.count(vKey))
			{
				this->vFlagTab[vKey]->vCount++;
				continue;
			}
			if(this->vOptTab.count(vKey))
			{
				if(rStream.fVetNext())
				{
					this->vOptTab[vKey]->vPak.push_back(rStream.fPopNext());
					continue;
				}
				else
				{
					if(rArg.size() > 1)
					{
						std::cerr
							<< "Error: missing argument for '" << rIter
							<< "' in -" << rArg << std::endl;
					}
					else
					{
						std::cerr
							<< "Error: missing argument for -" << rIter
							<< std::endl;
					}
					this->fExitHelp(1);
					return 0;
				}
			}
			if(rIter == 'h' && this->vHelpText != "")
			{
				this->fExitHelp(0);
				return 1;
			}
			if(rIter == 'v' && this->vVnumText != "")
			{
				this->fExitVnum(0);
				return 1;
			}
			if(rArg.size() > 1)
			{
				std::cerr
					<< "Error: '" << rIter << "' in -" << rArg
					<< " is not a recognised flag or option" << std::endl;
			}
			else
			{
				std::cerr
					<< "Error: -" << rIter
					<< " is not a recognised flag or option" << std::endl;
			}
			this->fExitHelp(1);
			return 0;
		}
		return 1;
	} // fParseOptS
	bool fParse(tStream &rStream)
	{
		bool vArgIs1st = 1;
		while(rStream.fVetNext())
		{
			tArg vArg = rStream.fPopNext();
			if(vArg == "--")
			{
				while(rStream.fVetNext())
				{
					this->vArgPak.push_back(rStream.fPopNext());
				}
				continue;
			} // ArgPak
			if(vArg.compare(0, 2, "--") == 0)
			{
				this->fParseOptL(vArg.substr(2), rStream);
				continue;
			} // OptL
			if(vArg[0] == '-')
			{
				if(vArg.size() == 1 || std::isdigit(vArg[1]))
				{
					this->vArgPak.push_back(vArg);
				}
				else
				{
					this->fParseOptS(vArg.substr(1), rStream);
				}
				continue;
			} // OptS
			// registered command
			if(vArgIs1st && this->vCmdTab.count(vArg))
			{
				tParser *pParser = this->vCmdTab[vArg];
				this->vCmdKey	 = vArg;
				pParser->fParse(rStream);
				if(pParser->pCallback)
				{
					pParser->pCallback(vArg, *pParser);
				}
				continue;
			} // Cmd
			if(vArgIs1st && vArg == "help" && this->vCmdTab.size())
			{
				if(rStream.fVetNext())
				{
					tKey vKey = rStream.fPopNext();
					if(this->vCmdTab.find(vKey) == this->vCmdTab.end())
					{
						std::cerr
							<< "Error: '" << vKey
							<< "' is not a recognised command" << std::endl;
						this->fExitHelp(1);
						return 0;
					}
					else
					{
						this->vCmdTab[vKey]->fExitHelp(0);
					}
				}
				else
				{
					std::cerr
						<< "Error: the help command requires an argument"
						<< std::endl;
					this->fExitHelp(1);
					return 0;
				}
			} // help
			this->vArgPak.push_back(vArg);
			vArgIs1st = 0;
		} // while(rStream.fVetNext())
		return 1;
	}	  // fParse
  public: // actions
	bool fParse(int vArgC, const char **vArgV)
	{
		if(vArgC > 1)
		{
			tStream vStream;
			for(int vIter = 1; vIter < vArgC; vIter++)
			{
				vStream.fAppend(vArgV[vIter]);
			}
			this->fParse(vStream);
		}
		return 1;
	} // fParse
	bool fParse(int vArgC, char **vArgV)
	{
		if(vArgC > 1)
		{
			tStream vStream;
			for(int vIter = 1; vIter < vArgC; vIter++)
			{
				vStream.fAppend(vArgV[vIter]);
			}
			this->fParse(vStream);
		}
		return 1;
	} // fParse
	bool fParse(const tPak &rArgPak)
	{
		tStream vStream;
		for(const tArg &rArg: rArgPak)
		{
			vStream.fAppend(rArg);
		}
		this->fParse(vStream);
		return 1;
	} // fParse
	bool fPrint() const
	{
		std::cout << "Options:\n";
		if(this->vOptTab.size())
		{
			for(auto &rIter: this->vOptTab)
			{
				tOption *pOpt = rIter.second;
				std::cout << "  " << rIter.first << ": ";
				std::cout << "(" << pOpt->vDef << ") ";
				std::cout << pOpt->vPak;
				std::cout << std::endl;
			}
		}
		else
		{
			std::cout << "  [none]" << std::endl;
		}
		std::cout << "\nFlags:\n";
		if(this->vFlagTab.size())
		{
			for(auto &rIter: this->vFlagTab)
			{
				std::cout
					<< "  " << rIter.first << ": " << rIter.second->vCount
					<< std::endl;
			}
		}
		else
		{
			std::cout << "  [none]\n";
		}
		std::cout << "\nArguments:\n";
		if(this->vArgPak.size())
		{
			for(auto &rArg: this->vArgPak)
			{
				std::cout << "  " << rArg << std::endl;
			}
		}
		else
		{
			std::cout << "  [none]" << std::endl;
		}
		std::cout << "\nCommand:\n";
		if(this->fVetCmd())
		{
			std::cout << "  " << this->vCmdKey << std::endl;
		}
		else
		{
			std::cout << "  [none]" << std::endl;
		}
		return 1;
	}	  // fPrint
  public: // datadef
	tPak	   vArgPak;
	tStr	   vHelpText;
	tStr	   vVnumText;
	tCallback *pCallback;
  private: // datadef
	tKey					  vCmdKey;
	std::map<tKey, tParser *> vCmdTab;
	std::map<tKey, tOption *> vOptTab;
	std::map<tKey, tFlag *>	  vFlagTab;
} tParser;

} // namespace nTermArgs

#endif // dTermArgsHxx
