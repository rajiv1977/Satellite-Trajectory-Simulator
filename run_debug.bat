@ECHO ON

set BASEDIR=%~dp0
PUSHD %BASEDIR%

RMDIR /Q /S build


conan install . -c tools.system.package_manager:mode=install -c tools.system.package_manager:sudo=True --output-folder=build --build=missing --settings=build_type=Debug
cd build
cmake .. -G "Visual Studio 17 2022" -DCMAKE_TOOLCHAIN_FILE=./build/generators/conan_toolchain.cmake -DCMAKE_POLICY_DEFAULT_CMP0091=NEW 
cmake --build . --config Debug
robocopy ../test/config Debug constel_1.xml /z
robocopy ../test/config Debug constel_2.xml /z
robocopy ../test/config Debug constel_3.xml /z
robocopy ../test/config Debug constel_4.xml /z
robocopy ../test/config Debug constel_5.xml /z
robocopy ../test/config Debug constel_6.xml /z
robocopy ../test/config Debug constel_7.xml /z
robocopy ../test/config Debug test_0.xml /z
robocopy ../test/config Debug test_1.xml /z
robocopy ../test/config Debug test_2.xml /z
robocopy ../test/config Debug test_3.xml /z
robocopy ../test/config Debug test_4.xml /z
robocopy ../test/config Debug test_5.xml /z
robocopy ../test/config Debug test_6.xml /z
robocopy ../test/data Debug TLE.3le /z
Debug\SatelliteTrajectorySimulator.exe 