#ifndef MOUSE_H
#define MOUSE_H

#include "SpritesheetDisplayer.h"
#include "SpritesheetView.h"

/**
 *  This class manages mouse selection related to the program.
 */
namespace sm
{
    class Mouse
    {
        public:
            Mouse(SpritesheetDisplayer *spritesheetDisplayer, sf::RenderWindow *window);
            virtual ~Mouse();

            // Gets & Sets
            void setTransparentMode(bool transparentMode){this->transparentMode = transparentMode;}
            std::vector<int>* getSelectedSprites(){return &selectedSprites;}

            // Methods
            void mousePressed();
            void mouseReleased();
            void drawSelection();
            void switchTransparentMode();

        protected:
        private:
            SpritesheetDisplayer *spritesheetDisplayer;
            sf::RenderWindow *window;
            sm::SpritesheetView *view;
            sf::Vector2f mousePreviousPosition;
            sf::RectangleShape mouseRectangle;
            bool selecting,
                transparentMode;
            std::vector<int> selectedSprites;

            // Methods
            bool isSelectionColliding(int i);
            sf::Vector2f getPositionRelativeToView();
            bool isInsideView(sf::Vector2i position);
            bool isInsideSpritesheet(sf::Vector2i position);
    };
}

#endif // MOUSE_H
