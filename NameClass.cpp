// Lic:
// Who is the Virus
// Name Class
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
#undef NC_Debug

#include <algorithm>

#include "NameClass.hpp"
#include "Names.hpp"
#include <QuickString.hpp>

#ifdef NC_Debug
#include <iostream>
#define Chat(a) std::cout << "NAME-DEBUG:>>" << a <<std::endl
#else
#define Chat(a)
#endif

using namespace TrickyUnits;

namespace Virus {
	std::map<std::string, NameClass> NameClass::Register{};
	std::vector<std::string> NameClass::Lijst{};

	void NameClass::NewBoy(std::string n) { NewCh(n, Gender::Boy); Chat("Boy added: " << n); }
	void NameClass::NewGirl(std::string n) { NewCh(n, Gender::Girl); Chat("Girl added: " << n); }

	void NameClass::NewCh(std::string n, Gender g) {
		NameClass r;
		r._name = n;
		r._gender = g;
		Register[r.CName()] = r;
		Lijst.push_back(r.CName());
	}

	static bool compareFunction(std::string a, std::string b) { return a < b; }

	void NameClass::NewAll() {
		for (auto boy : boys) NewBoy(boy);
		for (auto girl : girls) NewGirl(girl);
		std::sort(Lijst.begin(), Lijst.end(), compareFunction);
	}

	std::string NameClass::Name() { return _name; }
	std::string NameClass::CName() { return Upper(_name); }

	int NameClass::Letters() { return _name.size(); }
	bool NameClass::Even() { return Letters() % 2 == 0; }
	bool NameClass::Odd() { return Letters() % 2 != 0; }

}