#include "SpritesheetDisplayer.h"

namespace sm
{
    SpritesheetDisplayer::SpritesheetDisplayer(sf::RenderWindow *window, bool showNumbers) :
        spritesheetView(window, sf::Vector2f(0.01, 0.01), sf::Vector2f(0.68, 0.98))
    {
        // Initialize pointers to null
        spritesheet = nullptr;
        sheetTexture = nullptr;
        sheetSprite = nullptr;

        // Init general vars
        this->showNumbers = showNumbers;
        this->window = window;

        // Init Rectangle
        rectangle.setOutlineColor(sf::Color::Green);
        rectangle.setOutlineThickness(1);
        rectangle.setFillColor(sf::Color::Transparent);

        // Init Rectangle Number
        font.loadFromFile("fonts/sansation.ttf");
        rectangleNumber.setFont(font);
        rectangleNumber.setScale(0.4,0.4);
        rectangleNumber.setColor(sf::Color::Black);

        // Init Black Board
        blackBoard.setPosition(spritesheetView.getViewportWindowPosition());
        blackBoard.setSize(spritesheetView.getViewportWindowSize());
        blackBoard.setOutlineThickness(3);
        blackBoard.setOutlineColor(sf::Color::Black);
        blackBoard.setFillColor(sf::Color::White);
    }

    SpritesheetDisplayer::~SpritesheetDisplayer()
    {
        // If pointers have values on them, delete them.
        if (spritesheet != nullptr)
            delete spritesheet;
        if (sheetTexture != nullptr)
            delete sheetTexture;
        if (sheetSprite != nullptr)
            delete sheetSprite;
    }

    /**
     *  Draw rectangles in the sprites boundaries.
     */
    void SpritesheetDisplayer::drawRectangles(std::vector<int> *selectedSprites){
        // Reset color to green.
        sf::Color color;
        rectangle.setOutlineColor(color.Green);
        // Iterate through sprites, drawing rectangles on their boundaries
        for (int i=0; i<spritesheet->getStartBoundaries().size(); i++){
            rectangle.setPosition(spritesheet->getStartBoundaries().at(i));
            rectangle.setSize(spritesheet->getSpriteSize(i));
            window->draw(rectangle);
            // If true Display Numbers on top
            if (showNumbers){
                rectangleNumber.setPosition(spritesheet->getStartBoundaries().at(i));
                rectangleNumber.setString(static_cast<std::ostringstream*>( &(std::ostringstream() << i+1) )->str());
                window->draw(rectangleNumber);
            }
        }

        // Selected sprites drawing.
        // Draw a blue rectangle on top of the green one. This is a bad design, but it has better performance than checking the number multiple times in the first for loop.
        if (selectedSprites->size() > 0){
            rectangle.setOutlineColor(color.Blue);
            int index;
            for (int j = 0; j < selectedSprites->size(); j++){
                index = selectedSprites->at(j);
                rectangle.setPosition(spritesheet->getStartBoundaries().at(index));
                rectangle.setSize(spritesheet->getSpriteSize(index));
                window->draw(rectangle);
            }
        }
    }

    /**
     *  Draws the spritesheet and rectangles if there's a spritesheet loaded.
     */
    void SpritesheetDisplayer::draw(std::vector<int> *selectedSprites){
        if (spritesheet != nullptr){
            window->draw(*sheetSprite);
            drawRectangles(selectedSprites);
        }
    }

    /**
     *  Switches showNumbers flag.
     */
    void SpritesheetDisplayer::switchShowNumbers()
    {
        showNumbers = (showNumbers) ? false : true;
    }

    /**
     *  Receives a pixel position to use that pixel's color as transparent
     */
    void SpritesheetDisplayer::setTransparent(sf::Vector2i pixelPosition){
        reloadImage();
        editedSheetImage.createMaskFromColor(editedSheetImage.getPixel(pixelPosition.x, pixelPosition.y));
        reloadTexture();
        reloadSprite();

        std::cout << "SpritesheetDisplayer: alpha layer set." << std::endl;
    }

    /**
     *  TODO: Resets the sprites in the spritesheet into its original form.
     *  Since this can make user lose progress, also add an "are you sure" window.
     */
    void SpritesheetDisplayer::reset(){

    }

    /**
     *  Loads new spritesheet, deleting the last one's memory position.
     */
    void SpritesheetDisplayer::loadSpritesheet(std::string pathToFile, std::vector<int> *selectedSprites){
        // Check whether there's already a spritesheet
        if (spritesheet != nullptr){
            delete spritesheet;
        }
        spritesheet = new Spritesheet(pathToFile);

        // Reset view position
        spritesheetView.setCenter(spritesheetView.getSize().x/2, spritesheetView.getSize().y/2);

        // Reset selected sprites if its not null
        if (selectedSprites != nullptr)
            selectedSprites->clear();

        // Reload the image.
        reloadImage();
        // When loading a sprite, the first pixel is transparent.
        editedSheetImage.createMaskFromColor(spritesheet->getImage()->getPixel(0,0));
        // Reload texture and sprite
        reloadTexture();
        reloadSprite();
    }

    /**
     * Image reloading - if it had a value before, delete it.
     */
    void SpritesheetDisplayer::reloadImage(){
        editedSheetImage = *spritesheet->getImage();
    }

    /**
     * Texture reloading - if it had a value before, delete it.
     */
    void SpritesheetDisplayer::reloadTexture(){
        if (sheetTexture != nullptr)
            delete sheetTexture;
        sheetTexture = new sf::Texture();
        sheetTexture->loadFromImage(editedSheetImage);
    }

    /**
     * Sprite reloading - if it had a value before, delete it.
     */
    void SpritesheetDisplayer::reloadSprite(){
        if (sheetSprite != nullptr)
            delete sheetSprite;
        sheetSprite = new sf::Sprite();
        sheetSprite->setTexture(*sheetTexture);
    }

    /**
     *  Reloads image, texture and sprite
     */
    void SpritesheetDisplayer::reloadAll(){
        reloadImage();
        reloadTexture();
        reloadSprite();
    }

    /**
     *  Draw black board below the spritesheet.
     */
     void SpritesheetDisplayer::drawBoard(sf::View *currentView){
        window->setView(window->getDefaultView());
		window->draw(blackBoard);
        window->setView(*currentView);
     }
}
