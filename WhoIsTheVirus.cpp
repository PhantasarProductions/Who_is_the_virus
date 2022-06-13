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
// Version: 22.06.13
// EndLic
#include <QCol.hpp>
#include <Platform.hpp>
#include "bdate.hpp"
#include "NameClass.hpp"

using namespace std;
using namespace Virus;
using namespace TrickyUnits;

int main(int c, char** args) {
	//QCol = &ANSI; // Test purposes only!
	QCol->Write(qColor::Yellow, "Who is the virus?\n");
	QCol->Write(qColor::LightBlue, "Coded by: Tricky\n");
	QCol->Write(qColor::LightMagenta, "Build date: " + string(BuildDate) + "\n");
	QCol->Doing("Platform", Platform());
	QCol->Doing("ColorDrv", QCol->Name);
	QCol->Write(qColor::White, "(c) Jeroen P. Broks, released under the terms of the GPL3\n\n");
	QCol->Reset();
	NameClass::NewAll();

}