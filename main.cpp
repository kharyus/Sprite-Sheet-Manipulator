/**
 *  Author: Crístian Dias de Oliveira
 *  Email: ken-cristian@hotmail.com
 *  Made using SFML(Simple Fast Media Library)
 */

#include <SFML/Graphics.hpp>
#include "Logger.hpp"
#include "Spritesheet.hpp"
#include "SpritesheetDisplayer.hpp"
#include "SpritesheetView.hpp"
#include "Mouse.hpp"
#include <TGUI/TGUI.hpp>
#include "icon.c"

// Windows-specific libraries.
#if __WIN32__
#include <windows.h>
#endif

// Todo: Add functionality.
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

// Todo: Refactor
// Opens a window to select a spritesheet to load. Then loads the spritesheet.
void openFile(sf::RenderWindow *window, sm::SpritesheetDisplayer *spritesheetDisplayer, std::vector<int> *selectedSprites){
    // Windows File Opening
    #ifdef __WIN32__
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
            LOG("Did not load.");
    #elif __linux__
        LOG("This is running on linux");
    #endif
}

// Open console window
inline void openConsole(){
    AllocConsole();
    freopen("CONOUT$", "w", stdout);
    //freopen("CONIN$", "r", stdin); // No reason yet to have input in the console.
    LOG("Sprite Sheet Manipulator Started.\nGCC Version: " << __VERSION__);
}

int main(int argc, char* argv[])
{
    // Open console if DEBUG on.
    #ifdef DEBUG
        openConsole();
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
                case sf::Keyboard::Num6:
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
        // Draw white board.
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
