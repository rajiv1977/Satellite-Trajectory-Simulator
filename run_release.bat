@ECHO ON

set BASEDIR=%~dp0
PUSHD %BASEDIR%

RMDIR /Q /S build


conan install . -c tools.system.package_manager:mode=install -c tools.system.package_manager:sudo=True --build=missing
cd build
cmake .. -G "Visual Studio 17 2022" -DCMAKE_TOOLCHAIN_FILE=./build/generators/conan_toolchain.cmake -DCMAKE_POLICY_DEFAULT_CMP0091=NEW
cmake --build . --config Release
robocopy ../test/config Release constel_1.xml /z
robocopy ../test/config Release constel_2.xml /z
robocopy ../test/config Release constel_3.xml /z
robocopy ../test/config Release constel_4.xml /z
robocopy ../test/config Release constel_5.xml /z
robocopy ../test/config Release constel_6.xml /z
robocopy ../test/config Release constel_7.xml /z
robocopy ../test/config Release test_0.xml /z
robocopy ../test/config Release test_1.xml /z
robocopy ../test/config Release test_2.xml /z
robocopy ../test/config Release test_3.xml /z
robocopy ../test/config Release test_4.xml /z
robocopy ../test/config Release test_5.xml /z
robocopy ../test/config Release test_6.xml /z
robocopy ../test/data Release TLE.3le /z
Release\SatelliteTrajectorySimulator.exe 
