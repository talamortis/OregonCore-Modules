# LordPsyan Modules https://github.com/LordPsyan/OregonCore-Modules

# NPC Exchanger

# Description
NPC Exchanger is a simple script that lets you right exchange one type of points for another. Example: 1000 Arena Points for 100 honor points. THis can all be set in a database table.

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
	NPC_Exchanger.Enable
	NPC_Exchanger.Announce
Set those to 1 for on or 0 for off.


### On Linux
Modify the dabatase table world.mod_config
there are 2 entries:
	NPC_Exchanger.Enable
	NPC_Exchanger.Announce
Set those to 1 for on or 0 for off.


## Start the server and enjoy
Done, you are ready to use the npc exchanger! Go online, add the npc to your realm (entry 100070 by default) and try him out!
