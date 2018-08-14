ECHO ON
echo Building Package: %1 %2 %3
../Arx/Engine/Build/BatchFiles/RunUAT.bat BuildCookRun -nop4 -project=%1 -cook -stage -package -clientconfig=%2 -ue4exe=UE4Editor-Cmd.exe -SKIPEDITORCONTENT -pak -prereqs -targetplatform=%3 -build -CrashReporter -utf8output -compile -verbose
pause
