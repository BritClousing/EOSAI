## Contents

* [Introduction](#introduction)
* [Structure of the Map](#structure-of-the-map)
	* AIRegions, MultiRegions, Geos, and Map Objects (Units, Cities, Resources)
* Pathfinding
* Heat Map for Movement
* Movement maps for various move types
* [Creating the EOSAI Interface (For Game Developers)](#creating-the-eosai-interface-for-game-developers)

## Introduction

The Maps System is used for pathfinding and identifying which regions are vulnerable.

Pathfinding

The pathfinding system has to take several factors into account when running calculations.  The system wants to minimize the amount of time it takes to move from point A to point B.  However, the pathfinding system may also want include other factors as well, including: avoiding enemy units and avoiding being seen by enemies.

Region Evaluation

The map system is used to figure out what regions are vulnerable (this means identifying regions controlled by the AI that are vulnerable to invasion, and identifying enemy-controlled regions that are vulnerable to AI invasion).

## Structure of the Map

**AIRegions** The entire map is split-up into territories. Each territory is called an "AIRegion". The AI uses information about these AIRegions (which regions are adjacent to each other, how much distance there is from the center of one region to the next) to plan pathway movement. In Empires of Steel, units can move continuously across the map (i.e. there are no grids). When building games, it's not necessary to allow for continuous movement. In small EOS maps, there are about 30x30 AIRegions, for a total of around 900 AIRegions. The largest EOS maps can have around 12,000 AIRegions.

**MultiRegions** For faster calculations, the AI groups AIRegions together and calls them "MultiRegions". This allows for faster calculations, which is important since the AI runs lots of hypothetical scenarios.

**Geos** The AI groups areas together into "Geos". A "Geo" is a continuous area of land or sea (called LandGeos and WaterGeos). It allows the AI to quickly figure out if, say, a ground unit at location X can move to location Y without a transport.

**Map Objects** (Units, Cities, Resources) have to be placed on the map. Each MapObject has a defined location - which means it has an x,y location, a single AIRegion, and a single MultiRegion. Map objects can be items the AI would like to protect (his own cities, resources), units for accomplishing goals, enemy units he'd like to destroy, and valuable enemy objects the AI would like to capture or destroy (e.g. cities).

Below is an example of a map.  The left image is the 'normal view' of the map.  The right image shows AIRegions (each differently shaded area is an AIRegion).

![](http://i.imgur.com/E4MBYXd.png)


## Creating the EOSAI Interface (For Game Developers) ##

The EOSAIInterface class must convert the game map into an AIRegion map. An AIRegion is a small section of the map (note: the sections shouldn't be too small or too big, more on that later).

The mapping system requires several things:

* Each part of the map must belong to an AIRegion.  (In other words: there should never be a situation where a unit, city, etc exists on the map, but can't be resolved to an AIRegion.  The AI cannot find a pathway between two map-objects that aren't inside AIRegions.)
* The AIRegions must be linked together so the AI knows what regions connect to which regions (i.e. Region A is adjacent to Region B), and include a distance between them.
* Each AIRegion should have the same terrain.  In other words: a region might a "water" or "forest" or "plains".  An AIRegion can't contain both forest+water or even forest+plains.  This is important for figuring out pathways because the pathway calculated for a ground unit is different than a pathway calculated for ships.  If a tank can't move through forests, then it won't be allowed to move through any AIRegions designated as "forest".


