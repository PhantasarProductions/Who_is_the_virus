// Lic:
// Who is the Virus?
// Name Class (header)
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
#pragma once
#include <map>
#include <vector>
#include <string>

namespace Virus {

	class NameClass;

	enum class Gender {
		Unknown=0,
		Man=1,
		Male=1,
		Boy=1,
		Woman=2,
		Female=2,
		Girl=2
	};

	class NameClass {
		static std::map<std::string, NameClass> Register;
		static std::vector<std::string> Lijst;
		std::string _name{};
		Gender _gender{Gender::Unknown};
	public:
		static void NewBoy(std::string n);
		static void NewGirl(std::string n);
		static void NewCh(std::string n, Gender g);
		static void NewAll();
		static size_t Names();
		static NameClass RandomName();
		std::string Name();
		std::string CName();
		bool Boy();
		bool Girl();
		int Letters();
		bool Even();
		bool Odd();
	};

}