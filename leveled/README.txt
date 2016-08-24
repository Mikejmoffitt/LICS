Lyle in Cube Sector
Genesis Level Editor
====================

Usage
-----
leveled [map_filename]

When creating a new file, or editing an existing file's meta data, the console
will prompt the user for the following information:
	* Room ID
	* Tileset choice
	* Sprite palette
	* Background
	* Music
	* Map coordinates of the room's top-left region
	* Room title (char *)
	* Width of the room (in 40-column screens)
	* Height of the room (in 30-row screens)

This information can be changed by striking the F8 or I key.

Data Sets
---------

Tilesets, palettes, and backdrops are loaded from Lyle in Cube Sector's project
data files. They are in Sega Genesis format. The level editor expects to be in
the same directory as the res/ folder, and will pull data from there.

Level editor keys
-----------------

Name            	Action
----            	------
Arrows          	Move
F5              	Save
F6 / T          	Change tile source size (16x16 or 8x8)
F7 / R          	Change tile 16x16 snap or sprite grid snap
F8 / I          	Change level metadata; see console for info interview
F9 / P          	Set tile high / low priority mode
F10 / X         	Flip tile about Y axis (H flip)
F11 / Y         	Flip tile about X axis (V flip)
Enter/Space     	Edit selected object data
Delete/Backspace	Remove selected object
Plus/Minus      	Change selected object type

