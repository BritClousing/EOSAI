set cpin=c:\Programming\BCToolkit\cpin

call _Setup.bat

rem %cpin% "..\Source\EnumTradeAgreementState.h" ./External/
rem %cpin% "..\Source\CombatUnitType.*" ./External/

%cpin% "..\..\BCToolkit\LogFile.*" ./External/

rem %cpin% "..\Source\Unitset.*" ./External/

%cpin% "..\Source\Location.h" ./External/

%cpin% "..\Source\BCDumbArray1D.h" ./External/
%cpin% "..\Source\BCDumbArray2D.h" ./External/

%cpin% "..\Source\EnumFieldOrders.h" ./External/
%cpin% "..\Source\EnumPoiObjectType.h" ./External/
%cpin% "..\Source\EnumAITypeOfUnit.h" ./External/
%cpin% "..\Source\EnumAttackGoal.h" ./External/

%cpin% "..\Source\SimpleCharMap.*" ./External/

%cpin% "..\Source\LongSet.*" ./External/

%cpin% "..\Source\UnitsubsetPermission.*" ./External/
%cpin% "..\Source\AIUnitCombatCapability.*" ./External/
%cpin% "..\Source\StringAndLongSet.*" ./External/
rem %cpin% "..\Source\BuildOption.*" ./External/
rem %cpin% "..\Source\MovementDescription.*" ./External/
rem %cpin% "..\Source\BuildCompletion.*" ./External/
rem %cpin% "..\Source\BuildOrder.*" ./External/
rem %cpin% "..\Source\MovementUnitType.*" ./External/

%cpin% "..\Source\UnitTag.*" ./External/

rem %cpin% "..\Source\WorldBuildDesc.*" ./External/





