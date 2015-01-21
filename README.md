DARA
====
An attempt to exploit every single FIFA game on the DS (all regions) to run unsigned code through its save file.

Currently there is support for running code through the following games:

- FIFA 06 (E),
- FIFA 07 (E and U),
- FIFA 08 (E),
- FIFA 09 (E),
- FIFA 10 (E),
- FIFA Street 2 (E and U),

### Usage

Run `make` on the `CRCFix` directory to build CRCFix. This will be used to fix the checksum of a save file to stop it from showing up as corrupt in the game.

Run `make` on the `Exploit` directory to build the exploitable save games.

### Activating the exploit

For FIFA 06 activate the exploit by going to "My Club", "Edit My Club".

For FIFA 10 activate the exploit by going to "My FIFA 10", "Creation Zone", and then "My Custom Club".

For FIFA Street 2 activate the exploit by going to "My Street", "Profile", and then "Edit Profile".

For all other supported games activate the exploit by going to "My FIFA (year)", and then "My Profile".

### Problem games

- FIFA World Cup 2006 (E) CRC fixing support is added, but the game doesn't crash with a large name.
- FIFA Street 3 (E) CRC fixing support is added. The game crashes on the "Touch to Start" screen, but I can't get the PC to go to a meaningful address.
- FIFA 11 (E) CRC fixing support is added. The large name is displayed, but couldn't get the game to crash.