#include "SpritesheetView.hpp"

#include "Logger.hpp"

namespace sm
{
    SpritesheetView::SpritesheetView(sf::RenderWindow *window, sf::Vector2f viewportPosition, sf::Vector2f viewportSize) : sf::View()
    {
        this->window = window;

        // Initialize Previous size
        previousSize = sf::Vector2f(window->getSize().x,window->getSize().y);

        // Construct View
        setSize(window->getSize().x*viewportSize.x, window->getSize().y*viewportSize.y);
        // You can't set the view's position through a setPosition function, so you have to calculate the center by using half of the size.
        setCenter((window->getSize().x/2)*viewportSize.x, (window->getSize().y/2)*viewportSize.y);
        setViewport(sf::FloatRect(viewportPosition.x,viewportPosition.y,viewportSize.x,viewportSize.y));
    }

    SpritesheetView::~SpritesheetView()
    {
        //dtor
    }

    void SpritesheetView::resizeView(sf::Vector2f newSize)
    {
        // Resize view and move to adjust to new size.
        setSize(newSize.x * getViewport().width, newSize.y * getViewport().height);
        move((newSize.x - previousSize.x)*getViewport().width/2, (newSize.y - previousSize.y)*getViewport().height/2);

        // Update previous size
        previousSize.x = newSize.x;
        previousSize.y = newSize.y;

        // Update View
        window->setView(*this);

        // Debug
        LOG("SpritesheetView: View resized");
    }

}
