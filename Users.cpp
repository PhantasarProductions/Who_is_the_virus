// Lic:
// Who is the Virus?
// Users
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
// Version: 22.06.13
// EndLic
#include "Users.hpp"
#include <Dirry.hpp>
#include <QCol.hpp>

using namespace TrickyUnits;
using namespace std;

namespace Virus {

	GINIE User::_Data{};

	string User::GFile() { return Dirry("$AppSupport$/WhoIsTheVirus_Users.ini"); }
	shUser User::Current{nullptr};
	bool User::Loaded{ false };

	shUser User::GetCurrent() { return Current; }

	std::string User::Password() {
		return _Data.Value(Name,"Password");
	}

	User::User(std::string n, std::string pw) {
		Name = n;
		if (!Loaded) {
			QCol->Doing("Loading", GFile());
			_Data.FromFile(GFile(),true);
			_Data.AutoSave = GFile();
		}
		if (!_Data.HasGroup(n)) {
			QCol->Doing("New user", n);
			_Data.Value(n, "Password", pw);
		}
		Okay = pw == Password();
	}
	shUser User::Login(int c, char** args) {
		shUser Ret{ nullptr };
		switch (c) {
		case 1:
			do {
				string u, p;
				QCol->Doing("Username", "", "");
				cin >> u;
				QCol->Doing("Password", "", "");
				cin >> p;
				Ret = Login(u, p);
			} while (!Ret->Okay);
			return Ret;
		case 2:
		{
			string p;
			QCol->Doing("Username", args[1]);
			QCol->Doing("Password", "", "");
			return Login(args[1], p);
		}
		break;
		default:
			QCol->Doing("Username", args[1]);
			QCol->Doing("Password", args[2]);
			return Login(args[1], args[2]);
		}
	}
	shUser User::Login(std::string u, std::string p) {
		Current = make_shared<User>(u, p);
		return Current;
	}
}