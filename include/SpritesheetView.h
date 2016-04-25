#ifndef SPRITESHEETVIEW_H
#define SPRITESHEETVIEW_H

#include <SFML/Graphics.hpp>
#include <iostream>

/**
 *  This class is a child of View.
 *  It manages the specific behavior that a view needs in this particular program, and provides some helper functions that view doesn't have.
 */
namespace sm
{
    class SpritesheetView : public sf::View
    {
        public:
            SpritesheetView(sf::RenderWindow *window, sf::Vector2f viewportPosition, sf::Vector2f viewportSize);
            virtual ~SpritesheetView();

            // Methods
            sf::Vector2f getViewportPosition(){return sf::Vector2f(getViewport().left, getViewport().top);}
            sf::Vector2f getViewportWindowPosition(){return sf::Vector2f(getViewport().left * window->getSize().x, getViewport().top * window->getSize().y);}
            sf::Vector2f getViewportWindowSize(){return sf::Vector2f(getViewport().width * window->getSize().x, getViewport().height * window->getSize().y);}
            sf::Vector2f getViewportWindowEndPosition(){return getViewportWindowPosition() + getViewportWindowSize();}
            void resizeView(sf::Vector2f newSize);

        protected:
        private:
            sf::RenderWindow *window;
            sf::Vector2f previousSize;
            sf::Vector2f viewportPosition, viewportSize;

    };
}

#endif // SPRITESHEETVIEW_H
