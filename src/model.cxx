#include "model.hxx"


// constructor:
Model::Model()
    : player(),
      title(true),
      death(false),
      paused(false),
      mousepos(0,0),
      score(0),
      highscore(0),
      bgpos(0)
{}

// member functions:
void
Model::on_frame(double dt) {

    // if you're in game
    if (!title && !paused) {
        // loop over every wall
        for (auto wall : player.walls) {

            // if you currently intersect with a wall, you die
            if (player.would_crash(wall) == 1) {
                death = true;
                if (highscore <= score) {
                    highscore = score;
                }
                return;
            // if you intersect with a wall on the y level, and you would
            // fully intersect the wall on the next updated position, you slide
            } else if (player.would_crash(wall) == 2 &&
            player.next(dt).would_crash(wall) == 1) {

                // only update the walls, not the player
                player.update_walls(dt);

                // have to update the score too, because you'd skip it otherwise
                if (!death && !paused) {
                    score += dt;
                }
                return;
            }
        }
        // if you're not dead nor sliding, then update the score, the player,
        // and the walls
        if (!death && !paused) {
            score += dt;
        }
        player = player.next(dt);
        player.update_walls(dt);
    }

}

void Model::pause() {
    if (paused) {
        paused = false;
    } else {
        paused = true;
    }
}

void
Model::resetplayer(int state) {
    // back to title screen
    if (state == 1) {
        player = Player();
        death = false;
        paused = false;
        title = true;
        score = 0;
    // back to game
    } else if (state == 2) {
        player = Player();
        death = false;
        paused = false;
        score = 0;
    }
}