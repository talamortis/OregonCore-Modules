## Pocket Portal 
* Use with mod_TeleNPC2:(https://github.com/talamortis/OregonCore-Modules/tree/master/modules_scripts/mod-TeleNPC2) 
* To spawn a non exploitable Temporary Portal Master

# Installation
## Core Setup

* On windows, open git bash and paste this command:
```
git clone https://github.com/talamortis/OregonCore-Modules
```
* On linux:

```
git clone https://github.com/talamortis/OregonCore-Modules
```

To add the module follow the next steps:
* 1. Go into the folder <source_of_oregoncore>/modules
* 2. Copy the module folder mod-Pocket-Portal and mod-TeleNPC2 there.



## Database Setup
The sql files are located in mod-pocket-portal/sql and mod-TeleNPC2/sql
They will be imported into your database automatically, assuming you have.
* DatabaseUpdater.Enabled = 1
* set in your oregoncore.conf file.

### To do it manually, just run all the sql files located in <source_of_oregoncore>/modules/mod-pocket-portal/sql and mod-TeleNPC2/sql.

## Server Config Setup
### On Windows
Modify the dabatase table world.mod_config
there are 4 entries.
* PocketPortal.Enable = 1 
* Portal.NpcDuration = 60(seconds) 
* TeleNPC.Enable = 1 
* TeleNPC.Announce = 1 


### On Linux
Modify the dabatase table world.mod_config
there are 4 entries.
* PocketPortal.Enable = 1 
* Portal.NpcDuration = 60(seconds) 
* TeleNPC.Enable = 1 
* TeleNPC.Announce = 1 

## Start the server and enjoy
Done, you are ready to use Pocket Portal... Go online, .add item (entry 90044 by default) and try it out!


#Credits
* Micrah/Milestorme: Pocket Portal Creator(https://github.com/milestorme).
* LordPsyan: mod-TeleNPC2 Creator (https://github.com/LordPsyan).
