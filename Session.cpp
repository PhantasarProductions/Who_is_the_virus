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
#include <QCol.hpp>
#include <TRandom.hpp>
#include <Dirry.hpp>
#include <FileList.hpp>
#include <TrickySTOI.hpp>
#include <QuickString.hpp>
#include "Session.hpp"
#include "NameClass.hpp"
#include "Users.hpp"

#define DEBUGMODE

using namespace std;
using namespace TrickyUnits;

namespace Virus {

	string GetHint(Session Ses, File* VFile, File* Check); // Only header in "hints.cpp", so I was NOT willing to make a separate header file for that.


	void TSession::Dir(std::vector<std::string> p) {
		//cout << "DIR!\n"; // debug only!
		for (auto a : Files) {
			auto key{ a.first };
			auto val{ a.second };
			bool LetsSee{ true };
			//cout << "Dir: " << key << endl; // debug only;
			//cout << "Parameters: " << p.size() << "\n"; // debug only
			for (uint32 i = 1; i < p.size(); ++i) {
				//cout << "DEBUG> Checking " << i << "/" << p.size() - 1 << " -> " << p[i] << endl; // debug only!
				if (p[i] == "BOYS") LetsSee = LetsSee && val.Stuff.Boy();
				else if (p[i] == "GIRLS") LetsSee = LetsSee && val.Stuff.Girl();
				else if (p[i][0] == '*') LetsSee = LetsSee && suffixed(key, TrickyUnits::right(p[i], p[i].size() - 1));
				else if (p[i][p[i].size() - 1] == '*') LetsSee = LetsSee && prefixed(key, TrickyUnits::left(p[i], p[i].size() - 1));
#ifdef DEBUGMODE
				else if (p[1] == "VIRUS") LetsSee = LetsSee && val.IsVirus;
#endif
				else {
					size_t ws{ (size_t)ToInt(TrickyUnits::right(p[i],p[i].size() - 1)) };
					size_t ps{ key.size() };
					switch (p[i][0]) {
					case '%':
					case '=':
						LetsSee = LetsSee && (ws == ps);
						break;
					case '>':
						LetsSee = LetsSee && (ps > ws);
						break;
					case '<':
						LetsSee = LetsSee && (ps < ws);
						break;
					default:
						QCol->Error("I do not understand parameter " + p[i]);
						return;
					}
				}
			}
			if (LetsSee) {
				QCol->Color(qColor::Magenta, qColor::Black);
				printf("%3d ", key.size());
				if (val.Stuff.Boy()) QCol->Color(qColor::LightCyan, qColor::Black); else QCol->Color(qColor::Pink, qColor::Black);
				cout << left(val.Stuff.Name() + "                              ", 20);
				if (val.Seen)
					QCol->Write(qColor::Yellow, val.Aanwijzing);
				cout << endl;
			}

		}
		cout << endl;
	}

	uint32 TSession::Score() { return FilesTried + (FilesDeleted * 10); }

	Session TSession::Create(uint32 Files) {
		if (Files < 25) { QCol->Error("A session must have at least 25 files"); return nullptr; }
		if (Files > NameClass::Names()) { QCol->Error("There are only " + to_string(NameClass::Names()) + " names registered in this game. So you cannot make a session with " + to_string(Files) + " files"); return nullptr; }
		auto ret = make_shared<TSession>();
		ret->SessionUser = User::GetCurrent();
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
				} while (ret->Files.count(Name.CName()));
				File F;
				F.IsVirus = false;
				F.Aanwijzing = "No hint yet. (If you see this message, please report it!)";
				F.ShowName = Name.Name();
				F.VUP = Name.CName();
				F.Stuff = Name;
				ret->Files[Name.CName()] = F;
			}

			CountDown = 100000;
			for (auto FL : (ret->Files)) {
				string NF{};
				do {
					auto value{ FL.second };
					Dupes = false;
					if ((--CountDown) == 0) { QCol->Error("Creating a new session failed (hint timeout)"); return nullptr; }
					NF = GetHint(ret, &VirusFile, &value);
					for (auto FL2 : ret->Files) Dupes = Dupes || FL2.second.Aanwijzing == NF;
				} while (Dupes || (!NF.size()));
				ret->Files[FL.first].Aanwijzing = NF;
			}
		}
		return ret;
	}
	std::string TSession::SesFile(std::string User) { return Dirry("$AppSupport$/Session_" + User + "WITV_Session"); }
	std::string TSession::SesFile() { return SessionUser->MyName(); }
	bool TSession::HasSession(std::string User) { return IsFile(SesFile(User)); }
	bool TSession::HasSession() { return HasSession(SessionUser->MyName()); }
	Session TSession::Load(std::string User) {
		QCol->Error("Loading sessions not yet implemented");
		return nullptr;
	}

	void TSession::Save() {
		QCol->Error("Saving sessions not yet implemented");
	}



	void TSession::Run() {
		uint16 Score{ 0 };
		uint16 Deleted{ 0 };
		uint16 FilesWatched{ 0 };
		QCol->Write(qColor::Yellow, "Type \"");
		QCol->Write(qColor::Green, "HELP");
		QCol->Write(qColor::Yellow, "\" for help.\n\n");
		do {
			std::string cmd;
			do {
				QCol->Color(qColor::Magenta, qColor::Black);
				printf("%04d>", Score);
				QCol->Color(qColor::LightMagenta, qColor::Black);
				std::getline(std::cin, cmd); //cin >> cmd; 
				cout << endl;
				cmd = Upper(Trim(cmd));
			} while (!cmd.size());
			auto s = Split(cmd, ' ');
			if (s[0] == "DIR")
				Dir(s);
			else if (s[0] == "DEL") {
				if (s.size() < 2)
					QCol->Error("Syntax error in deletion command");
				else if (!Files.count(s[1]))
					QCol->Error("There is no file named '" + s[1] + "'");
				else if (Files[s[1]].IsVirus) {
					QCol->Doing("Deleting", s[1]);
					QCol->Doing("Is virus", "Yes!");
					QCol->Write(qColor::Yellow, "\n\nCONGRATULATIONS!! YOU KILLED THE VIRUS\n\n");
					SessionUser->Sessions(1);
					SessionUser->Score(Score);
					SessionUser->Success(1);
					QCol->Doing("Files tried (x1)", FilesWatched);
					QCol->Doing("Files deleted (x10)", Deleted);
					QCol->Doing("Score", Score);
					QCol->Color(qColor::White, qColor::Magenta);
					cout << "=========================================================================";
					QCol->Color(qColor::White, qColor::Black);
					cout << endl;
					QCol->Doing("Sessions", SessionUser->Sessions());
					QCol->Doing("Suceeded", SessionUser->Succeed());
					QCol->Doing("Failed", SessionUser->Failed());
					QCol->Doing("Forfeit", SessionUser->Forfeit());
					QCol->Doing("Total Score", SessionUser->Score());
					QCol->Doing("Average", SessionUser->Average());
					return;
				} else {
					Files.erase(s[1]);
					QCol->Doing("Deleting", s[1]);
					QCol->Doing("Is virus", "No!");
					QCol->Write(qColor::Red, "\nToo bad! That was NOT the virus!\n");
					Score += 100;
					Deleted++;
				}
			} else if (Files.count(s[0])) {
				if (Files[s[0]].IsVirus) {
					QCol->Color(qColor::Red, qColor::Black);
					cout << "I AM THE VIRUS!!!!\n\nYOUR ENTIRE SYSTEM HAS BEEN DELETED\n\nGAME OVER!!\n\n";
					Score += 100;
					SessionUser->Sessions(1);
					SessionUser->Score(Score);
					SessionUser->Failed(1);
					return;
				}
				if (!Files[s[0]].Seen) {
					Files[s[0]].Seen = true;
					Score++;
					FilesWatched++;
				}
				QCol->Write(qColor::Yellow, Files[s[0]].Aanwijzing);
				cout << endl;
			} else {
				QCol->Error("Bad command or file name");
			}
		} while (true);
	}
}