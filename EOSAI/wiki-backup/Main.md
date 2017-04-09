# EOSAI #

EOSAI is an artificial intelligence system for strategy games. 

It was originally part of the game [*Empires of Steel* (EOS)](http://www.empiresofsteel.com), but the code is being generalized for use in other strategy games.

**NOTE: This repository is still in the initial stages of being uploaded. It is not fully functional yet.**

## Contents: ##

1. [Legal](#legal)
2. [Introduction](#introduction)
	* What can I use the EOSAI library for?
	* What Platform does EOSAI work on?
	* What kinds of strategy games does EOSAI work for?
3. [Overview of EOSAI](#overview-of-eosai)
	* Parts of the EOSAI library
	* Compiling EOSAI
4. [Compiling EOSAI](#compiling-eosai)
5. [Debugging EOSAI](#debugging-eosai)
6. [For Game Developers / Using the Library in a Game](#for-game-developers--using-eosai-in-a-game)
	* Communicating to/from the library
7. For AI Developers / Improving the Library
8. [EOSAI Concepts](#eosai-concepts)
	* EOSAI Timeline (Pre, Post, Messaging)
	* Map
		* Pathfinding
		* Heat Map for movement
		* Movement maps for various move types
		* Object Types (Poi, Units, Cities, Resources)
	* Combat Calculations
	* Build Management
	* Tech Research
	* Foreign Relations
		* Historical Events and Opinion
		* War, Peace, Alliances (current + planning)
		* SATE score
		* Resigning
		* Trade and Negotiation
		* Player Communication
9. [EOSAI Viewer Application](#eosai-viewer-application)
	1. Building the Viewer
	2. Using the Viewer
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

EOSAI built for turn-based games where all units can move each turn. This is a common design for 4X games (e.g. Civilization, Masters of Orion, etc).

EOSAI is *not* built for real-time strategy games. The AI takes some time to "think" about it's turn. It would be nice if EOSAI could (at some point in the future) handle RTS AI, but it can't handle it currently.

EOSAI is also not built for games like chess where where one "unit" is moved per turn.


## Overview of EOSAI

EOSAI gets compiled into a dll ("EOSAI.dll").  The dll is dynamically linked to the game.

### Parts of the EOSAI Project

* **EOSAI Source Code/Library (EOSAI.dll)** - The majority of source code in this project is used to create EOSAI, which is the AI.

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

	![](http://i.imgur.com/qfugwrq.png)

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

## EOSAI Concepts

## EOSAI Viewer Application

The EOSAIViewer Application gives developers a good high-level view of what the AI is doing and why. It's at the beginning stages of development.

### Building the Viewer

EOSAIViewer is built using QT because it's cross-platform. This means developers will need to download QT in order to compile it.

TODO: Details on the version of QT to use.

I'm currently looking into what version I need for this. I think I previously used Qt_5_5_1_MSVC2013_32bit

### Using the Viewer

Nothing to see here yet.

## Glossary

Poi - An acronym for point of interest. It's the base class for all objects on the map which have a defined place on the map. Examples: a mountain, a city, a military unit.

SATE Score - An acronym for Self, Ally, Teammate, Enemy. It's a way of calculating a nation's situation. A powerful nation + lots of powerful allies + no enemies results in a high SATE score. A weak nation + no allies + powerful enemies results in a negative SATE score.

[OtherPage](OtherPage)
