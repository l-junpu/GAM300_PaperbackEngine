//----------------------------
//           README
//----------------------------

1) Open the "build" folder and run GetDependencies.bat
2) Once the Dependencies installation is completed, return to the initial "CS396_Project" folder
3) Open "dependencies/xcore/dependencies/zstd/build/VS2010"
4) Open "zstd.sln" and right-click on "zstd" and "lib-zstd-dll" solutions and open the properties page
5) Navigate to "C/C++ -> General" and set "Treat Warnings as Errors" to "No"
6) Compile the "zstd.sln" on both "Debug" and "Release" in x64 mode
7) Navigate back to the initial "CS396_Project" folder
8) Run the project by navigating to "build/CS396_Proj" and run "CS396_Proj.vs2019.sln"