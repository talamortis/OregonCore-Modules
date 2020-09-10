# LordPsyan Modules https://github.com/LordPsyan/OregonCore-Modules

# TemplateNPC

# Description
TemplateNPC is an npc will let you instant (or earned) level 70 characters choose their spec of choice. It will give you armor and weapons, PLus I added 5k gold and bags (Netherweave Bags 16 slot)
It places them in proper slots. You can add or remove anything you wish. All you need to know is the slot number.

This checks to see if your player has any items equipped, and warns you to remove all your items.

WARNING: it add a hearthstone, so players must remove the hearthstone. It also adds bags to slots, so basically, they need to be completely naked with 0 items and 0 extra bags.
For hunters, it adds a quivver and 3 bags. Everyone else it adds 4 bags.


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
The sql files are located in mod_TemplateNPC/sql/
They will be imported into your database automatically, assuming you have
DatabaseUpdater.Enabled = 1
set in your oregoncore.conf file.

To do it manually, just run all the sql files located in <source_of_oregoncore>/modules/mod-TemplateNPC/sql/


## Server Config Setup
### On Windows
Modify the dabatase table world.mod_config
there are 2 entries:
	TemplateNPC.Enable
	TemplateNPC.Announce

Set those to 1 for on or 0 for off.

### On Linux
Modify the dabatase table world.mod_config
there are 2 entries:
	TemplateNPC.Enable
	TemplateNPC.Announce

Set those to 1 for on or 0 for off.

## Start the server and enjoy
Done, you are ready to use the TemplateNPC NPC! Go online, spawn him (entry 99991 by default) and try him out!
