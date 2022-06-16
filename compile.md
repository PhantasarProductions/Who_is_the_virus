# Compile "Who is the Virus?"

The project was entirely created with Visual Studio 2022. How well it will compile with G++ or other C++ compilers, I don't know, as I never tried (hence the absence of a makefile or stuff like that).

The external libraries needed to compile this game are "Tricky's Units for C++", "JCR6" and "Medals". When compiling in Visual Studio 2022 you can best dedicate a folder entirely for my work. Now I recommend to do this from the command line by either cmd, powershell or cygwin (or just the sh/bash prompt when you are on in a Unix based environment, however I am not sure how well VS2022 will work on that, although if I can get this compatible with G++, I will set the compile scripts up in the same manner).

Go to this folder and use the next commands
~~~shell
git clone "https://github.com/PhantasarProductions/Who_is_the_virus.git" WhoIsTheVirus
git clone "https://github.com/Tricky1975/TrickyUnits_CPP.git" TrickyUnits
git clone "https://github.com/jcr6/JCR6_CPP" JCR6
git clone "https://github.com/TrickyGameTools/Medals.git" Medals
~~~

Now when it comes to Medals, you can just ignore the .cs file, which is merely there because the medal system had to work in both C# and C++. You may however need to replace the line "#define TRUE_MEDALS" with "#undef TRUE_MEDALS" or Medals won't compile. Please note, when you compile the game yourself, you cannot get the bonusses in my other games for making achievements, which is actually an anti-cheat measure. The file in the repository merely contains some empty functions in order to prevent linker errors.

Now the game does not call to any third party libraries (the zlib library in JCR6 is NOT used in this game, and its header won't be called, so that won't matter at all), so basically the WhoIsTheVirus.sln file should then just work. 


(The setup project in the solution is rather experimental and never gave me satisfying results, so best is to ignore its existence).



