# LordPsyan Modules https://github.com/LordPsyan/OregonCore-Modules

# BeastMaster

# Description
Beastmaster is an npc that will allow either hunters, or anyone to get a pet instantly. If you do not know the standard pet spells (feed pet, call pet, tame pet, etc.) it will atuomatically teach them to you


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
The sql files are located in mod_beastmaster/sql/
They will be imported into your database automatically, assuming you have
DatabaseUpdater.Enabled = 1
set in your oregoncore.conf file.

To do it manually, just run the 2 sql files located in <source_of_oregoncore>/modules/mod-BeastMaster/sql/


## Server Config Setup
### On Windows
Modify the dabatase table world.mod_config
there are 3 entries:
	BeastMaster.OnlyHunter
	BeastMaster.Announce
	BeastMaster.EnableNormalPet
Set those to 1 for on or 0 for off.

### On Linux
Modify the dabatase table world.mod_config
there are 3 entries:
	BeastMaster.OnlyHunter
	BeastMaster.Announce
	BeastMaster.EnableNormalPet
Set those to 1 for on or 0 for off.

## Start the server and enjoy
Done, you are ready to use the BeastMaster NPC! Go online, spawn him (entry 99990 by default) and try him out!
