# LordPsyan Modules https://github.com/LordPsyan/OregonCore-Modules

# Summon Item

# Description
Summon item is a simple script that lets you right click an item, and it summons an npc of your choice.

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
there are 4 entries:
	Summon.Enable
	Summon.Announce
Set those to 1 for on or 0 for off.
	Summon.SpellId
Set this to the spell ID you want as a visual. Default is 40162
	Summon.CreatureEntry
Set this to the creature_template_entry of your choice. Default is 100000


### On Linux
Modify the dabatase table world.mod_config
there are 4 entries:
	Summon.Enable
	Summon.Announce
Set those to 1 for on or 0 for off.
	Summon.SpellId
Set this to the spell ID you want as a visual. Default is 40162
	Summon.CreatureEntry
Set this to the creature_template_entry of your choice. Default is 100000


## Start the server and enjoy
Done, you are ready to use the summon item! Go online, add the item to your inventory (entry 90002 by default) and try him out!
