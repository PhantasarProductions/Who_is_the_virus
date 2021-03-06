// Lic:
// Who is the Virus?
// Hints
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
#include <string>
#include <vector>
#include <TRandom.hpp>
#include "Session.hpp"
#include "NameClass.hpp"

#undef HINTDEBUG


#ifdef HINTDEBUG
#define Chat(a) std::cout<<"HINTDEBUG!\t"<<a<<std::endl;
#else
#define Chat(a)
#endif

using namespace std;
using namespace TrickyUnits;

namespace Virus {

	typedef string(*FHint)(Session, File*, File*); // Session, Virus, File being compared

	static bool Vowel(char c) {
		switch (c) {
		case 'a':
		case 'A':
		case 'e':
		case 'E':
		case 'i':
		case 'I':
		case 'o':
		case 'O':
		case 'U':
		case 'u':
			return true;
		default:
			return false;
		}
	}

	static bool Consonant(char c) {
		// The game considers Y as neither a vowel nor a consonant.
		return c != 'y' && c != 'Y' && (!Vowel(c));
	}

	static string UselessHint1(Session S, File* F, File* Chk) { return "I am the virus!"; }
	static string UselessHint2(Session S, File* F, File* Chk) { return "Oh my god! They killed the virus! You bastards!"; }
	static string UselessHint3(Session S, File* F, File* Chk) { return "Mark Rutte is one of the best prime ministers the Netherlands ever had. Very trustworthy. Only a complete moron would hate him out of pure stupidity!"; }
	static string FirstLetter1(Session S, File* F, File* Chk) {
		auto l{ (char)TRand(65,90) };
		auto f{ F->VUP[0] };
		if (l != f) {
			//return string("The virus starts with the letter " + l);
			char ret[255];
			sprintf_s(ret, "The virus starts with the letter '%c'", l);
			return ret;
		}
		return "";
	}
	static string FirstLetter2(Session S, File* F, File* Chk) { 
		char ret[200];
		sprintf_s(ret, "The virus does not start with the letter '%c'", F->VUP[0]);
		//return string("The virus does not start with the letter " + F->VUP[0]); 
		return ret;
	}
	static string FirstLetter3(Session S, File* F, File* Chk) {
		if (Vowel(F->VUP[0]))
			return "The first letter of the virus is not a vowel";
		else
			return "The first letter of the virus is a vowel";
	}
	static string FirstLetter4(Session S, File* F, File* Chk) {
		if (Consonant(F->VUP[0]))
			return "The first letter of the virus is not a consonant";
		else
			return "The first letter of the virus is a consonant";
	}

	static string LastLetter1(Session S, File* F, File* Chk) {
		auto l{ (char)TRand(65,90) };
		auto f{ F->VUP[F->VUP.size() - 1] };
		if (l != f) {
			//return string("The virus ends with the letter " + l);
			char ret[255];
			sprintf_s(ret, "The virus ends with the letter '%c'", l);
			return ret;
		}
		return "";
	}
	static string LastLetter2(Session S, File* F, File* Chk) { 
		char ret[255];
		sprintf_s(ret,"The virus does not end with the letter '%c'" , F->VUP[F->VUP.size() - 1]); 
		return ret;
	}
	static string LastLetter3(Session S, File* F, File* Chk) {
		if (Vowel(F->VUP[F->VUP.size() - 1]))
			return "The last letter of the virus is not a vowel";
		else
			return "The last letter of the virus is a vowel";
	}
	static string LastLetter4(Session S, File* F, File* Chk) {
		if (Consonant(F->VUP[F->VUP.size() - 1]))
			return "The last letter of the virus is not a consonant";
		else
			return "The last letter of the virus is a consonant";
	}
	static string FirstAndLast(Session S, File* F, File* Chk) {
		if (F->VUP[0] == F->VUP[F->VUP.size() - 1])
			return "The first letter of my virus is not the same as the last letter of the virus";
		else
			return "The first letter of the virus is the same as the last letter of the virus";
	}
	static string Num(Session S, File* F, File* Chk) {
		uint32 m{ 0 };
		for (auto FL : S->Files) m = max<uint32>(m, FL.second.VUP.size());
		if (m < 3) return "";
		uint32 n{ (uint32)TRand(3, m) };
		char r[200];
		if (F->VUP.size() == n)
			sprintf_s(r, "The virus does not contain %d letters", n);
		else
			sprintf_s(r, "The virus contains %d letters", n);
		return r;
	}
	static string Num2(Session S, File* F, File* Chk) {
		uint32 m{ 0 };
		for (auto FL : S->Files) m = max<uint32>(m, FL.second.VUP.size());
		if (m < 3) return "";
		uint32 n{ (uint32)TRand(3, m) };
		char r[200];
		if (F->VUP.size() >= n)
			sprintf_s(r, "The virus contains less than %d letters", n);
		else if (F->VUP.size() <= n)
			sprintf_s(r, "The virus contains more than %d letters", n);
		else
			return "";
		return r;
	}
	static string Even(Session S, File* F, File* Chk) {
		if (F->VUP.size() % 2 == 0)
			return "The number of letters in the name of the virus is odd";
		else
			return "The number of letters in the name of the virus is even";
	}
	static string Same(Session S, File* F, File* Chk) {
		bool fsame{ false };
		for (uint32 i1 = 0; i1 < F->VUP.size(); i1++)
			for (uint32 i2 = 0; i2 < F->VUP.size(); i2++)
				if (i1 != i2)
					fsame = fsame || (F->VUP[i1] == F->VUP[i2]);
		if (fsame)
			return "None of the letters in the virus can be found multiple times in it";
		else
			return "One or more of the letters in the virus can be found multiple times in it";
	}

	static string SameAsVirus1(Session S, File* F, File* Chk) {
		if (F->VUP[0] == Chk->VUP[0])
			return "I do not have the same starting letter as the virus does";
		else
			return "I have the same starting letter as the virus does";
	}
	static string SameAsVirus2(Session S, File* F, File* Chk) {
		if (F->VUP[F->VUP.size()-1] == Chk->VUP[Chk->VUP.size()-1])
			return "I do not have the same ending letter as the virus does";
		else
			return "I have the same end letter as the virus does";
	}
	static string SameAsVirus3(Session S, File* F, File* Chk) {
		if (F->VUP.size() == Chk->VUP.size())
			return "I don't have the same number of letters in my name as the virus does";
		else
			return "I have the same number of letters in my name as the virus does";
	}
	static string Jeroen(Session S, File* F, File* Chk) {
		if (F->VUP == "JEROEN") return "";
			return "The virus has the same name as the creator of this silly game";
	}
	static string Point(Session S, File* F, File* chk) {
		auto r = NameClass::RandomName();
		while (r.CName() == F->VUP) r = NameClass::RandomName(); // Make sure the virus itself is NEVER chosen
		if (S->Files.count(r.CName())) return r.Name() + " is the virus!";
		return "";
	}

	static string Contain(Session S, File* F, File* chk) {
		auto c = (char)TRand(65, 90);
		auto t = (byte)TRand(1, 7);
		uint16 found{ 0 };
		uint16 ml = (uint16)TRand(2, F->VUP.size());
		for (uint16 i = 0; i < F->VUP.size(); i++) {
			if (F->VUP[i] == c) found++;
		}
		switch (t) {
		case 1:
			if (ml == found) {
				char ret[200];
				sprintf_s(ret, "The letter '%c' can not be found %d times in the name of the virus", c, ml);
				return ret;
			}
			// NO BREAK! FALLTHROUGH INTENDED!!!
		case 2:
		case 3:
		case 4: {
			char ret[255];
			if (found)
				sprintf_s(ret, "The virus does not contain the letter '%c'", c);
			else
				sprintf_s(ret, "The virus does contain the letter '%c'" , c);
			return ret;
		}
		case 5:
		case 6:
		case 7:
			return "";
		default:
			return "The *is* no virus in this game"; // Just a useless hint, but this situation should not occur.
		}

	}
	static string IAm(Session, File*, File*) { return "I am the virus"; }

	static string Second(Session S, File* V, File* C) {
		auto ch{ (char)TRand(65,90) };
		//char fmt[255];
		char ret[255];
		if (V->VUP[1] == ch) {
			sprintf_s(ret, 255,"The second letter in the name of the virus is not a '%c'",ch);
		} else {
			sprintf_s(ret, 255,"The second letter in the name of the virus is a '%c'", ch);
		}
		//sprintf_s(ret, fmt, ch);
		return ret;
	}
	static string Boy(Session, File* V, File*) {
		if (V->Stuff.Boy())
			return "The virus does not have a boy's name";
		else
			return "The virus has a boy's name";
	}
	static string Girl(Session, File* V, File*) {
		if (V->Stuff.Girl())
			return "The virus does not have a girl's name";
		else
			return "The virus has a girl's name";
	}

	static vector<FHint> Hint{
		UselessHint1, // 0
		UselessHint1, // 1
		UselessHint2, // 2
		UselessHint3, // 3 // Actually the best hint in there, but that's within in the scope of the game... useless.
		FirstLetter1, // 4
		FirstLetter1, // 5
		FirstLetter2, // 6
		FirstLetter3, // 7
		FirstLetter4, // 8
		Second, // 9
		Second, // 10
		LastLetter1, // 11
		LastLetter2, // 12
		LastLetter3, // 13
		LastLetter4, // 14
		FirstAndLast, // 15
		Num, // 16
		Num2, // 17
		Num2, // 18 // Just increase the chance this happens
		Even, // 19
		Same, // 20
		SameAsVirus1, // 21
		SameAsVirus2, // 22
		SameAsVirus3, // 23
		Jeroen, // 24
		Point, // 25
		Boy, // 26
		Girl, // 27
		Contain, // 28
		IAm
	};


	string GetHint(Session Ses, File* VFile, File* CFile) {
		auto r = TRand(0,Hint.size() - 1);
		auto ret{ Hint[r](Ses, VFile, CFile) };
		if (ret.size() && ret[0] >= 65 && ret[0] <= 90) return ret;
#ifdef HINTDEBUG
		static map<uint16, uint16>times{};
		if (ret.size())
			Chat("Unwanted outcome (" << r << ")('" << ret << "') (This function did so " << (++times[r]) << "x)");
#endif
		return ""; // Likely faulty
	}

}