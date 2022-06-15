// Lic:
// Who is the Virus?
// Users (header)
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
// Version: 22.06.15
// EndLic
#pragma once
#include <GINIE.hpp>

namespace Virus {
	class User; typedef std::shared_ptr<User> shUser;

	class User {
	private:
		static TrickyUnits::GINIE _Data;
		static bool Loaded;
		static std::string GFile();
		static shUser Current;
		std::string Name{ "" };	
		std::string SetHash();
		void UHash();
	public:
		static shUser GetCurrent();
		bool Okay{ false };
		std::string Password();
		User(std::string n, std:: string pw);
		int Sessions();
		void Sessions(int nv);
		int Succeed();
		void Success(int nv);
		int Failed();
		void Failed(int nv);
		int Score();
		void Score(int nv);
		int Forfeit();
		void Forfeit(int nv);
		int Average();
		std::string MyName();
		void Award(std::string a,std::string v);
		bool Award(std::string a);
		std::string Awarded(std::string a);




		static shUser Login(int c, char** args);
		static shUser Login(std::string u, std::string p);
	};
}