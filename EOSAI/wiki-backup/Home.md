EOSAI is an artificial intelligence system for strategy games. 

It was originally part of the game [*Empires of Steel* (EOS)](http://www.empiresofsteel.com), but the code is being generalized for use in other strategy games.

**NOTE: This repository is still in the initial stages of being uploaded. It is not fully functional yet.**

## Contents: ##

1. [Legal](#legal)
2. [Introduction](#introduction)
	* What can I use the EOSAI library for?
	* What Platform does EOSAI work on?
	* What kinds of strategy games does EOSAI work for?
3. [Parts of the EOSAI Project](#parts-of-the-eosai-project)
	* EOSAI Source Code
	* EOS.exe (copy of the game Empires of Steel for testing purposes)
	* EOSAIInterface Sample (for Game Developers)
	* EOSAIViewer (not completed)
	* Unit Tests (not created)
4. [Compiling EOSAI](#compiling-eosai)
5. [Debugging EOSAI](#debugging-eosai)
6. [For Game Developers / Using the Library in a Game](#for-game-developers--using-eosai-in-a-game)
	* Communicating to/from the library
7. [For AI Developers / Improving the Library](#for-ai-developers--improving-the-library)
8. AI Components
	* AI Timeline (Pre, Post, Messaging)
	* Personality
	* Strategic AI
	* [Foreign Relations](foreign-relations)
	* Tactical AI
		* Stationary Unit Placement
	* [Map System](map-system)
	* Combat Calculations
	* Build Management
	* Technology Research
	* Trade
		* Communication Style
9. [EOSAI Viewer Application](#eosai-viewer-application)
	* Building the Viewer
	* Using the Viewer
10. [Glossary](#glossary)

## Legal

This source code is available under the MIT License, which is a permissive open-source license.


## Introduction

### What can I use the EOSAI library for?

The intended use of this project include:

* You may use EOSAI in your strategy game(s).
* Developing/Improving AI for the Empires of Steel.
* Providing an example AI for the purpose of teaching developers about AI.

You are free to use the EOSAI library in your strategy games. You do not need to pay to use the library. 

If you use EOSAI, please give it a mention in the credits. 

I'd appreciate it if you let me know about projects using EOSAI. I might be willing to link to them on the EOSAI webpage.

You are not obligated to contribute your code back to this project (although I would appreciate it if you did, plus remaining current with the project means you'll benefit from improvements and fixes in the future).

### What Platform does EOSAI work on?

EOSAI is built for Windows. I would like to move away from the Windows-only dependencies. It was originally built using MFC (Microsoft Foundation Classes), but I would like to move towards using platform-independent classes (like the Standard Template Library) so that EOSAI will be available cross-platform.

### What kinds of strategy games does EOSAI work for?

EOSAI built for turn-based 4X games (e.g. Civilization, Masters of Orion, etc).

EOSAI is *not* built for real-time strategy games (e.g. Starcraft). The AI takes some time to "think" about it's turn. It would be nice if EOSAI could (at some point in the future) handle RTS AI, but it can't handle it currently.

EOSAI is also not built for games like chess where where one "unit" is moved per turn.

EOSAI does not learn. It uses calculations to figure out what it's going to do and contains sub-systems that allow it to perform those actions (e.g. a system for considering hypothetical actions and scoring them so it can choose the best option, a pathfinding system that allows it to plan movement from point A to point B, a combat calculator to decide if it has enough military power to win a battle before committing troops to that plan, etc).

## Parts of the EOSAI Project

* **EOSAI Source Code/Library (EOSAI.dll)** - The majority of source code in this project is used to create EOSAI, which is the AI. EOSAI gets compiled into a dll ("EOSAI.dll"). The dll is dynamically linked to the game.

* **EOS.exe** - The project includes a copy of the Empires of Steel game (binary code only). Developers can use the game as a test-bed for EOSAI changes. The EOS application dynamically links to EOSAI, so AI developers can change the AI and run Empires of Steel to test how it changes the AI behavior in the game. It's possible to run the debugger in the EOSAI project and step through the AI code during an EOS game.

* **EOSAI Interface Sample** - When a game uses EOSAI, it needs to communicate over an interface. This interface sends information to EOSAI for processing, and receives information back from EOSAI about the AI's decision. There's sample source code from EOS included in this project. If your game uses the EOSAI library, you'll have to create your own EOSAI Interface.

* **EOSAI Viewer** - An application is being written to allow developers (game developers and AI developers) to get a high-level view what the AI is doing, and debug what's going on. (Regular debugging is good for low-level code, but doesn't provide a good high-level view of what the AI is doing or why.) This project is just being started. The EOSViewer is being built using QT (because QT runs cross-platform).

* **EOSAI Test** - (Not included yet.) The EOSAI library will include a test-bed for verifying that the AI is "thinking" and acting rationally, and that code changes did not break functionality.

![](http://i.imgur.com/54rGRC8.png)

## Compiling EOSAI

### Compiling EOSAI

Instructions for building EOSAI using Visual Studio:

**Download Visual Studio 2015 Community Edition.** 

Any edition of VS2015 should be fine, but if you don't already have it, the community edition is free.

During the installation process, choose "Desktop Development with C++"

https://www.visualstudio.com/vs/older-downloads/

**Download Boost 1.63.**

http://www.boost.org/users/history/version_1_63_0.html

It needs to be placed in a directory adjacent to the EOSAI base directory. Example:

	C:\Dev\EOSAI
    C:\Dev\boost_1_63_0

Open EOSAI.sln in VisualStudio, and compile it.

## Debugging EOSAI

The project includes a copy of EOS-EOSAI.exe, which you can use as a test-bed. EOS-EOSAI.exe is a copy of the game "Empires of Steel". If you build a debug copy of EOSAI, you can step through the EOSAI code in the debugger - just press the "debug" button in Visual Studio).

If things aren't working when you try to debug, you'll have to double-check to make sure that:

* You're building the debug version of EOSAI
* The dll get copied to the same directory as EOS-EOSAI.exe
* That VisualStudio points to the location of EOS-EOSAI.exe when the debugger is initiated. 
	(Go to "Configuration Properties->Debugger" and check if "Command" is set to the correct location - "..\..\EOS\Build\EOS-EOSAI.exe" by default.)

(TODO: Make sure the project includes EOS-EOSAI.exe and associated files are included.)

## For Game Developers / Using EOSAI in a Game

### Libraries: ###

In order to use EOSAI with a game, you need to:

First, build a copy of EOSAI. (We don't have EOSAI.lib/EOSAI.dll binaries for you.) See section 4 "For AI Developers" for details on compiling EOSAI.

	In the game's source code/make files:
	* Add the EOSAI/Build/Export directory to the library paths.
		It includes EOSAI.lib and EOSAI.dll files.
	* Include the EOSAI.lib (which adds the EOSAI.dll at runtime).
	* Add the EOSAI/Build/Export/Include directory to the library paths.
		It has all the header files for EOSAI that you'll need.
	* Use #include "EOSAIInterface.h" (to include EOSAI header files)
	* Derive from two classes:
		class MyEOSAIInterface : public EOSAI::CInterface{}
		class MyEOSAIWorldDistanceTool : public EOSAI::CWorldDistanceTool{}
		You will override methods in these classes to communicate with EOSAI.
		There are examples of this in the EOS example code (see EOSAI/ExampleCode).
		See the next section for details on this information.

### EOSAI Data Input/Outout: ###

Each turn, the game needs to send information to the AI so that it can make decisions.  The AI will process the data and return results.  An example of how to do this can be found in the "EOSAI\Build\Export\ExampleCode\" directory.  Details on how to do this are listed below.

**Input data:**

- **Map Data:** The AI needs to know the layout of the map.  EOSAI sees the world as a bunch of connected territories.  This map information is used for a few different purposes: to find pathways and to figure out the vulnerability of different regions.  Below is a image of the regions.  Each different color represents a separate region.  (EOS doesn't use regions, everything moves continuously over the map, but the map is cut up into regions for the purpose of doing AI calculations.)  [Maps Click here to view more details on the map system.]

	![](http://i.imgur.com/E4MBYXd.png)

- **Poi (Points of Interest):** Cities, Resources, Airfields, Fortresses, and other points of interest - everything of value on the map should be described because the AI has to make decisions about capturing or destroying them.

- **Unit Templates:** The units in the game need to be classified into types.  For example, all units of the "Sherman Tank" type have the same movement and combat attributes.  (If the game is setup so that every unit on the map have completely different attributes, then the developer must create a unique unit template for each unit in the game.)  The Unit Templates are described by the CAIUnitTemplate class.

- **Units:** Each unit in the game needs to be described - it needs a unit template, a location, health information, an owner (what player controls the unit), etc.  Units are described by the CAIUnit class.

- **Foreign Relations, Agreements and Constraints:** The AI needs to know about what kinds of agreements and constraints it has with other players.  This allows the developer to restrain the AI in various ways (e.g. in a World War 2 game, under no conditions is the US player allowed to declare war on England because that would be historically inaccurate), and allows the AI to make decisions about who to attack and when to make peace (e.g. if the AI is currently at war with Player 2 and is losing, then the AI should not declare war on another player, but should seek to maintain peace with other players to avoid getting overwhelmed).

- **Technology:** The AI needs to make decisions about what technologies to research and how much effort to put into these research efforts.  In order to make these decisions, the AI needs to know information about the research options, including the cost, time, and benefits of the technology.

- **Personality:** Each AI can have a different personality.  The personalities are described by variables that affect his decisions.  For example, the personality might favor certain types of units (e.g. ground units over air units) or might influence his tendency towards aggressive vs cautious play.

- **History:** The AI keeps track of past interactions with other players - for example, if the player recently did a sneak-attack against them (or someone else), or if they were a loyal ally.  This is important in deciding who to attack and who to maintain peace with.  While much of this information is stored inside the AI's data structures, the developer must inform the AI about events in the game (e.g. Player X just sneak attacked them), and the Player can include pre-game historical information (for example, if the game is a WW2 historical scenario that begins in January, 1940, then the developer might want to include historical information that the Germans sneak attacked Poland in September 1939).

**Output:**

Based on the input data, the AI will create some output data:

- **Agreements, Trade, Messages, Declarations:** The AI will output some foreign relations actions.  This might include declarations of war, requests for peace, responses to player messages, trade agreements, etc.

- **Orders:** The AI will write-out orders describing what each city should build, and orders for each of his units, and technology research orders.  (This data is contingent on responses to/from other players.  For example, the AI might give his units order to attack an enemy while, at the same time, sending a message to that same player requesting peace.  If the player agrees to the peace agreement, then the AI's orders will have to be recalculated, changing his units' attack orders and changing his city-build orders to be less war-oriented.)

**Customization:**

If the game allowed for unique types of agreements (like "research agreements"), then those will have to coded into the system.

..

## For AI Developers / Improving the Library

Because the AI is composed of a few different sub-systems, it's easy to work on one particular aspect of the AI without needing to understand all of it.

Components that are already well-developed (read: the system works pretty well, but there are optimizations which make the code more difficult to understand, there might be some bugs to fix):

* **Map System** (fastest route, avoiding enemies). The code for pathfinding is complete, but there are some tweaks that could be done with enemy avoidance. Enemy avoidance could take into account several factors: how valuable for the AI to move secretly (in some cases, it might not be necessary for the AI to move secretly - e.g. if he's bringing in a large, unstoppable fleet; in other cases, he might be outnumbered and he wants a few transports to move ground onto an island; there is also some work that could be done to avoid aircraft better). 
	* **[In-depth information on the Map System.](map-system)**
	* Classes involved in this: See the "Spatial Maps" folder.
* **Tactical AI** - The AI plans it's attacks. Specifically, it considers a variety of options, comes up with a score for these options (including taking account of how quickly he can get units to the location, and whether he has enough military power to capture the target), and chooses the best option(s).
	* Classes involved in this: See the "Tactical AI" folder.
* **Strategic AI** (War/Peace Strategy; deciding who/when to attack, who to make peace with, trying to get other players to join in war against a common enemy). The AI could do a lot better with trying to "seduce" players into alliances. For example, the AI could decide that he needs other players to form an alliance against the strongest player in the game. The AI needs to actually take steps to make that alliance happen.
	* Classes involved in this: 
	See the "Foreign Relations" and "Strategic AI" and "Tactical AI/Summary" folders
	EOSAIGlobalForeignRelations.h/cpp

Components that work, but there's plenty of room for improvement:

* **Trade and Negotiation** (Negotiation messages, counter-offers, etc). The AI needs to understand what it values, how valuable it is, how valuable things are to other players, and try to create/accept trade deals which favors itself. There's a variety of factors here which will influence his negotiation. For example, he'll give a better deal to an ally than an enemy. If the AI is in a three-player game, then giving good trade deals to one player might help him turn the player into an ally against the third player. The relative power of all players will also come into play (helping a weaker player become stronger is generally better than helping a strong player become stronger). The AI might also want to bully weaker players into paying a tribute. The AI's actions should also be dependent on his personality (some AI players might "play nice" with other players, others might be bullies, others might talkative and diplomatic while seeking out allies).
	* Classes involved in this: See "Strategic AI/Messages" and "Foreign Relations/Trade" folders, EOSAIPlayerInteraction_Trade.h/cpp
* **Technology Research** - What technologies should the AI research, and how much should he spend, are there technologies that the AI should research based on countering other players technologies, what technologies should the AI favor based on his geographical situation - e.g. if the map requires a lot of island hopping, then maybe he should favor ship technology; if he's landlocked - like Russia in WW2 - then he should favor ground unit technology.
	* Classes involved in this: See "Strategic AI/Technology" folder and CEOSAIStrategicAI.h/cpp
* **Build Manager** - The AI needs to figure out what to build and where. Ships? Ground Units? Aircraft? This depends on a variety of factors - like the geography where the AI finds itself (if there's lots of unexplored land around the AI, then he should probably build ground units; if the map is a bunch of small islands, then he'll have to build some ships). The AI should also build more expensive units deeper in his territory and construct short-build defensive units closer to the front.
	* Classes involved in this: See EOSAIBuildManager.h/cpp
* **Foreign Relations** - The AI keeps track of historical interactions with other players. There is some additional work that could be done with this system. Right now, the AI keeps track of these historical interactions (past wars, trade agreements, etc) and uses those to adjust its foreign relations feelings towards players. The system is setup so that players feelings fade over time (e.g. a war that ended 5 turns ago produces larger negative feelings than a war 100 turns ago).
	* Classes invovled in this: See "Historical Information" folder.
* **Stationary Unit Placement** - The AI should distribute his units in a way that allows him to defend his territory (e.g. aircraft and ground units near the front). The AI should keep vulnerable units (e.g. transports) further away from the front, away from enemy submarines and aircraft. The AI might want to place some units in areas to attack incoming enemies (e.g. place destroyers in waterways where enemy submarines might try to move).
	* Classes involved in this: See EOSAIWarzoneLocation.h
* **Communication Style** (writing messages to allow the AI to communicate with players in a way that displays some individual personality - e.g. "Nobody crosses the British Empire. We've put up with your insolence long enough.")  I think the communication personality should be customizable by game-developers (i.e. there should be a way for the game to pass-in conversation information to EOSAI, rather than hard-coding it in the library). I'm not sure what the best way to accomplish this would be. Maybe create a class that holds various text messages. The game passes in this class to the AI. This would allow a third-party game to create customized leaders. Maybe there should be a way for modders to design AI personalities.
	* Classes involved in this: See "Strategic AI" Folder

Do not work on this right now:

* **EOSAIViewer** - The idea behind the Viewer is to allow developers to get a high-level view of what/why the AI is doing. This is important for debugging. (I need to do some high-level design of this.)

## EOSAI Viewer Application

The EOSAIViewer Application gives developers a good high-level view of what the AI is doing and why. It's at the beginning stages of development. This component is not very developed at all. It needs more design and mock-up work. I'll be looking into this.

### Building the Viewer

EOSAIViewer is built using QT because it's cross-platform. This means developers will need to download QT in order to compile it.

TODO: Details on the version of QT to use.

I'm currently looking into what version I need for this. I think I previously used Qt_5_5_1_MSVC2013_32bit

### Using the Viewer

Nothing to see here yet.

## Glossary

Poi - An acronym for point of interest. It's the base class for all objects on the map which have a defined place on the map. Examples: a mountain, a city, a military unit.

SATE Score - An acronym for Self, Ally, Teammate, Enemy. It's a way of calculating a nation's situation. A powerful nation + lots of powerful allies + no enemies results in a high SATE score. A weak nation + no allies + powerful enemies results in a negative SATE score.
