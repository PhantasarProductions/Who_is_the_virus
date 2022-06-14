// Lic:
// Who is the Virus?
// Session
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
// Version: 22.06.14
// EndLic
#include <string>
#include "Session.hpp"
#include "NameClass.hpp"
#include <QCol.hpp>
#include <TRandom.hpp>

using namespace std;
using namespace TrickyUnits;

namespace Virus {

	string GetHint(Session* Ses, File* VFile); // Only header in "hints.cpp", so I was NOT willing to make a separate header file for that.


	uint32 TSession::Score() { return FilesTried + (FilesDeleted * 10); }

	Session TSession::Create(uint32 Files) {
		if (Files < 25) { QCol->Error("A session must have at least 25 files"); return nullptr; }
		if (Files > NameClass::Names()) { QCol->Error("There are only " + to_string(NameClass::Names()) + " names registered in this game. So you cannot make a session with " + to_string(Files) + " files"); return nullptr; }
		auto ret = make_shared<TSession>();
		File VirusFile;		
		{
			auto VName = NameClass::RandomName();
			VirusFile.ShowName = VName.Name();
			VirusFile.Aanwijzing = "***VIRUS***";
			VirusFile.IsVirus = true;
			VirusFile.VUP = VName.CName();
			VirusFile.Stuff = VName;
			ret->Files[VirusFile.VUP] = VirusFile;
		}
		for (uint32 i = 0; i < Files; ++i) {
			uint32 CountDown = 100000;
			bool Dupes{ true };
			{
				NameClass Name;
				do {
					if ((--CountDown) == 0) { QCol->Error("Creating a new session failed (name timeout)"); return nullptr; }
					Name = NameClass::RandomName();
				} while (this->Files.count(Name.CName()));
				File F;
				F.IsVirus = false;
				F.Aanwijzing = "No hint yet. (If you see this message, please report it!";
				F.ShowName = Name.Name();
				F.VUP = Name.CName();
				F.Stuff = Name;
				this->Files[Name.CName()] = F;
			}
			
			CountDown = 100000;
			do {
				Dupes = true;
				if ((--CountDown) == 0) { QCol->Error("Creating a new session failed (hint timeout)"); return nullptr; }

				

			} while (Dupes);
		}
		return ret;
	}
}