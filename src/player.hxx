#pragma once

#include <ge211.hxx>

using Wall = ge211::Rect<int>;
using Position = ge211::Posn<int>;

// For reference: the "player" is the little red triangle you control, and a
// "wall" is the while obstacle coming at you from any one of the six sides
// of the hexagon.

// Although the game looks like everything is coming at the player from 6
// different directions, the physics are actually very simple. The player is
// just a 8x8 square, that has an x and a y position. When you press the
// left arrow, it decreases the x pos, and when you press the right arrow, it
// increases the x pos. This x pos goes from 0 to 360, and the player can wrap
// around the sides. Now, the walls that come at the player are modeled as 60
// by 30 Rects. By locking the x position of these walls to one of {0,60,120,
// 180,240,300}, you essentially get your six sides of the hexagon. Now, all you
// have to do is decrease the y position of these walls at a constant rate,
// and suddenly you have your super hexagon game (or, at least, the hit boxes
// of your game). Then, with some fancy math, and some ugly looking things in
// View::draw(), you can draw everything like it's coming in from six
// different directions. But the core of the game itself is very simple, just
// dodge the obstacles coming at you from above (below, actually, because y =
// 0 is the top of the screen). You can choose to uncomment things in
// View::draw() to actually SEE the hit box representation, if you want.

struct Player
{
    ////////////////////////////////////////////
    // constructor:
    Player();

    ////////////////////////////////////////////
    // member functions:

    // updates the player's x position to wrap around the right side
    void wrap_around_right();
    // updates the player's x position to wrap around the left side
    void wrap_around_left();

    // sets the players velocity to a positive number, negative number, or
    // zero, respectively
    void move_right();
    void move_left();
    void stand_still();

    // moving() returns -1 if moving left, 0 if standing still, and 1 if moving
    // right. It's used in the controller so that if you're holding the right
    // arrow key, and you press the left arrow key, the player moves to the
    // left even though you're now holding down both arrow keys. This is the
    // most intuitive way to move the player around.
    int moving();

    // Returns 1 if there is an intersection of the player and the given wall.
    // Returns 2 if there is an intersection only on the y levels (meaning
    // the wall is next to the player)
    // Else, returns 0
    // Needs three different outputs for use in detecting whether the player
    // should slide off of the wall or crash from hitting the wall face on.
    // see on_frame() in model.cxx to see how this works
    int would_crash(Wall wall1);


    // converts the x position of the player to the position it is drawn at in
    // view, using trig
    Position x_to_posn() const;

    // takes this new position, and returns the direction of the normal
    // vector, to make the player always point outward from the center of the
    // circle.
    int posn_to_angle(Position posn) const;

    // updates the position of the player based on its velocity and based on
    // whether it will wrap around or not.
    Player next(double dt) const;

    // iterates through every wall in walls and updates their y position
    // Also updates wall (the member of player, see line 128)
    void update_walls(double dt);

    // uses the trick from HW4 to swap every wall that doesn't matter anymore
    // (y pos is less than 15) with the last wall in the walls vector and
    // then pop_back it.
    void remove_walls();

    // Randomly calls  one of the obstacle sets below to add a bunch of walls
    // (in a certain pattern) to the walls vector. It has a 1/3 chance to
    // also add a transition set after the obstacle.
    // Also, if walls is empty, it will call startingwalls() as well. This
    // way, the game always starts in the same pattern, although with a
    // random orientation.
    void generate_walls();

    // wall sets:
    void startingwalls();

    // transition sets
    void onehex();
    void threeandone();
    void three();
    void threethick();
    void twothick();
    void threedouble();
    void twodouble();

    // obstacle sets
    void manylittle();
    void threehex();
    void spiral();
    void slide();
    void tworthreel();
    void strafe();


    ////////////////////////////////////////////
    // members:

    // The player's position is modeled as x,y, where x can go from [0,360] and
    // y is always around 58
    Position pos;

    // The velocity only updates the players x position when it is non-zero
    int velocity;

    // wall is a Rect that is updated/changed, then added to the vector walls
    // to create wall sets. It is updated by the update_walls function, along
    // with every Wall in walls. However, every time generate_walls() is
    // called, it's y value gets moved up as a whole new wall set is made, so
    // it never actually reaches the player. (even though it's invisible and
    // doesn't get drawn).
    Wall wall;

    // The walls vector contains every wall that approaches the player. Every
    // wall in the vector is drawn in the view, and updated by update_walls()
    // every frame. Periodically, in the controller, generate_walls() is
    // called and adds more walls to the walls vector. When a wall in the
    // walls vector moves into the hexagon, i.e. moves past the player, it is
    // removed by swapping with the last wall in the vector and pop(ing it)
    // _out. This is done by remove_walls(), called in the controller at the
    // same time as generate_walls(). This way, walls never gets too big, and
    // contains only about 50-200 walls at a time
    std::vector<Wall> walls;

    // GAME_SPEED, WALL_SET_SEPARATION, WALL_SPEED_MULT, and RIGHT_SPEED_MULT
    // are all related. Because the speed didn't scale linearly I hard-coded
    // 8 different combinations of values for GAME_SPEED,
    // WALL_SET_SEPARATION, and WALL_SPEED_MULT. This way, every 10 seconds
    // the game could just update an index for each vector and move to the
    // next "difficulty".

    // You can see the values of each vector in the constructor in player.cxx.

    std::vector<int> GAME_SPEED;
    std::vector<int> WALL_SET_SEPARATION;
    std::vector<double> WALL_SPEED_MULT;
    std::vector<double> RIGHT_SPEED_MULT;

    // And index is the index for each vector.
    int index;

};