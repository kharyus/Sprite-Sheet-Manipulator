#ifndef SPRITESHEETDISPLAYER_H
#define SPRITESHEETDISPLAYER_H

#include "Spritesheet.h"
#include "SpritesheetView.h"

/**
 *  This Class manages everything related to displaying the spritesheet on the window.
 *  TODO: Add selectedSprites pointer that is passed to the constructor.
 */
namespace sm
{
    class SpritesheetDisplayer
    {
        public:
            SpritesheetDisplayer(sf::RenderWindow *window, bool showNumbers = true);
            virtual ~SpritesheetDisplayer();

            // Gets & Sets
            void setShowNumbers(bool showNumbers){this->showNumbers = showNumbers;}
            const sf::Image& getImage(){return editedSheetImage;}
            const sf::Texture* getTexture(){return sheetTexture;}
            const sf::Sprite* getSprite(){return sheetSprite;}
            Spritesheet* getSpritesheet(){return spritesheet;}
            sm::SpritesheetView* getView(){return &spritesheetView;}

            // Methods
            void switchShowNumbers();
            void setTransparent(sf::Vector2i pixelPosition);
            void draw(std::vector<int> *selectedSprites);
            void reset();
            void loadSpritesheet(std::string pathToFile, std::vector<int> *selectedSprites = nullptr);
            void saveToFiles(std::vector<int> *selectedSprites){ if (spritesheet != nullptr) spritesheet->saveToFiles(selectedSprites, spritesheet->getImage());}
            void removeSprites(std::vector<int> *selectedSprites){spritesheet->removeSprites(selectedSprites);}
            void uniteSprites(std::vector<int> *selectedSprites){spritesheet->uniteSprites(selectedSprites);}
            void drawBoard(sf::View *currentView);

        protected:
        private:
            Spritesheet *spritesheet;
            sf::RenderWindow *window;
            sf::Image editedSheetImage;
            sf::Texture *sheetTexture;
            sf::Sprite *sheetSprite;
            std::vector<sf::RectangleShape> rectangles;
            sf::RectangleShape rectangle;
            sf::Font font;
            sf::Text rectangleNumber;
            sf::RectangleShape blackBoard;
            SpritesheetView spritesheetView;
            bool showNumbers;

            // Methods
            void drawRectangles(std::vector<int> *selectedSprites);
            void reloadImage();
            void reloadTexture();
            void reloadSprite();
            void reloadAll();
    };
}

#endif // SPRITESHEETDISPLAYER_H
