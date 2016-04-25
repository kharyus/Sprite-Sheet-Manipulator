#include "Spritesheet.h"

namespace sm{

    Spritesheet::Spritesheet(std::string pathToFile)
    {
        loadImage(pathToFile);

        // Default Flag value
        selectUnitedSprite = false;
    }

    Spritesheet::~Spritesheet()
    {
        //dtor

    }

    /**
     *  Verifies if the 2 colors are equal
     */
    bool Spritesheet::isColorEqual(const sf::Color &color1, const sf::Color &color2){
        if(color1.r == color2.r && color1.g == color2.g && color1.b == color2.b && color1.a == color2.a)
            return true;
        return false;
    }

    /**
     *  Calculates how much is the offset of this byte.
     */
    int Spritesheet::byteOffset(const int x, const int y, const int width, const int bytesPerPixel){
        return ((y*width) + x)*bytesPerPixel;
    }

    /**
     *  Calculates how much is the offset of this pixel.
     */
    int Spritesheet::pixelOffset(const int x, const int y, const int width, const int bytesPerPixel){
        return byteOffset(x, y, width, bytesPerPixel)/bytesPerPixel;
    }

    /**
     *  Function based on the flood fill algorithm. It detects the pixels nearby that are not the transparent color and saves them to arrays.
     *
     *  This method used to be recursive, but would case stack overflow in big files.
     *
     *  @param  x           The current horizontal position.
     *  @param  y           The current vertical position.
     *  @param  pixels      The array of pixels in this image.
     *  @param  searched    An array containing a bool to each pixel, to check if it has been searched before.
     */
    void Spritesheet::loopFill(int x, int y, const sf::Color &transparentColor, const sf::Uint8 *pixels, bool *searched)
    {
        sf::Color color;
        sf::Vector2f highest, lowest;
        lowest.x = x; lowest.y = y;
        // Add current pixel to the stack
        pixelStack.push_back(sf::Vector2i(x,y) );
        do {
            int currentx,currenty;
            currentx = pixelStack.back().x;
            currenty = pixelStack.back().y;
            pixelStack.pop_back();
            int offset = byteOffset(currentx,currenty,sheetImage.getSize().x,4);
            color.r = pixels[offset+0];
            color.g = pixels[offset+1];
            color.b = pixels[offset+2];
            color.a = pixels[offset+3];
            // If color in this pixel is transparent go to the next iteration.
            if(isColorEqual(transparentColor,color) || searched[offset/4]) continue;
            searched[offset/4] = true;
            // If current x and y are higher or lower than the highest and lowest, update dimensions of the sprite.
            // TODO: Turn those into functions.
            if (currentx > highest.x)
                highest.x = currentx;
            if (currentx < lowest.x)
                lowest.x = currentx;
            if (currenty > highest.y)
                highest.y = currenty;
            if (currenty < lowest.y)
                lowest.y = currenty;
            //sheetImage.setPixel(x,y,color.Red); // To change color.
            //Add the adjacent pixels to the stack.
            if (currenty > 0 && !searched[pixelOffset(currentx,currenty-1,sheetImage.getSize().x,4)]) //north
                pixelStack.push_back(sf::Vector2i(currentx,currenty-1));
            if (currenty > 0 && currentx < sheetImage.getSize().x-1 && !searched[pixelOffset(currentx+1,currenty-1,sheetImage.getSize().x,4)]) //northeast
                pixelStack.push_back(sf::Vector2i(currentx+1,currenty-1));
            if (currentx < sheetImage.getSize().x-1 && !searched[pixelOffset(currentx+1,currenty,sheetImage.getSize().x,4)]) //east
                pixelStack.push_back(sf::Vector2i(currentx+1,currenty));
            if (currentx < sheetImage.getSize().x-1 && currenty < sheetImage.getSize().y-1 && !searched[pixelOffset(currentx+1,currenty+1,sheetImage.getSize().x,4)]) //southeast
                pixelStack.push_back(sf::Vector2i(currentx+1,currenty+1));
            if (currenty < sheetImage.getSize().y-1 && !searched[pixelOffset(currentx,currenty+1,sheetImage.getSize().x,4)]) //south
                pixelStack.push_back(sf::Vector2i(currentx,currenty+1));
            if (currentx > 0 && currenty < sheetImage.getSize().y-1 && !searched[pixelOffset(currentx-1,currenty+1,sheetImage.getSize().x,4)]) //southwest
                pixelStack.push_back(sf::Vector2i(currentx-1,currenty+1));
            if (currentx > 0 && !searched[pixelOffset(currentx-1,currenty,sheetImage.getSize().x,4)]) //west
                pixelStack.push_back(sf::Vector2i(currentx-1,currenty));
            if (currentx > 0 && currenty > 0 && !searched[pixelOffset(currentx-1,currenty-1,sheetImage.getSize().x,4)]) //northwest
                pixelStack.push_back(sf::Vector2i(currentx-1,currenty-1));
        } while(!pixelStack.empty());

        // Add 1 to highest values to have the last row and column of pixels also added to the sprite.
        highest.x++;
        highest.y++;

        // Push the boundaries of the current sprite into the array.
        startBoundaries.push_back(lowest);
        endBoundaries.push_back(highest);
    }

    /**
     *  Flow through each individual pixel to find the sprite's boundaries.
     */
    void Spritesheet::flowThroughPixels(const sf::Color &transparentColor, const sf::Uint8 *pixels, bool *searched)
    {
        sf::Color color;
        for (int j=0;j<sheetImage.getSize().y;j++)
            for (int i=0;i<sheetImage.getSize().x;i++){
                int offset = ((j*sheetImage.getSize().x) + i)*4;
                color.r = pixels[offset+0];
                color.g = pixels[offset+1];
                color.b = pixels[offset+2];
                color.a = pixels[offset+3];
                if (!isColorEqual(transparentColor,color) && !searched[offset/4] ){
                    loopFill(i,j,transparentColor,pixels,searched);
                }
            }
    }

    /**
     *  Find the spritesheet's sprites and it's boundaries.
     */
    void Spritesheet::parseSpritesheet(bool createAlpha)
    {
        transparentColor = sheetImage.getPixel(0,0); // Pick first color as transparent
        // Image pixels
        const sf::Uint8 *pixels = sheetImage.getPixelsPtr();
        // Array of wether the pixel has been searched already for each pixel.
        bool *searched = new bool[sheetImage.getSize().x*sheetImage.getSize().y];
        // Initialize manually the searched array: each compiler starts with a different value.
        for (int i =0;i<sheetImage.getSize().x*sheetImage.getSize().y;i++){searched[i]=false;}
        // Flow through the pixels finding the sprites boundaries.
        flowThroughPixels(transparentColor,pixels,searched);
        // Free the searched array
        delete[] searched;
        // Applies transparency if createAlpha is true
        if (createAlpha)
            sheetImage.createMaskFromColor(transparentColor);
    }

    /**
     *  Loads image.
     */
    void Spritesheet::loadImage(std::string pathToFile, bool createAlpha){
        // Try to load image, if it does not load don't parse the spritesheet and don't set the texture and sprite.
        if (sheetImage.loadFromFile(pathToFile)){
            // Loaded successfully
            parseSpritesheet(createAlpha);
        }
        else {
            // Loading failed
            std::cout << "Spritesheet: Couldn't load the image." << std::endl;
        }
    }

    /**
     *  Save every single sprite on the sheet into separate files.
     */
    void Spritesheet::saveSheetToFiles(sf::Image *image){
        sf::Image singleSprite;
        int width, height;
        for (int i=0; i<startBoundaries.size(); i++) {
            std::stringstream outputPath;

            // Sets path of the file to be saved. Adds 1 to it to avoid using index 0.
            outputPath << "output/" << i+1 << ".png";
            width = endBoundaries.at(i).x - startBoundaries.at(i).x;
            height = endBoundaries.at(i).y - startBoundaries.at(i).y;
            singleSprite.create(width,height);
            singleSprite.copy(*image,0,0,sf::IntRect(startBoundaries.at(i).x,startBoundaries.at(i).y,width,height));
            if (!singleSprite.saveToFile(outputPath.str()))
                std::cout << "Failed to save file: ";
            std::cout << i+1 << " " << outputPath.str() << " " << width << " " << height << std::endl;
        }
    }

    /**
     *  Save only the selected sprites into separate files.
     */
    void Spritesheet::saveSelectedToFiles(const std::vector<int> *selectedSprites, sf::Image *image){
        sf::Image singleSprite;
        int width, height;
        for (int i=0;i<selectedSprites->size();i++) {
            std::stringstream outputPath;

            // Sets path of the file to be saved. Adds 1 to it to avoid using index 0.
            outputPath << "output/" << selectedSprites->at(i)+1 << ".png";
            width = endBoundaries.at(selectedSprites->at(i)).x - startBoundaries.at(selectedSprites->at(i)).x;
            height = endBoundaries.at(selectedSprites->at(i)).y - startBoundaries.at(selectedSprites->at(i)).y;
            singleSprite.create(width,height);
            singleSprite.copy(*image,0,0,sf::IntRect(startBoundaries.at(selectedSprites->at(i)).x,startBoundaries.at(selectedSprites->at(i)).y,width,height));
            if (!singleSprite.saveToFile(outputPath.str()))
                std::cout << "Failed to save file: ";
            std::cout << (selectedSprites->at(i)+1) << " " << outputPath.str() << " " << width << " " << height << std::endl;
        }
    }

    /**
     *  Save sprites into separate files. If 1 or more sprites are currently selected, save them, otherwise save all.
     */
    void Spritesheet::saveToFiles(const std::vector<int> *selectedSprites, sf::Image *image){
        // Check if there is any sprite selected
        if (selectedSprites != nullptr){
            if (selectedSprites->size() > 0)
                saveSelectedToFiles(selectedSprites, image);
            else
                saveSheetToFiles(image);
        }
    }

    /**
     *  TODO: Save a .xml/.json/.txt or something else with the positions, sizes and origins(if on) of the sprites.
     */
    void Spritesheet::saveCoordinatesToTxt(std::string path)
    {

    }

    /**
     *  Removes the sprite on number i from the files to be saved.
     */
    void Spritesheet::removeSingleSprite(int i){
        startBoundaries.erase(startBoundaries.begin()+i);
        endBoundaries.erase(endBoundaries.begin()+i);
    }

    /**
     *  Removes the selected sprites from the files to be saved.
     */
    void Spritesheet::removeSprites(std::vector<int> *selectedSprites){
        std::cout << "Removed Sprites Total: " << selectedSprites->size() << std::endl;
        if (selectedSprites->size() > 0){
            for (int i = 0; i < selectedSprites->size(); i++){
                // Erase the rectangle at the beginning of the array + the index of the selected sprite.
                // Then you remove the number of times it has iterated because it's changing the index each iteration.
                removeSingleSprite(selectedSprites->at(i)-i);
                std::cout << "Removed sprite: " << selectedSprites->at(i)-i << std::endl;
            }
            selectedSprites->clear();
        }
    }

    /**
     *  Unite sprites by verifying the biggest and smallest of it's boundaries, and deleting the ones selected.
     *  Sets the biggest and smallest parameters to the values calculated.
     *
     *  @param selectedSprites  Sprites currently selected.
     *  @param biggest          The biggest value, it's passed by reference to acess the value from outside the class.
     *  @param smallest         The smallest value, it's passed by reference to acess the value from outside the class.
     */
    void Spritesheet::uniteSprites(std::vector<int> *selectedSprites){
        std::cout << "United Sprites Total: " << selectedSprites->size() << std::endl;
        if (selectedSprites->size() > 1){
            // Verify and hold the smallest and biggest positions
            // Note: There is a catch in the implementation here, since every time the loop runs a position is deleted the vector index changes, so we need to subtract 1 for each iteration.
            // TODO: Fix the relationship between rectangles and this class.
            sf::Vector2f biggest,
                smallest;
            smallest.x = 999999; smallest.y = 999999;
            for (int i = 0; i < selectedSprites->size(); i++){
                if (startBoundaries.at(selectedSprites->at(i)-i).x < smallest.x)
                    smallest.x = startBoundaries.at(selectedSprites->at(i)-i).x;
                if (startBoundaries.at(selectedSprites->at(i)-i).y < smallest.y)
                    smallest.y = startBoundaries.at(selectedSprites->at(i)-i).y;
                if (endBoundaries.at(selectedSprites->at(i)-i).x > biggest.x )
                    biggest.x = endBoundaries.at(selectedSprites->at(i)-i).x;
                if (endBoundaries.at(selectedSprites->at(i)-i).y > biggest.y )
                    biggest.y = endBoundaries.at(selectedSprites->at(i)-i).y;
                removeSingleSprite(selectedSprites->at(i)-i);
                std::cout << "United sprite: " << selectedSprites->at(i)-i << std::endl;
            }
            startBoundaries.push_back(smallest);
            endBoundaries.push_back(biggest);
            selectedSprites->clear();
            // If flag on, select united sprite.
            if (selectUnitedSprite)
                selectedSprites->push_back(startBoundaries.size()-1);
        }
    }
}
