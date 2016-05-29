#include "Mouse.hpp"

#include "Logger.hpp"

namespace sm
{
    Mouse::Mouse(SpritesheetDisplayer *spritesheetDisplayer, sf::RenderWindow *window)
    {
        this->window = window;
        this->spritesheetDisplayer = spritesheetDisplayer;
        this->view = spritesheetDisplayer->getView();

        // Initialize mouseRectangle, which is the graphic display of the selection.
        mouseRectangle.setOutlineColor(sf::Color::Red);
        mouseRectangle.setOutlineThickness(2);
        mouseRectangle.setFillColor(sf::Color::Transparent);

        // Damn GCC with it's boolean = true default.
        transparentMode = false;
        selecting = false;
    }

    Mouse::~Mouse()
    {
        //dtor
    }

    /**
     *  Returns the mouse position relative to the spritesheet's view.
     */
    sf::Vector2f Mouse::getPositionRelativeToView(){
        sf::Vector2f viewPos;
        viewPos.x = view->getCenter().x - view->getSize().x/2;
        viewPos.y = view->getCenter().y - view->getSize().y/2;
        // Return the mouse position after calculations.
        return sf::Vector2f(sf::Mouse::getPosition(*window).x + viewPos.x - (window->getSize().x*view->getViewportPosition().x),
            sf::Mouse::getPosition(*window).y + viewPos.y - (window->getSize().y*view->getViewportPosition().y));
    }

    /**
     *  If the center of a sprite is inside the selection it collides.
     *
     *  @param  i   Index of the sprite to check collision.
     *  @return Returns true if it's colliding, false if it's not.
     */
    bool Mouse::isSelectionColliding(int i)
    {
        // Sprite positions
        float xSprite = spritesheetDisplayer->getSpritesheet()->getSpriteCenter(i).x,
            ySprite = spritesheetDisplayer->getSpritesheet()->getSpriteCenter(i).y;
        // If Sprite's center is inside mouse selection, it's colliding. The OR operation is to check for both sides of selections.
        if (((mousePreviousPosition.x < xSprite && getPositionRelativeToView().x > xSprite)
            || (mousePreviousPosition.x > xSprite && getPositionRelativeToView().x < xSprite))
            && ((mousePreviousPosition.y < ySprite && getPositionRelativeToView().y > ySprite)
            || (mousePreviousPosition.y > ySprite && getPositionRelativeToView().y < ySprite)))
            return true;
        return false;
    }

    /**
     *  Checks if the position given is inside the spritesheet.
     *  TODO: Put this in Spritesheet
     */
     bool Mouse::isInsideSpritesheet(sf::Vector2i position)
     {
        if ((position.x >= 0 &&
            position.y >= 0) &&
            (position.x < spritesheetDisplayer->getSpritesheet()->getImage()->getSize().x &&
            position.y <  spritesheetDisplayer->getSpritesheet()->getImage()->getSize().y) )
            return true;
        else
            return false;
     }

    /**
     *  Checks if the position given is inside the view.
     *  TODO: Put this in SpritesheetView
     */
    bool Mouse::isInsideView(sf::Vector2i position)
    {
        LOG(view->getViewportWindowEndPosition().x << " " << position.x);
        LOG(view->getViewportWindowEndPosition().y << " " << position.y);
        if ((position.x >= view->getViewportWindowPosition().x &&
            position.y >= view->getViewportWindowPosition().y) &&
            (position.x < view->getViewportWindowEndPosition().x &&
            position.y < view->getViewportWindowEndPosition().y))
            return true;
        else
            return false;
    }

    /**
     *  What happens when the mouse is pressed.
     */
    void Mouse::mousePressed(){
        // Only work if there's a spritesheet loaded.
        if (spritesheetDisplayer->getSpritesheet() != nullptr){
            // Calculate clicked position
            sf::Vector2i clickedPosition = static_cast<sf::Vector2i>(getPositionRelativeToView());

            // Check if transparentMode is true then select alpha layer color
            if (transparentMode)
            {
                // Check if position is inside spritesheet and the view.
                if (isInsideSpritesheet(clickedPosition) && isInsideView(sf::Mouse::getPosition(*window)))
                {
                    spritesheetDisplayer->setTransparent(clickedPosition);
                    transparentMode = false;
                }
            }
            // Check if clickedPosition is inside the View.
            else if (isInsideView(sf::Mouse::getPosition(*window)))
            {
                selecting = true;
                mousePreviousPosition = getPositionRelativeToView();
            }
        }
    }

    /**
     *  What happens when the mouse is released.
     */
    void Mouse::mouseReleased(){
        // Only work if there's a spritesheet loaded.
        if (spritesheetDisplayer->getSpritesheet() != nullptr){
            // If there currently is a selection.
            if (selecting){
                // Reset selecting bool
                selecting = false;
                // Reset selected sprites.
                selectedSprites.clear();
                for (int i = 0; i < spritesheetDisplayer->getSpritesheet()->getNumberOfSprites(); i++){
                    if (isSelectionColliding(i))
                    {
                        // Set as the currently selected sprites
                        selectedSprites.push_back(i);
                    }
                }
            }
        }
    }

    /**
     *  Draw square to display where the mouse is going to select.
     */
    void Mouse::drawSelection(){
        // Only work if there's a spritesheet loaded.
        if (spritesheetDisplayer->getSpritesheet() != nullptr){
            // If there currently is a selection.
            if (selecting)
            {
                mouseRectangle.setPosition(mousePreviousPosition);
                mouseRectangle.setSize(static_cast<sf::Vector2f>(getPositionRelativeToView() - mousePreviousPosition));
                window->draw(mouseRectangle);
            }
        }
    }

    /**
     *  Switches whether or not the user is on transparent color picking mode.
     */
    void Mouse::switchTransparentMode(){
        if (transparentMode)
            transparentMode = false;
        else
            transparentMode = true;
        LOG("Mouse: Transparent Mode switched " << transparentMode);
    }
}
