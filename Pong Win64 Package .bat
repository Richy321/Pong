@ECHO OFF
SET config=Development
SET project=J:/Projects/Pong/Pong.uproject
SET targetPlatform=Win64

ECHO ON
echo Building Package: %project% %config%
../Arx/Engine/Build/BatchFiles/RunUAT.bat BuildCookRun -nop4 -project=%project% -cook -stage -package -clientconfig=%config% -ue4exe=UE4Editor-Cmd.exe -SKIPEDITORCONTENT -pak -prereqs -targetplatform=%targetPlatform% -build -CrashReporter -utf8output -compile -verbose
pause
