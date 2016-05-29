/**
 *  Author: Crístian Dias de Oliveira
 *  Email: ken-cristian@hotmail.com
 *  Made using SFML(Simple Fast Media Library)
 *  I must make a copyright.
 */

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include "Spritesheet.h"
#include "SpritesheetDisplayer.h"
#include "SpritesheetView.h"
#include "Mouse.h"
#include <TGUI/TGUI.hpp>
#include "icon.c"
#if __WIN32__
#include <windows.h>
#endif

// OS specific bool - Made to avoid using too many preprocessor directives
bool windows = false,
    linux = false;

// Global Flags and variables
bool flag1 = true,          // If ON automatically unite sprites with colliding bounding boxes
flag2 = false,              // If ON the method to find the first sprite is the diagonal one, otherwise is the default
flag3 = false,              // If ON the diagonal method stated above will find sprites utilizing the tallest sprite found on the line. Otherwise it will move through the center(rounded down) of last sprite.
flag4 = false,              // If ON the sprites will have origins visible and pressing (O)rigin will enter in a mode where the mouse clicks set the origin of chosen sprite.
flag5 = false;              // If ON when saving the sprites will be saved with transparency.
//flag6 = true,             // Show numbers on the sprites
//flag7 = true,             // Allows a sprite to be inside multiple animations
//flag 8 = true,            // If on the sprite selection must collide with center of the sprite instead of a single pixel.

// Set the selected group of sprites as an animation
void setAnimation(std::vector<int> &selectedsprites, std::vector<std::vector<int> > &animations, std::vector<sf::RectangleShape> &rectangles){
    sf::Color color;
    //TODO: If a sprite is already inside an animation, it can't be in another. Or add support to a single sprite with multiple animations
    if (selectedsprites.size() > 1){
        std::vector<int> tempvec;
        for (int i = 0; i < selectedsprites.size(); i++)
        {
            tempvec.push_back(selectedsprites.at(i));
            rectangles.at(selectedsprites.at(i)).setOutlineColor(color.Yellow);
        }
        animations.push_back(tempvec);
    }
}

// Opens a window to select a spritesheet to load. Then loads the spritesheet.
void openFile(sf::RenderWindow *window, sm::SpritesheetDisplayer *spritesheetDisplayer, std::vector<int> *selectedSprites){
    // Windows File Opening
    if (windows){
        const int BUFSIZE = 1024;
        char buffer[BUFSIZE] = {0};
        OPENFILENAME ofns = {0};
        ofns.lStructSize = sizeof( ofns );
        ofns.lpstrFile = buffer;
        ofns.nMaxFile = BUFSIZE;
        ofns.lpstrTitle = "Open Sprite Sheet";
        ofns.Flags = OFN_NOCHANGEDIR;
        if (GetOpenFileName( & ofns ))
            spritesheetDisplayer->loadSpritesheet(buffer, selectedSprites);
        else
            std::cout << "Did not load.";
    }
    else if(linux)
        std::cout << "This is running on linux" << std::endl;
}

/**
 *  TODO: Add the other buttons.
 *  TODO: Add MenuBar.
 *  TODO: Implement Linux opening file system.
 *  Issue: Numbers above sprites causes a lot of performance loss.
 *  TODO: Put zoom in spritesheetView.
 *  Issue: Zoom center wrong when resized.
 *  TODO: When image contains too many sprites or is too big, may add a "This file is too big, are you sure you want to open it?" with a "dont show me this again".
 *  TODO: Add x64 target build.
 *  TODO: Draw icon executable and running program.
 *  TODO: Add option to remove other sprites that are colliding when saving files.
 *  TODO: Add mouse functionality of dragging and dropping a sprite to move it. This is not that useful though, unless i added a save entire spritesheet function.
 *  TODO: Add the flag to merge 2 sprites automatically if they are colliding when parsing.
 *  TODO: Limit how much the view can move, and reset the view when new sprite is loaded.
 *  TODO: Make the transparent button re-parse the spritesheet.
 *  TODO: Add naming system to the sprites, and a list showing the sprites in the interface.
 *  TODO: Add slider to move screen/Another mouse button to move screen.
 *  TODO: Create folder for each spritesheet/Add "Save As" where user decides path. In Save As button can also choose format.
 *  TODO: Make Save button save files as the same extension from the spritesheet.
 *  TODO: (LOW PRIORITY) Ctrl-z Button returns the actions done. Might be a major pain in the behind.
 *  FEEDBACK(not sure if this has purpose): Saving as animations(.gif). SFML might not have support for it.
 *  TODO: If ctrl is being held, add selections to the sprites. If shift is being held remove some sprites from the selection.
 *  TODO: Limit up to which point the camera can go.
 *  TODO: Maybe get rid of TGUI since it's not being that useful. I'm currently creating the button class.
 *  TODO: Put the file opening system into a class or separate it from main.cpp.
 *  TODO: Make a spritesheet file that saves the position of each sprite alongside the image data. Pratically the same thing as generating .json.
 *  TODO: Rescale view to see if resolution gets better.
 *  TODO: Threads for event-driven interface. (It seems i will need to use a c++11 compiler to achieve this, which means i have to build the libraries again.)
 */
int main(int argc, char* argv[])
{
    // Set OS specific variables.
    #if __WIN32__
    windows = true;
    #elif __linux__
    linux = true;
    #endif

    // Opens Console only if in Debug mode
    #if DEBUG
    // Open console window
    AllocConsole();
    freopen("CONOUT$", "w", stdout);
    //freopen("CONIN$", "r", stdin); // No reason yet to have input in the console.
    std::cout << "Sprite Sheet Manipulator Started.\n GCC Version: " << __VERSION__ << DEBUG << std::endl;
    #endif // DEBUG

    // SFML window
	sf::RenderWindow window(sf::VideoMode(800, 600), "Sprite Sheet Manipulator", sf::Style::Titlebar | sf::Style::Resize | sf::Style::Close);
	window.setFramerateLimit(60);

	// Sets window's Icon
    window.setIcon(icon.width,icon.height,icon.pixel_data);

    // SpritesheetDisplayer
    sm::SpritesheetDisplayer spritesheetDisplayer(&window, false);
    // If there is more than a single parameter being given to the executable, pick the first one that's not the name of the executable
    // and try to load a spritesheet with it.
    if (argc > 1)
        spritesheetDisplayer.loadSpritesheet(argv[1]);

    // Views - TODO: Put into the spritesheetView class
	// Other views
	sm::SpritesheetView zoomview(&window, sf::Vector2f(0.01, 0.01), sf::Vector2f(0.68, 0.98));
	zoomview.setCenter(300, 225);
	zoomview.setSize(200, 150);
	sm::SpritesheetView *currentview;
	currentview = spritesheetDisplayer.getView();
    bool zoomed = false;

	// Animations
	std::vector<std::vector<int> > animations;

    // Mouse
    sm::Mouse mouse(&spritesheetDisplayer, &window);

    // TGUI
    tgui::Gui gui{window};

    // TGUI Theme
    tgui::Theme::Ptr theme = std::make_shared<tgui::Theme>("TGUI/SMStyle.txt");

    // TGUI Background
    tgui::Picture::Ptr background = std::make_shared<tgui::Picture>("TGUI/background.png");
    gui.add(background);

    // TGUI Unite Button
    // Could change the name to merge too.
    tgui::Button::Ptr uniteButton = theme->load("button");
    uniteButton->setPosition(window.getSize().x - 225.f, window.getSize().y - 150.f);
    uniteButton->setText("Unite");
    uniteButton->setSize(100, 40);
    uniteButton->connect("pressed", [&](){ spritesheetDisplayer.uniteSprites(mouse.getSelectedSprites()); });
    gui.add(uniteButton);

    // TGUI Remove Button
    tgui::Button::Ptr removeButton = theme->load("button");
    removeButton->setPosition(window.getSize().x - 115.f, window.getSize().y - 150.f);
    removeButton->setText("Remove");
    removeButton->setSize(100, 40);
    removeButton->connect("pressed", [&](){ spritesheetDisplayer.removeSprites(mouse.getSelectedSprites()); });
    gui.add(removeButton);

    // TGUI Load Spritesheet Button
    tgui::Button::Ptr loadSpritesheetButton = theme->load("button");
    loadSpritesheetButton->setPosition(window.getSize().x - 225.f, window.getSize().y - 100.f);
    loadSpritesheetButton->setText("Load");
    loadSpritesheetButton->setSize(100, 40);
    loadSpritesheetButton->connect("pressed", [&](){ openFile(&window, &spritesheetDisplayer, mouse.getSelectedSprites()); });
    gui.add(loadSpritesheetButton);

    // TGUI Pick Transparent Color Button
    tgui::Button::Ptr transparentColorButton = theme->load("button");
    transparentColorButton->setPosition(window.getSize().x - 115.f, window.getSize().y - 100.f);
    transparentColorButton->setText("Transparent");
    transparentColorButton->setSize(100, 40);
    transparentColorButton->connect("pressed", [&](){ mouse.switchTransparentMode(); });
    gui.add(transparentColorButton);

    // TGUI Save to Files Button
    tgui::Button::Ptr saveToFilesButton = theme->load("button");
    saveToFilesButton->setPosition(window.getSize().x - 225.f, window.getSize().y - 50.f);
    saveToFilesButton->setText("Save");
    saveToFilesButton->setSize(100, 40);
    saveToFilesButton->connect("pressed", [&](){ spritesheetDisplayer.saveToFiles(mouse.getSelectedSprites()); });
    gui.add(saveToFilesButton);

    // TGUI Exit Button
    tgui::Button::Ptr exitButton = theme->load("button");
    exitButton->setPosition(window.getSize().x - 115.f, window.getSize().y - 50.f);
    exitButton->setText("Exit");
    exitButton->setSize(100, 40);
    exitButton->connect("pressed", [&](){ window.close(); });
    gui.add(exitButton);

    // Main Loop
	while (window.isOpen())
	{
	    // Handle events.
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type){
            case sf::Event::MouseButtonPressed:
                mouse.mousePressed();
                break;
            case sf::Event::MouseButtonReleased:
                mouse.mouseReleased();
                break;
			case sf::Event::Closed:
				window.close();
				break;
            case sf::Event::Resized:
                spritesheetDisplayer.getView()->resizeView(sf::Vector2f(event.size.width, event.size.height));
				break;
			case sf::Event::KeyPressed:
				switch (event.key.code){
				case sf::Keyboard::Right:
					spritesheetDisplayer.getView()->move(15.0f,0);
					zoomview.move(15.0f,0);
					break;
				case sf::Keyboard::Left:
					spritesheetDisplayer.getView()->move(-15.0f,0);
					zoomview.move(-15.0f,0);
					break;
				case sf::Keyboard::Up:
					spritesheetDisplayer.getView()->move(0,-15.0f);
					zoomview.move(0,-15.0f);
					break;
				case sf::Keyboard::Down:
					spritesheetDisplayer.getView()->move(0,15.0f);
					zoomview.move(0,15.0f);
					break;
                case sf::Keyboard::Num1:
					flag1 = (flag1) ? false : true;
					break;
                case sf::Keyboard::Num2:
					flag2 = (flag2) ? false : true;
					break;
                case sf::Keyboard::Num3:
					flag3 = (flag3) ? false : true;
					break;
                case sf::Keyboard::Num4:
					flag4 = (flag4) ? false : true;
					break;
                case sf::Keyboard::Num5:
					flag5 = (flag5) ? false : true;
					break;
                case sf::Keyboard::Num6:
                    // Flag 6
					spritesheetDisplayer.switchShowNumbers();
					break;
                case sf::Keyboard::A:
                    //setAnimation(selectedsprites, animations, rectangles);
					break;
                case sf::Keyboard::B:
					//TODO: Break Animation and release sprites
					break;
                case sf::Keyboard::L:
                    // TODO: document this key
                    openFile(&window, &spritesheetDisplayer, mouse.getSelectedSprites());
					break;
                case sf::Keyboard::R:
					spritesheetDisplayer.getSpritesheet()->removeSprites(mouse.getSelectedSprites());
					break;
                case sf::Keyboard::F2:
					//TODO: Reset spritesheet
					break;
                case sf::Keyboard::S:
                    spritesheetDisplayer.saveToFiles(mouse.getSelectedSprites());
					break;
                case sf::Keyboard::T:
                    mouse.switchTransparentMode();
					break;
                case sf::Keyboard::U:
					spritesheetDisplayer.getSpritesheet()->uniteSprites(mouse.getSelectedSprites());
					break;
				case sf::Keyboard::Z:
				    if (zoomed)
                    {
                        currentview = spritesheetDisplayer.getView();
                        zoomed = false;
				    }
                    else
                    {
                        currentview = &zoomview;
                        zoomed = true;
                    }
					break;
				}
			}

            gui.handleEvent(event); // Pass the events to the widgets
		}

        // Sets the current view, so that after modifications it's updated.
        window.setView(*currentview);
        // Clear current frame.
		window.clear();
		// Draw all GUI widgets.
		gui.draw();
        // Draw black board.
        spritesheetDisplayer.drawBoard(currentview);
        // Draw spritesheetDisplayer class.
		spritesheetDisplayer.draw(mouse.getSelectedSprites());
        // Draw mouse square.
        mouse.drawSelection();
        // Display frame.
		window.display();
	}

    // Exit program successfully
	return 0;
}
