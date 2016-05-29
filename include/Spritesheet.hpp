#ifndef SPRITESHEET_HPP
#define SPRITESHEET_HPP

#include <SFML/Graphics.hpp>
#include <sstream>

/**
 *  This class manages the core functionality of the program.
 *  Parsing the spritesheet, saving it to different files and everything related.
 */
namespace sm
{
    class Spritesheet
    {
        public:
            Spritesheet(std::string pathToFile);
            virtual ~Spritesheet();

            // Gets and Sets
            void setTransparentColor(sf::Color transparentColor){this->transparentColor = transparentColor;}
            sf::Image* getImage(){return &sheetImage;}
            const std::vector<sf::Vector2f>& getStartBoundaries(){return startBoundaries;}
            const std::vector<sf::Vector2f>& getEndBoundaries(){return endBoundaries;}
            int getNumberOfSprites(){return startBoundaries.size();}

            /**
             *  Returns the size of the specified sprite.
             *
             *  @param  i   Position of sprite in array.
             */
            sf::Vector2f getSpriteSize(int i){return endBoundaries.at(i) - startBoundaries.at(i);}

            /**
             *  Returns the center of the specified sprite.
             *
             *  @param  i   Position of sprite in array.
             */
            sf::Vector2f getSpriteCenter(int i){return sf::Vector2f((endBoundaries.at(i).x - startBoundaries.at(i).x)/2 + startBoundaries.at(i).x,
                                                                    (endBoundaries.at(i).y - startBoundaries.at(i).y)/2 + startBoundaries.at(i).y);}

            // Public Methods
            void loadImage(std::string pathToFile, bool createAlpha = false);
            void saveToFiles(const std::vector<int> *selectedSprites, sf::Image *image);
            void saveCoordinatesToTxt(std::string path);
            void removeSprites(std::vector<int> *selectedSprites);
            void uniteSprites(std::vector<int> *selectedSprites);

        private:
            // Private Variables
            sf::Image sheetImage;
            sf::Color transparentColor;
            std::vector<sf::Vector2i> pixelStack;
            std::vector<sf::Vector2f> startBoundaries, endBoundaries;
            // Flags
            bool selectUnitedSprite;

            // Private Methods
            bool isColorEqual(const sf::Color &color1, const sf::Color &color2);
            int byteOffset(const int x, const int y, const int width, const int bytesPerPixel);
            int pixelOffset(const int x, const int y, const int width, const int bytesPerPixel);
            void loopFill(int x, int y, const sf::Color &transparentColor, const sf::Uint8 *pixels, bool *searched);
            void flowThroughPixels(const sf::Color &transparentColor, const sf::Uint8 *pixels, bool *searched);
            void saveSheetToFiles(sf::Image *image);
            void saveSelectedToFiles(const std::vector<int> *selectedSprites, sf::Image *image);
            void removeSingleSprite(int i);
            void parseSpritesheet(bool createAlpha = false);
    };
}

#endif // SPRITESHEET_HPP
