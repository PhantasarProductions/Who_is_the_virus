// Lic:
// Who is the Virus?
// Session (header)
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
#include <string>
#include <map>
#include <memory>
#include <QuickTypes.hpp>
#include "NameClass.hpp"
#include "Users.hpp"

namespace Virus {

	struct File {
		std::string VUP{};
		std::string ShowName{};
		std::string Aanwijzing{};
		NameClass Stuff{};
		bool Seen{ false };
		bool IsVirus{ false };
	};

	class TSession; typedef std::shared_ptr<TSession> Session;

	class TSession {
		std::shared_ptr<User> SessionUser{ nullptr };
		void Dir(std::vector<std::string> p);
		TrickyUnits::uint16 Score{ 0 };
		TrickyUnits::uint16 Deleted{ 0 };
		TrickyUnits::uint16 FilesWatched{ 0 };

	public:
		std::map<std::string, File> Files{};
		//TrickyUnits::uint32 FilesTried{ 0 };
		//TrickyUnits::uint32 FilesDeleted{ 0 };
		//TrickyUnits::uint32 Score();
		static Session Create(TrickyUnits::uint32 Files = 100);
		static std::string SesFile(std::string User);
		std::string SesFile();
		static bool HasSession(std::string User);
		bool HasSession();
		static Session Load(std::string User);
		void Save();
		void Run();
	};


}