rem |
rem | This batch file is used to get together all the files for export.
rem |
rem | It copies the EOSAI.dll (release and debug versions) and all the required include files into the Export directory
rem | The Export directory contains all the data that is needed by the game
rem | When the game is released, it only needs to include the EOSAI.dll (release version, not the debug version or include files)
rem |
rem | If you want to use the build with your own strategy game, you can either point to the files in the \Export directory,
rem | or create another cpin task at the end of this batch file.  Write "cpin -r \Export\*.* [YOUR_DIRECTORY]" to recursively 
rem | copy everything to a target directory.
rem |
rem | This (_ConstructExportPackage.bat) should be called in Project->Build Events->Post Build Events

set ExportDirectory=.\Build\Export\

echo Copying EOS Example Code to EOSAI (we just want a copy as an example; we don't compile it)
rem cpin c:/programming/EOS/source/EOSAIInterface_EOS.cpp %ExportDirectory%\ExampleCode\.
rem cpin c:/programming/EOS/source/EOSAIInterface_EOS.h %ExportDirectory%\ExampleCode\.
rem cpin c:/programming/EOS/source/EOSAIWorldDistanceTool_EOS.cpp %ExportDirectory%\ExampleCode\.
rem cpin c:/programming/EOS/source/EOSAIWorldDistanceTool_EOS.h %ExportDirectory%\ExampleCode\.
cpin ../../EOS/source/EOSAIInterface_EOS.cpp %ExportDirectory%\ExampleCode\.
cpin ../../EOS/source/EOSAIInterface_EOS.h %ExportDirectory%\ExampleCode\.
cpin ../../EOS/source/EOSAIWorldDistanceTool_EOS.cpp %ExportDirectory%\ExampleCode\.
cpin ../../EOS/source/EOSAIWorldDistanceTool_EOS.h %ExportDirectory%\ExampleCode\.

echo Copying EOSAI Include files to the Export Directory (so EOS can access it)
rem cpin -c Build\Debug\EOSAI-Debug.dll %ExportDirectory%\EOSAI-Debug.dll
rem cpin -c Build\Debug\EOSAI-Debug.lib %ExportDirectory%\EOSAI-Debug.lib
cpin -c Build\Debug\EOSAI-Debug.dll %ExportDirectory%\EOSAI-Debug.dll
cpin -c Build\Debug\EOSAI-Debug.lib %ExportDirectory%\EOSAI-Debug.lib
cpin -c Build\Release\EOSAI.dll %ExportDirectory%\EOSAI.dll
cpin -c Build\Release\EOSAI.lib %ExportDirectory%\EOSAI.lib
rem cpin -c Build\Release\EOSAI.dll %ExportDirectory%\EOSAI.dll
rem cpin -c Build\Release\EOSAI.lib %ExportDirectory%\EOSAI.lib
cpin -r %ExportDirectory%\*.dll ..\EOS\


echo %ExportDirectory%
cpin .\AIPlayerDesc.h %ExportDirectory%\Include\
cpin .\EOSAIActionRequirement.h %ExportDirectory%\Include\
cpin .\EOSAIActionScheduler.h %ExportDirectory%\Include\
cpin .\AIPlayer.h %ExportDirectory%\Include\
cpin .\EOSAIAirfield.h %ExportDirectory%\Include\
cpin .\EOSAIAirUnitPathwayData.h %ExportDirectory%\Include\
cpin .\EOSAIAttackVs2.h %ExportDirectory%\Include\
cpin .\EOSAIBCArray.h %ExportDirectory%\Include\
cpin .\EOSAIBCArray2.h %ExportDirectory%\Include\
cpin .\EOSAIBCDumbArray1D.h %ExportDirectory%\Include\
cpin .\EOSAIBCDumbArray2D.h %ExportDirectory%\Include\
cpin .\EOSAIBrain.h %ExportDirectory%\Include\
cpin .\EOSAIBuildingDescription.h %ExportDirectory%\Include\
cpin .\EOSAIBuildManager.h %ExportDirectory%\Include\
cpin .\EOSAIBuildOption.h %ExportDirectory%\Include\
cpin .\EOSAIBuildOrder.h %ExportDirectory%\Include\
cpin .\EOSAIMain.h %ExportDirectory%\Include\

cpin .\City.h %ExportDirectory%\Include\
cpin .\EOSAICitResSummary.h %ExportDirectory%\Include\
cpin .\EOSAICitResUnitSummary.h %ExportDirectory%\Include\
cpin .\EOSAICitResValueMultiplier.h %ExportDirectory%\Include\
cpin .\EOSAICombatUnitType.h %ExportDirectory%\Include\
cpin .\EOSAICommonData.h %ExportDirectory%\Include\
cpin .\EOSAIEnumAttackGoal.h %ExportDirectory%\Include\
cpin .\EOSAIEnumFieldOrders.h %ExportDirectory%\Include\
cpin .\EOSAIEnumForeignRelations.h %ExportDirectory%\Include\
cpin .\EOSAIEnumGeoPlan.h %ExportDirectory%\Include\
cpin .\EOSAIEnumGeoSituation.h %ExportDirectory%\Include\
cpin .\EnumAIMailResponse.h %ExportDirectory%\Include\
cpin .\EOSAIEnumMovementType.h %ExportDirectory%\Include\
cpin .\EOSAIEnumPoiObjectType.h %ExportDirectory%\Include\
cpin .\EOSAIEnumTradeAgreementResponse.h %ExportDirectory%\Include\
cpin .\EOSAIEnumTradeAgreementState.h %ExportDirectory%\Include\
cpin .\EOSAIEnumTypeOfUnit.h %ExportDirectory%\Include\
cpin .\EOSAIForeignRelationsSituation.h %ExportDirectory%\Include\
cpin .\EOSAIForeignRelationsState.h %ExportDirectory%\Include\
cpin .\EOSAIGamePlayer.h %ExportDirectory%\Include\
cpin .\EOSAIGameRules.h %ExportDirectory%\Include\
cpin .\EOSAIGenericRegion.h %ExportDirectory%\Include\
cpin .\EOSAIGenericRegionManager.h %ExportDirectory%\Include\
cpin .\EOSAIGeo.h %ExportDirectory%\Include\
cpin .\EOSAIGeoView.h %ExportDirectory%\Include\
cpin .\EOSAIGlobalForeignRelations.h %ExportDirectory%\Include\
cpin .\EOSAIIntSet.h %ExportDirectory%\Include\
cpin .\EOSAIListSort.h %ExportDirectory%\Include\
cpin .\EOSAIListOwned.h %ExportDirectory%\Include\
cpin .\EOSAIListUnowned.h %ExportDirectory%\Include\
cpin .\EOSAILimitedList.h %ExportDirectory%\Include\
cpin .\EOSAILocation.h %ExportDirectory%\Include\
cpin .\EOSAILogFile.h %ExportDirectory%\Include\
cpin .\EOSAILongSet.h %ExportDirectory%\Include\
cpin .\EOSAIJobCapability.h %ExportDirectory%\Include\
cpin .\EOSAIJobRole.h %ExportDirectory%\Include\
cpin .\EOSAIJobsToDo.h %ExportDirectory%\Include\
cpin .\EOSAIMail.h %ExportDirectory%\Include\
cpin .\EOSAIMailResponse.h %ExportDirectory%\Include\
cpin .\EOSAIMathFunction.h %ExportDirectory%\Include\

cpin .\MessageFromAI.h %ExportDirectory%\Include\
cpin .\MessageFromAI_DeclareWar.h %ExportDirectory%\Include\
cpin .\MessageFromAI_TradeDesires.h %ExportDirectory%\Include\
cpin .\MessageFromAI_TradeOffer.h %ExportDirectory%\Include\
cpin .\MessageToAI.h %ExportDirectory%\Include\

cpin .\EOSAIMovementDescription.h %ExportDirectory%\Include\
cpin .\EOSAIMovementUnitType.h %ExportDirectory%\Include\
cpin .\EOSAIMultiRegion2.h %ExportDirectory%\Include\
cpin .\EOSAIMultiRegionManager2.h %ExportDirectory%\Include\
cpin .\EOSAINationalSummary.h %ExportDirectory%\Include\
cpin .\EOSAINationalSummary3.h %ExportDirectory%\Include\
cpin .\EOSAIPathfinderPoint.h %ExportDirectory%\Include\
cpin .\EOSAIPlayerOwnershipMap.h %ExportDirectory%\Include\
cpin .\EOSAIPlayerManager.h %ExportDirectory%\Include\
cpin .\PoiObject.h %ExportDirectory%\Include\
cpin .\PoiObjectState.h %ExportDirectory%\Include\
cpin .\EOSAIPoiMobile.h %ExportDirectory%\Include\
cpin .\EOSAIObjectActionProgression.h %ExportDirectory%\Include\
cpin .\EOSAIQuickCombatCalculation.h %ExportDirectory%\Include\
cpin .\EOSAIQuickCombatResults.h %ExportDirectory%\Include\
cpin .\EOSAISerial.h %ExportDirectory%\Include\
cpin .\EOSAIRegion2.h %ExportDirectory%\Include\
cpin .\EOSAIRegionManager2.h %ExportDirectory%\Include\
cpin .\EOSAIRegionPathfinder.h %ExportDirectory%\Include\
cpin .\EOSAIRegionPathwayMap.h %ExportDirectory%\Include\
cpin .\EOSAIRegionMapToEverywhere.h %ExportDirectory%\Include\
cpin .\EOSAIResource.h %ExportDirectory%\Include\
cpin .\EOSAIResourceEffectsSummary.h %ExportDirectory%\Include\
cpin .\EOSAIRoad2.h %ExportDirectory%\Include\
cpin .\EOSAISimpleCharMap.h %ExportDirectory%\Include\
cpin .\EOSAISimpleMap.h %ExportDirectory%\Include\
cpin .\EOSAIStopwatch2.h %ExportDirectory%\Include\
cpin .\EOSAIStrategicAI.h %ExportDirectory%\Include\
cpin .\EOSAIStrategicAIOrder.h %ExportDirectory%\Include\
cpin .\EOSAIStringAndFloat.h %ExportDirectory%\Include\
cpin .\EOSAIStringAndFloatSet.h %ExportDirectory%\Include\
cpin .\EOSAIStringAndLongSet.h %ExportDirectory%\Include\
cpin .\EOSAITacticalProject2.h %ExportDirectory%\Include\
cpin .\EOSAITacticalProject2Strategy.h %ExportDirectory%\Include\
cpin .\EOSAITaskForce3.h %ExportDirectory%\Include\
cpin .\EOSAITechnologyDesc.h %ExportDirectory%\Include\
cpin .\EOSAITechTreeNode.h %ExportDirectory%\Include\
cpin .\EOSAITerrainBasedValue.h %ExportDirectory%\Include\
cpin .\EOSAIThoughtDatabase.h %ExportDirectory%\Include\
cpin .\EOSAITradeAgreement.h %ExportDirectory%\Include\
cpin .\EOSAITradeAgreementResponse.h %ExportDirectory%\Include\
cpin .\EOSAITransportsSet.h %ExportDirectory%\Include\
cpin .\EOSAIUnit.h %ExportDirectory%\Include\
cpin .\EOSAIUnit2CombatCapability.h %ExportDirectory%\Include\
cpin .\EOSAIUnitPathwayFinder.h %ExportDirectory%\Include\
cpin .\EOSAIUnitPathwayResultStep.h %ExportDirectory%\Include\
cpin .\EOSAIUnitSummary.h %ExportDirectory%\Include\
cpin .\EOSAIUnitActionIdea.h %ExportDirectory%\Include\
cpin .\EOSAIUnitActionStack.h %ExportDirectory%\Include\
cpin .\EOSAIUnitPathwayResult.h %ExportDirectory%\Include\
cpin .\EOSAIUnitPathwayPredefinedStep.h %ExportDirectory%\Include\
cpin .\EOSAIUnitTemplate.h %ExportDirectory%\Include\
cpin .\EOSAIUnitTemplateSet.h %ExportDirectory%\Include\
cpin .\EOSAIUnitTemplateAndFloat.h %ExportDirectory%\Include\
cpin .\EOSAIUnitTemplatesAndFloat.h %ExportDirectory%\Include\
cpin .\EOSAIUnitsubsetPermission.h %ExportDirectory%\Include\

rem |
rem |
cpin .\EOSAIPublic.h %ExportDirectory%\Include\
cpin .\EOSAIIncludes.h %ExportDirectory%\Include\
cpin .\EOSAISettings.h %ExportDirectory%\Include\

rem |
rem | Copy EOSAI interfaces to the Export\Include directory
rem |
cpin .\EOSAIWorldDistanceTool.h %ExportDirectory%\Include\
cpin .\EOSAIWorldDistanceTool.cpp %ExportDirectory%\Include\
cpin .\EOSAIInterface.h %ExportDirectory%\Include\
cpin .\EOSAIInterface.cpp %ExportDirectory%\Include\

rem |
rem | If you want the files copied somewhere else, edit the line below:
rem | cpin -r \Export\*.* [YOUR_DIRECTORY]
rem |
rem | cpin -r %ExportDirectory%\*.* c:\programming\EOS\Source\EOSAI\
rem cpin -r %ExportDirectory%\*.* c:\programming\EOSAI-1.0\EOSAI\
rem cpin -r %ExportDirectory%\*.dll C:\Programming\EOS\Build\
rem cpin -r %ExportDirectory%\*.* ..\..\EOSAI-1.0\EOSAI\
rem cpin -r %ExportDirectory%\*.dll ..\..\EOS\Build\
rem cpin -r %ExportDirectory%\*.dll ..\EOS\

echo Done

