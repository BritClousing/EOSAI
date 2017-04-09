
set VersionManager=..\..\BCToolkit\VersionManager

%VersionManager% Version.h "#define BUILD_ID" "++"
rc /fo %1%/EOSAI.res EOSAI.rc
