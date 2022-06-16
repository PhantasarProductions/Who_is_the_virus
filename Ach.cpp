// Lic:
// Who is the Virus?
// Achievements
// 
// 
// 
// (c) Jeroen P. Broks, 2022
// 
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
// 
// Please note that some references to data like pictures or audio, do not automatically
// fall under this licenses. Mostly this is noted in the respective files.
// 
// Version: 22.06.16
// EndLic

#include <map>
#include <QCol.hpp>
#include <Medals.hpp>
#include <TrickyTime.hpp>
#include <jcr6_core.hpp>
#include "Ach.hpp"
#include "Users.hpp"

using namespace std;
using namespace TrickyUnits;

namespace Virus {
	struct TAch{
		string Name;
		string Description;
		bool hide{ false };
	};

	
	static map<string, TAch> AchMap;
	
	static void Init() {
		static bool Done{ false };
		if (Done) return;
		jcr6::init_JCR6();
		TrickyMedals::LoadFromInternet();

		/*
Rec: MICHELANGELO
Rec: ILOVEYOU
Rec: DATACRIME1
Rec: DATACRIME2
Rec: TAIPAN
Rec: DOODLE
Rec: VICTOR

		*/
		AchMap["MICHELANGELO"] = TAch{ "Michelangelo", "Have the virus destroy your system", false };
		AchMap["ILOVEYOU"] = TAch{ "iloveyou","Delete the virus with less than 10 points",false };
		AchMap["DATACRIME1"] = TAch{ "DataCrime I","Delete the virus with less than 30 points",false };
		AchMap["DATACRIME2"] = TAch{ "DataCrime II","Delete the virus with less than 60 points",false };
		AchMap["TAIPAN"] = TAch{ "Whisper.Taipan","Delete the virus with less than 120 points",false };
		AchMap["DOODLE"] = TAch{ "Yankee Doodle", "Forfeit a session", false };
		AchMap["VICTOR"] = TAch{ "Victor","Delete the virus",false };

	}


	void Overzicht() {
		Init();
		for (auto a : AchMap) {
			auto have{ User::GetCurrent()->Award(a.first) };
			if (have) {
				QCol->LGreen(a.second.Name + "\n");
				QCol->Green(a.second.Description + "\n");
				QCol->Dark("Obtained: " + User::GetCurrent()->Awarded(a.first) + "\n\n");
			} else if (!a.second.hide) {
				QCol->Pink(a.second.Name + "\n");
				QCol->Red(a.second.Description + "\n\n\n");
			}
		}
	}

	void Award(std::string tag) {
		Init();
		auto cu{ User::GetCurrent() };
		if (cu->Award(tag)) return;
		cu->Award(tag, CurrentDate() + "; " + CurrentTime());
		TrickyMedals::Award("WHOISTHEVIRUS", tag.c_str());
		QCol->Yellow("Achievement obtained: ");
		QCol->LGreen(AchMap[tag].Name);
		QCol->Reset();
		cout << endl;
	}


}