# Spell bonus data

## Description

This module allows players to alter damage of spells and abilities via the database, players can alter spell damage depending on if they are fighting a creature/player

This module also has a reload command so the server does not have to restarted for changes to take effect.

This module can also edit buffs such as 
Power Word: Fortitude
Mark of the Wild ect


## How to use ingame

1. Load the module
2. Fill out data in the spell_bonus_data table located in world DB

## Database Structure Run down

# Editing Spells

- direct_bonus (default 1)
This column is to edit spells such as Frostbolt 

- dot_bonus (default 1)
This column is for spells such as DOTS for example: Insect Swarm and Shaow Word: Pain

- ap_bonus (default 1)
This column is for spells such has Heroic Strike and Mortal Strike

- dot_ap_bonus (default 1)
This Column is used for melee damage over time abiliies such as deep wounds or rend

This is basicly the same layout for players just alter the ****_**_player part


INSERT INTO `spell_bonus_data` (`entry`, `direct_bonus`, `dot_bonus`, `ap_bonus`, `dot_ap_bonus`, `direct_bonus_player`, `dot_bonus_player`, `ap_bonus_player`, `dot_ap_bonus_player`, `comments`) 
VALUES (17, 32.375, 1, 1, 1, 1, 1, 1, 1, 'PW Shield');


# Editing Buffs
Please use the direct_bonus table make make sure all rest is set to "1"


## Requirements

My new module requires:

- OregonCore Restructured


## Installation

```
1) Simply place the module under the `modules` directory of your OregonCore source. 
2) Import the SQL manually to the right Database (auth, world or characters) 
3) Re-run cmake and launch a clean build.
```

## Credits
Talamortis

## Show your a appreciation
[![Donate](https://img.shields.io/badge/Donate-PayPal-green.svg)](https://www.paypal.com/donate/?hosted_button_id=AKRWCTYQD23CQ)