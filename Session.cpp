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
// Version: 22.06.16
// EndLic
#include <string>
#include <QCol.hpp>
#include <TRandom.hpp>
#include <Dirry.hpp>
#include <FileList.hpp>
#include <TrickySTOI.hpp>
#include <QuickString.hpp>
#include <Platform.hpp>
#include <QuickStream.hpp>
#include <OpenURL.hpp>
#include "Session.hpp"
#include "NameClass.hpp"
#include "Users.hpp"
#include "Ach.hpp"

#undef DEBUGMODE

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

	//uint32 TSession::Score() { return FilesTried + (FilesDeleted * 10); }

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
	std::string TSession::SesFile(std::string User) { return Dirry("$AppSupport$/Session_" + User + ".WITV_Session"); }
	std::string TSession::SesFile() { return SesFile(SessionUser->MyName()); }
	bool TSession::HasSession(std::string User) { return IsFile(SesFile(User)); }
	bool TSession::HasSession() { return HasSession(SessionUser->MyName()); }
	Session TSession::Load(std::string User) {
#ifdef DEBUGMODE
		QCol->Doing("Loading", SesFile(User));

#else
		QCol->Doing("Loading", "Session");
#endif
		auto bt = ReadFile(SesFile(User));
#ifdef DEBUGMODE
		cout << "Session file size: " << bt->Size() << endl;
#endif
		{
			byte fuckyou;
			do {
				fuckyou = bt->ReadByte();
#ifdef DEBUGMODE
				cout << fuckyou;
#endif // DEBUGMODE

				if (bt->EndOfFile()) { QCol->Error("Invalid file!"); return nullptr; }
			} while (fuckyou != 0x1a);
		}
#ifdef DEBUGMODE
		cout << "\t Check!\n";
#endif // DEBUGMODE
		if (User::GetCurrent()->MyName() != User) { QCol->Error("User mismatch! Expect some trouble"); }
		auto ret = make_shared<TSession>();
		ret->SessionUser = User::GetCurrent();
		map<uint64, string> ID{};
		uint64 useID{ 0 };
		string useName = "";
		do {
			auto tag = bt->ReadByte();
#ifdef DEBUGMODE
			cout << "Loading!\t tag=" << (int)tag << endl;
#endif
			if (!tag) break;
			switch (tag) {
			case 1: {
				auto boy = (bool)bt->ReadByte();
				auto Name = bt->ReadString();
				auto IDid = bt->ReadUInt64();
#ifdef DEBUGMODE
				cout << "Adding name: " << Name << " (" << IDid << ")\n";
#endif
				ID[IDid] = Upper(Name);
				auto N = NameClass::GetName(Name);
				File F;
				F.IsVirus = false;
				F.Aanwijzing = "No hint yet. (If you see this message, please report it!)";
				F.ShowName = N.Name();
				F.VUP = N.CName();
				F.Stuff = N;
				ret->Files[N.CName()] = F;
			}
				  break;
			case 2:
				useID = bt->ReadUInt64();
				if (!ID.count(useID)) {
					QCol->Error("Unknown data ID (" + to_string(useID) + ") in session file. File may be corrupted!");
				}
				useName = ID[useID];
				break;
			case 3:
				ret->Files[useName].Aanwijzing = bt->ReadString();
				break;
			case 4:
				ret->Files[useName].IsVirus = (bool)bt->ReadByte();
				break;
			case 5:
				ret->Files[useName].Seen = (bool)bt->ReadByte();
				break;
			case 6:
				ret->Score = (int16)bt->ReadInt();
				break;
			case 7:
				ret->Deleted = (int16)bt->ReadInt();
				break;
			case 8:
				ret->FilesWatched = (int16)bt->ReadInt();
				break;
			case 9:
			{
				auto u = bt->ReadString();
				if (u != ret->SessionUser->MyName()) {
					QCol->Error("This session file isn't yours. It belongs to " + u + ". Trying to cheat?");
					return nullptr;
				}
			} break;
			default:
				QCol->Error("Illegal session tag (" + to_string(tag) + ")! Was this session made with a later version of the game, or is your session file corrupted?");
				return nullptr;
			}
		} while (!bt->EndOfFile());
		bt->Close();
		if (!FileDelete(SesFile(User))) {
			QCol->Error("Sorry! No go if this didn't go right!");
			return nullptr;
		}
		QCol->Green("Success!\n");
		//QCol->Error("Loading sessions not yet implemented");
		return ret;
	}

	void TSession::Save() {
#ifdef DEBUGMODE
		QCol->Doing("Saving", SesFile());
#else
		QCol->Doing("Saving", "Session");
#endif
		auto bt = WriteFile(SesFile());
		bt->Write("Who is the Virus?\x1a", true);
		map<uint64, File> tmp;
		for (auto f : Files) {
			uint64 r; do { r = abs(TRand(65535)); } while (tmp.count(r));
#ifdef DEBUGMODE
			cout << "Name \"" << f.second.Stuff.Name() << "\" assinged to ID#" << r << endl;
#endif
			tmp[r] = f.second;
			bt->Write((byte)1);
			bt->Write((byte)f.second.Stuff.Boy());
			bt->Write(f.second.Stuff.Name());
			bt->Write(r);
		}
		for (auto f : tmp) {
			bt->Write((byte)2);
			bt->Write(f.first);
			bt->Write((byte)3);
			bt->Write(f.second.Aanwijzing);
			bt->Write((byte)4);
			bt->Write((byte)f.second.IsVirus);
			bt->Write((byte)5);
			bt->Write((byte)f.second.Seen);
		}
		bt->Write((byte)6); bt->Write((int)Score);
		bt->Write((byte)7); bt->Write((int)Deleted);
		bt->Write((byte)8); bt->Write((int)FilesWatched);
		bt->Write((byte)9); bt->Write(SessionUser->MyName());
		bt->Write((byte)0);
		bt->Close();
		//QCol->Error("Saving sessions not yet implemented");
	}



	void TSession::Run() {
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
					Award("VICTOR");
					if (Score <= 10) Award("ILOVEYOU");
					if (Score <= 30) Award("DATACRIME1");
					if (Score <= 60) Award("DATACRIME2");
					if (Score <= 120) Award("TAIPAN");
					return;
				} else {
					Files.erase(s[1]);
					QCol->Doing("Deleting", s[1]);
					QCol->Doing("Is virus", "No!");
					QCol->Write(qColor::Red, "\nToo bad! That was NOT the virus!\n");
					Score += 10;
					Deleted++;
				}
			} else if (s[0] == "FORFEIT") {
				QCol->Color(qColor::Red, qColor::Black);
				cout << "Session forfeit\n\n";
				Score += 100;
				SessionUser->Sessions(1);
				SessionUser->Score(Score);
				SessionUser->Forfeit(1);
				Award("DOODLE");
				return;
			} else if (s[0] == "CLS") {
#ifdef Tricky_Windows
				system("cls");
#else
				system("clear");
#endif
				// Is there really not a BETTER method to do this?
			} else if (s[0] == "EXIT" || s[0] == "BYE") {
				Save();
				QCol->Doing("Logging out", "Goodbye");
				QCol->Reset();
				exit(0);
			} else if (s[0] == "HELP")
				OpenURL("https://github.com/PhantasarProductions/Who_is_the_virus/wiki");
			else if (s[0] == "ACH")
				Overzicht();
			else if (Files.count(s[0])) {
				if (Files[s[0]].IsVirus) {
					QCol->Color(qColor::Red, qColor::Black);
					cout << "I AM THE VIRUS!!!!\n\nYOUR ENTIRE SYSTEM HAS BEEN DELETED\n\nGAME OVER!!\n\n";
					Score += 100;
					SessionUser->Sessions(1);
					SessionUser->Score(Score);
					SessionUser->Failed(1);
					Award("MICHELANGELO");
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