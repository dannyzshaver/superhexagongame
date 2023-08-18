#include "controller.hxx"

Controller::Controller()
        : model_(),
          view_(model_),
          elapsed_time_1(0),
          elapsed_time_2(0),
          elapsed_time_3(0)
{}



// q quits the game, a/left arrow moves the player left, and d/right arrow
// moves the player right, and the space bar pauses the game
void
Controller::on_key_down(ge211::Key key)
{
    if (key == ge211::Key::code('q')) {
        quit();
    } else if (key == ge211::Key::code('d') || key == ge211::Key::right()) {
        model_.player.move_right();
    } else if (key == ge211::Key::code('a') || key == ge211::Key::left()) {
        model_.player.move_left();
    } else if (key == ge211::Key::code(' ')) {
        model_.pause();
    }
}

// Only stop moving if you release the key you're moving in the direction of.
// Ex: if you're moving left while holding down both arrow keys, and you
// release the right arrow key, you aren't going to stop moving.
void
Controller::on_key_up(ge211::Key key)
{
    if (((model_.player.moving()==-1) && (key == ge211::Key::code('a')
    ||key == ge211::Key::left())) ||
        ((model_.player.moving()==1) && (key == ge211::Key::code('d')||key
        == ge211::Key::right()))) {
        model_.player.stand_still();
    }
}

// same thing as above but with mouse buttons
void
Controller::on_mouse_up(ge211::Mouse_button button, ge211::Posn<int>)
{
    if (((model_.player.moving()==-1) && (button==ge211::Mouse_button::left)) ||
        ((model_.player.moving()==1) && (button==ge211::Mouse_button::right))) {
        model_.player.stand_still();
    }

}


void
Controller::on_mouse_down(ge211::Mouse_button button, ge211::Posn<int> posn)
{
    if (!model_.title) {
        // move left and right if you're in the game state
        if (button == ge211::Mouse_button::right) {
            model_.player.move_right();
        } else {
            model_.player.move_left();
        }

        // do the right things if you click a button on the death screen
        // retry button
        if (model_.death && view_.overbutton(posn,2) && button ==
        ge211::Mouse_button::left) {
            model_.resetplayer(2);
            elapsed_time_3 = 0;
        // title screen button
        } else if (model_.death && view_.overbutton(posn,3) && button ==
        ge211::Mouse_button::left) {
            model_.resetplayer(1);
        }
    // do the right thing if you click the play game button on the title screen
    } else if (model_.title && view_.overbutton(posn,1) && button ==
    ge211::Mouse_button::left) {
        model_.title = false;
        elapsed_time_3 = 0;
    }
}

// store the position of the mouse whenever there could be buttons clicked
void
Controller::on_mouse_move(ge211::Posn<int> posn)
{
    if (model_.title || model_.death) {
        model_.mousepos = posn;
    }
}


void
Controller::on_frame(double dt)
{
    // make the scrolling background in the title screen seem infinite
    elapsed_time_1 += dt;
    if (elapsed_time_1 > 8/60) {
        elapsed_time_1 = 0.0;
        if (model_.title) {
            if (model_.bgpos < -800) {
                model_.bgpos += 800;
            }
            model_.bgpos -= 1;
        }
    }

    // every given period of time (based on the game speed), periodically
    // generate and remove walls from the walls vector
    elapsed_time_2 += dt;
    if (!model_.title && !model_.death && elapsed_time_2 > 700/model_.player
    .GAME_SPEED[model_.player.index]) {
        //int length = model_.player.walls.size();
        elapsed_time_2 = 0.0;
        model_.player.generate_walls();
        model_.player.remove_walls();
    }

    // every 10 seconds, increment the index to make the game faster/harder
    if (!model_.title && !model_.paused && !model_.death) {
        elapsed_time_3 += dt;
        if (elapsed_time_3 > 10) {
            elapsed_time_3 = 0;
            if(model_.player.index < 7) {
                model_.player.index++;
            }
        }

    }
    // also call the model on frame!
    model_.on_frame(dt);
}


void
Controller::draw(ge211::Sprite_set& set)
{
    view_.draw(set);
}
