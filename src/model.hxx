#pragma once

#include <ge211.hxx>
#include "player.hxx"

class Model
{
    /////////////////////////////////////////
    // constructor:

public:
    Model();

    /////////////////////////////////////////
    // member functions:
    void on_frame(double dt);

    // sets paused to true if it was false, and to false if it was true
    // called by on_key_down when you press the space bar
    void pause();

    // resets the player to the title screen if state is 1, and back to the
    // game if state is 2
    void resetplayer(int state);

    //////////////////////////////////////
    // members:
    Player player;
    bool title; // true when on title screen, false when in game
    bool death; // true when you hit an obstacle
    bool paused; // explained in pause()

    // the current position of the mouse, only tracked on the title screen
    // and the death screen
    ge211::Posn<int> mousepos;

    // score gets +='d dt, except when on the title screen, dead, or paused
    float score;

    // when you die, the score is stored into highscore if it's higher than
    // the current high score.
    float highscore;

    // draw position of the background on the title screen, slowly moves left
    // when called in on_frame in controller.cxx
    int bgpos;
};

