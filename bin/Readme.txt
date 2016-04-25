-------------------
Introduction
-------------------
Sprite Sheet Manipulator
Version: 0.80

-------------------
How to use:
-------------------
Either drag and drop a file to the executable, or press "Load" after starting the program and select a sprite sheet. The program will then split the sheet's sprites by considering the first pixel of the image the transparent layer*. You can now unite pieces of sprites that should be together, or remove images unrelated to the spritesheet.

After having the sprites in their proper dimensions, press "Save" to save them to individual files in the "output" folder. If you want only some sprites to be saved, select them and press "Save".

Then load other sprite sheets if you want, or press "Exit" to close the program.

*In case you open a sprite sheet which the first pixel is not the transparent color, please edit it and open again so that the program may know how to split the sheet.

-------------------
Controls:
-------------------
Hold the left button of the mouse to select sprites. The center of the sprites must be inside the square for them to be selected.

Press the keyboard arrows to move the camera.

Press the key (T)ransparent and click in the desired color to apply transparency.

Press the key (U)nite to unite the selected sprites into one.

Press the key (R)emove to remove the selected sprites.

Press the key (S)ave Sprites to generate the sprites on the output folder. If there are sprites currently selected, only them will be generated to the output folder.

Press the key (6) to switch flag 6, which determines if the sprite numbers are shown.

-------------------
Author:
-------------------
Made by Crístian dias de Oliveira.
Interface and icon also drawn by me.

Contact: ken-cristian@hotmail.com

-------------------
Credits:
-------------------
Made with SFML:		http://www.sfml-dev.org/
	  TGUI:		https://tgui.eu/