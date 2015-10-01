
I'm not using this file yet

set VersionManager=c:\Programming\BCToolkit\VersionManager

%VersionManager% Version.h "#define BUILD_ID" "++"
rc /fo %1%/EOSAI.res EOSAI.rc
