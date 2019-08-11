<p align="center">
  <img width="500" height="250" src="https://i.imgur.com/cEwKoeV.png">
</p>

[![Discord](https://img.shields.io/discord/559937792092667904.svg?label=discord)](https://discord.gg/dEMgzUZ)
[![Build status](https://ci.appveyor.com/api/projects/status/hed52wml7ovx3qxb?svg=true)](https://ci.appveyor.com/project/EternityX/deadcell-csgo) 
[![GitHub Issues](https://img.shields.io/github/issues/EternityX/DEADCELL-CSGO.svg)](https://github.com/EternityX/DEADCELL-CSGO/issues)
[![GitHub license](https://img.shields.io/badge/license-MIT-blue.svg)](https://github.com/EternityX/DEADCELL-CSGO/blob/master/LICENSE)

## Preface
Full CS:GO project files for [deadcell.cc](https://deadcell.cc/), for the OSHGui project, please see the [OSHGui repository](https://github.com/EternityX/DEADCELL-OSHGUI).

This is *not* a ready-to-use cheat, it is missing a lot of core features and has a multitude of issues. As such it is highly recommended that you do not use this if you have no prior programming knowledge. However we are working towards a stable, free, feature-rich cheat for everyone to use!

If you want a clean base without any features, please see the [deadcell SDK](https://github.com/alpine971/DEADCELL-SDK).

## Discord
https://discord.gg/dEMgzUZ

We invite anyone interested in supporting development to join our Discord.

This is NOT for tech support. If you have an issue, open one [here](https://github.com/EternityX/DEADCELL-CSGO/issues/new/choose).

## Known issues
- Crashes.
- Some of the code is unoptimized.
- If you come across any more issues, please notify us via the issue tracker.

We are accepting any pull requests that fix any of the aforementioned issues.

## Contributing
See [CONTRIBUTING.md](https://github.com/EternityX/DEADCELL-CSGO/blob/master/CONTRIBUTING.md).

## FAQ
### Where's the DLL?
Build it yourself. We will not provide a binary.

### How do I add a hitmarker sound?
Create a **hitsounds** folder inside the **base** Counter-Strike Global Offensive directory.
Place as many **.wav** sound files as you want inside of it. Load the cheat and they will be under the hitsounds combobox.

### Where are config files stored?
Profiles are stored inside the **base** Counter-Strike Global Offensive directory under a folder named **profiles**.

You can share profiles with friends by navigating to the configuration tab inside the cheat and selecting **Export**. This will copy all profile data to the clipboard which you can then paste to a friend.

To import a profile, just copy all of the profile data and press **Import**. Make sure to save your profile by entering a name and pressing **Save**.

### Why does "Generating code..." take forever when building the project?
Most likely an issue with Visual Studio 2017, you can wait it out or use Visual Studio 2019 instead.

### Why does moving the menu crash the game?
Disable multi-core rendering in your CS:GO video settings.

### How do I open the menu?
Press `INSERT`.

### How do I unload the cheat?
Press `END`.

## Credits 
- dex and maddie for their address, hash, module, netvar, pattern, pe, and vmt classes
- [KN4CK3R for OSHGui](https://github.com/KN4CK3R/OSHGui)
- [nlohmann for a modern C++ implementation of JSON](https://github.com/nlohmann/json)

## License
Licensed under the MIT License. See [LICENSE](https://github.com/EternityX/DEADCELL-CSGO/blob/master/LICENSE) for more details.

## Media
![image](https://i.imgur.com/q5M8Qx4.png)
![image](https://i.imgur.com/JcksnA7.png)
![image](https://i.imgur.com/0wAOPPj.png)
![image](https://i.imgur.com/nziS7Ek.png)
![image](https://i.imgur.com/rtWEJiS.png)
