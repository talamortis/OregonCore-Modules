# LordPsyan Modules https://github.com/LordPsyan/OregonCore-Modules

# TeleNPC

# Description
TeleNPC is a complex script that lets you setup a teleport gossip menu. I consider it complex because of all the restrictions you can place on the teleport location. Below is a list of restrictions:

GUILD - Set guild ID so only members of that guild can use or see that teleport location.
GMLEVEL - set restriction based on gm level (0,1,2,3, etc) Good for VIP stuff.
ISGM - Bool (true false) that sets it whether it is GM3.
ACCOUNT - Allows specific teleport locations based on Account ID. Good for player housing, but would be a pain to use.
LEVEL - Level restriction so lower level players cannot go to illidan (just an example)
ITEM - requires possessing an item.
QUEST - Requires a specific quest to either be started, or completed.
GENDER - Gender based locations. Don't need guys teleporting into the ladies locker room!
RACE - Race specific locations. 
CLASS - Class specific locations.
REPUTATION - Reputation required locations.
PLAYER - Specific player locations. This would be more of a pain to setup that Account locations, but it's an option.

There are 3 database tables. Category, Destination, and association.

custom_npc_tele_category - The menu name (Example: Malls, Level 1-60 isntances, etc)
custom_npc_tele_destination - The name and coordnates of a specific location (Example: Black Temple, Stormwind, Orgrimmar)
custom_npc_tele_association - how the category and desitnation are linked.

It is not hard to use and comes with a complete ready to use set of categories and destinations. This is a paged system, so technically there is no limit to the number of destinations or categories.

# Installation
## Core Setup

To add the module follow the next steps:
1. Go into the folder <source_of_oregoncore>/modules
2. Copy the module folder there.

On windows, open git bash and paste this command:
```
git clone https://github.com/LordPsyan/OregonCore-Modules
```
On linux:

```
git clone https://github.com/LordPsyan/OregonCore-Modules
```

## Database Setup
The sql files are located in mod_summon/sql/
They will be imported into your database automatically, assuming you have
DatabaseUpdater.Enabled = 1
set in your oregoncore.conf file.

To do it manually, just run all the sql files located in <source_of_oregoncore>/modules/mod-summon/sql/

## Server Config Setup
### On Windows
Modify the dabatase table world.mod_config
there are 2 entries:
	TeleNPC.Enable
	TeleNPC.Announce
	These basically are just for the module announce when a player logs in.


### On Linux
Modify the dabatase table world.mod_config
there are 2 entries:
	TeleNPC.Enable
	TeleNPC.Announce
	These basically are just for the module announce when a player logs in.


## Start the server and enjoy
Done, you are ready to use TeleNPC... Go online, spawn the npc (entry 100000 by default) and try him out!
