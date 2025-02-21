#pragma once

#include <stdafx.h>
#include <stdlib.h>
#include <time.h>

using namespace cv;

class CGameObject {
protected:
    Point2i _position; ///< The position of the game object on the screen
    Point2f _velocity; ///< The instantaneous velocity of the object
    int _radius; ///< The radius of the circle to draw as the object
    int _lives; ///< How many hits the object can take before dying
    Scalar _colour; ///< The colour of the object
public:
    /**
     * @brief Updates/moves the object based on the given velocity
     * 
     */
    virtual void move();

    /**
     * @brief Sees if the object has collided with another object
     * 
     * @param obj The object that this object may have collided with
     * @return true if a collision has occured
     * @return false otherwise
     */
    virtual bool collide(CGameObject &obj);

    /**
     * @brief Sees if the object has collided with the wall
     * 
     * @param board The size of the canvas or area it is moving around on
     * @return true if a collision with the wall has occured
     * @return false otherwise
     */
    virtual bool collide_wall(Size board);

    /**
     * @brief What to do when a hit to the object occurs
     * 
     */
    virtual void hit();
    
    /**
     * @brief Get the amount of lives remaining for this object
     * 
     * @return int The amount of lives left (>=0)
     */
    int get_lives() { return _lives; }

    /**
     * @brief Set the amount of lives remaining for this object
     * 
     * @param lives The new amount of lives
     */
    void set_lives(int lives) { _lives = lives; }

    /**
     * @brief Sets a new position for this object
     * 
     * @param pos The new position
     */
    void set_pos(Point2f pos) { _position = pos; }

    /**
     * @brief Gets the current position of this object
     * 
     * @return Point2i The current position
     */
    Point2i get_pos() { return _position; }
    
    /**
     * @brief Gets the current radius of this object
     * 
     * @return int The radius
     */
    int get_radius() { return _radius; }

    /**
     * @brief Draws this object on the screen
     * 
     * @param im The cv::Mat to draw the object on
     */
    virtual void draw(Mat &im);
    };
    