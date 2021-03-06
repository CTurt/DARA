DARA
====
An attempt to exploit every single FIFA game on the DS (all regions) to run unsigned code through its save file.

Currently there is support for running code through every single FIFA game on the DS:

- FIFA 06 (E),
- FIFA 07 (E and U),
- FIFA 08 (E),
- FIFA 09 (E),
- FIFA 10 (E),
- FIFA 11 (E),
- FIFA Street 2 (E and U),
- FIFA Street 3 (E),
- FIFA World Cup 2006 (E),

However the CRC code for all of the different regions still needs to be done.

### Usage

Run `make` on the `CRCFix` directory to build CRCFix. This will be used to fix the checksum of a save file to stop it from showing up as corrupt in the game.

Run `make` on the `Exploit` directory to build the exploitable save games.

### Activating the exploit

For FIFA 06, activate the exploit by going to "My Club", "Edit My Club".

For FIFA 10, activate the exploit by going to "My FIFA 10", "Creation Zone", and then "My Custom Club".

For FIFA 11, activate the exploit by going to "MULTIPLAYER", "NINTENDO WI-FI CONNECTION", and then "CONNECT TO NINTENDO WFC".

For FIFA Street 2, activate the exploit by going to "My Street", "Profile", and then "Edit Profile".

For FIFA Street 3, activate the exploit by going to "Multiplayer", "Host Game", and then "Yes".

For FIFA World Cup 2006, activate the exploit by going to "My Cup", and then "My Profile".

For all other supported games, activate the exploit by going to "My FIFA (year)", and then "My Profile".

### Limitations

This is a single stage exploit that runs a small payload which turns the screen different colours. If you want to do something more advanced such as loading a background, you will need to write a second stage which loads more data from EEPROM.

The FIFA 11 exploit runs in DSi enhanced mode on DSi consoles, however it is extremely limited in its current state. The payload is copied with `strcpy`, so it cannot contain any 0 bytes, and it is limited to only slightly more than 100 bytes which can be written before corrupting the CPSR. But it works!
