 [English](README.md) 
# PART 1 - How to create your own module

use me as skeleton

### How to test your module?

Disable PCH (precompiled headers) and try to compile. If you forgot some headers, it is time to add them !
To disable PCH, set `USE_COREPCH ` to 0 with Cmake.


-------------------------------------------------------

# PART 2 - EXAMPLE OF A README.md

# MY NEW MODULE (title)


## Description

This module allows to do this and this.


## How to use ingame

Do this and that.


## Requirements

My new module requires:

- OregonCore Restructured


## Installation

```
1) Simply place the module under the `modules` directory of your OregonCore source. 
2) Import the SQL manually to the right Database (auth, world or characters) 
3) Re-run cmake and launch a clean build.
```

## Edit module configuration (optional)

If you need to change the module configuration, go to your server configuration folder (where your `oregon-core` or `oregon-core.exe` is), copy `my_module.conf.dist` to `my_module.conf` and edit that new file.


## Credits

