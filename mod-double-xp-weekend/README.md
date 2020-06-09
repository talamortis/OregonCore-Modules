-------------------------------------------------------

# XP Weekend module


## Description

This module is a xp module that will allow the server owner/host to select how much XP players can recieve on the weekend via a config in world database: world -> module_config.


## How to use ingame

Apply the module and Just log in on the weekend there you will see an announcement upon login to see if the module has been activated.

## Requirements

My new module requires:

- OregonCore rev.4370


## Installation

```
1) Simply place the module under the `modules` directory of your OregonCore source. 
2) Import the SQL manually to the right Database (auth, world or characters) or automated via DatabaseUpdater.
3) Re-run cmake and launch a clean build of OregonCore.
```

## Edit module configuration (optional)

If you need to change the module configuration, go to your world database, open table `module_config` and edit `XPWeekend.*`.


## Credits

* [Talamortis](https://github.com/Talamortis) (author of the module)
* [ekzobam](https://github.com/ekzobam) (edited for talamortis/OregonCore)

OregonCore: [repository](https://github.com/talamortis/OregonCore) - [discord chat community](https://discord.gg/Nyc3fTy)
