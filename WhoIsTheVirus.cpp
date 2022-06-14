// Lic:
// Who is the Virus?
// Main file
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

#include <time.h>
#include <QCol.hpp>
#include <Platform.hpp>
#include <TRandom.hpp>
#include "bdate.hpp"
#include "NameClass.hpp"
#include "Session.hpp"
#include "Users.hpp"

using namespace std;
using namespace Virus;
using namespace TrickyUnits;

bool AnotherSession() {
	QCol->Write(qColor::Yellow, "Play another session?");
	QCol->Write(qColor::Magenta, " <Y/N> ");
	QCol->Color(qColor::Cyan,qColor::Black);	
	string Antwoord;
	cin >> Antwoord;
	return Antwoord.size() && (Antwoord[0] == 'y' || Antwoord[0] == 'Y');	
}

int main(int c, char** args) {
	//QCol = &ANSI; // Test purposes only!
	QCol->Write(qColor::Yellow, "Who is the virus?\n");
	QCol->Write(qColor::LightBlue, "Coded by: Tricky\n");
	QCol->Write(qColor::LightMagenta, "Build date: " + string(BuildDate) + "\n");
	QCol->Doing("Platform", Platform());
	QCol->Doing("ColorDrv", QCol->Name);	
	QCol->Write(qColor::White, "(c) Jeroen P. Broks, released under the terms of the GPL3\n\n");
	NameClass::NewAll();
	TRandSeed(time(NULL));
	//auto r = TRand(20); cout << "R<" << r << ">\n";
	if (User::Login(c, args)->Okay) {
		do {
			auto un{ User::GetCurrent()->MyName() };
			Session Ses{ nullptr };
			if (TSession::HasSession(un)) {
				QCol->Doing("Restoring", "Session");
				Ses = TSession::Load(un);
			} else {
				QCol->Doing("Creating", "Session");
				Ses = TSession::Create();
			}
			if (Ses) Ses->Run();

		} while (AnotherSession());
	} else {
		QCol->Error("Login failed");
	}
	QCol->Reset();


}