![logo](https://avatars2.githubusercontent.com/u/14118733?s=400&v=4) Welcome to OregonCore!
=================================================================================

This repository is dedicated to all the oregoncore modules & Eluna Scripts

Build Status
------------

| Compiler      | Platform    | Branch | Status                  |
|:--------------|:------------|:------:|:-----------------------:|
| clang         | Linux x64   | master | [![Build Status][1]][7] |


How to install a module
---------------------------
```
1) Simply place the module under the `modules` directory of your OregonCore source. 
2) Import the SQL manually to the right Database (auth, world or characters) or using the DB updater in the config
3) Re-run cmake and launch a clean build of Oregoncore.
```

Why does some modules state azerothcore?
----------------------------------------
Some of the modules are backports from 3.3.5 some of these modules that are backported are ones that i have worked on or that i created for azerothcore.

Can i PR to post more modules here?
-----------------------------------
Yes, anyone if free to post modules simply open a PR and it will be merged.


[1]: https://api.travis-ci.org/talamortis/OregonCore-Modules.svg?branch=master
[2]: https://ci.appveyor.com/api/projects/status/bxn9cq9miqxn33gr/branch/master
[3]: https://wiki.oregon-core.net/
[4]: https://docs.oregon-core.net/
[5]: https://discord.gg/Nyc3fTy
[6]: https://forums.oregon-core.net/
[7]: https://travis-ci.org/github/talamortis/OregonCore-Modules
[8]: https://ci.appveyor.com/project/OregonCore/OregonCore/branch/master
