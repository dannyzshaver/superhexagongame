#include "view.hxx"
#include <iomanip>




View::View(Model const& model)
        : model_(model),
          player_sprite({8,8},{255,0,0}),
          wall_sprite({60,30},{255,255,255}),
          dot(1,{0,0,255}),
          center_sprite(52),
          line_sprite({800,3}),
          pause_block({800,400},{100,100,100,200}),
          bg_sprite1({800,300},{195,195,195}),
          bg_sprite2({800,300},{88,88,88}),
          titlebg_sprite1({800,600},{88,88,88}),
          title_button_sprite({542,190},{180,180,180,180}),
          death_button_sprite({460,106},{180,180,180,180}),

          player_s("player.png"),
          hexagon("r2.png"),
          block_bottom1("blockbottom1.png"),
          block_top1("blocktop1.png"),
          block_bottom2("blockbottom2.png"),
          block_top2("blocktop2.png"),
          L60("l60.png"),
          R60("r60.png"),
          L240("l240.png"),
          R240("r240.png"),
          wall120("wall120.png"),
          wall180("wall180.png"),
          playgamebutton("playgamebutton.png"),
          playgamebuttonhighlight("playgamebuttonhighlight.png"),
          titletext("titletext.png"),
          deathtext("deathtext.png"),
          deathtexthighlight("deathtexthighlight.png"),
          titlebg("titlebg.png")

{}

void
View::draw(ge211::Sprite_set& set)
{
    if (model_.title) {
        set.add_sprite(titletext,{0,0},2);
        set.add_sprite(playgamebutton,{120,230},2);
        set.add_sprite(title_button_sprite,{127,237},2);
        set.add_sprite(titlebg,{model_.bgpos,0},0);
        set.add_sprite(titlebg,{model_.bgpos + 800,0},0);

        ge211::Text_sprite::Builder builder1(sans30);
        builder1.color({255, 0, 0});
        builder1 << std::fixed << std::showpoint;
        builder1 << std::setprecision(2)<< "high score: " << model_.highscore;
        pause_text_sprite2.reconfigure(builder1);
        set.add_sprite(pause_text_sprite2, {280, 480}, 3);

        if (overbutton(model_.mousepos,1)) {
            set.add_sprite(playgamebuttonhighlight,{115,223},3);
        }
    }


    // UNCOMMENT THIS IF STATEMENT BELOW TO SEE THE ACTUAL HIT BOX
    // REPRESENTATION OF THE GAME.

    // Note: the player will move all wavy, which is expected, because of
    // line 103 in player.cxx (the cosine function). Comment that out too if
    // you want.

    ////////////////////////////////////////////////
/*
    if (!model_.title) {
        set.add_sprite(player_sprite, {model_.player.pos.x+220,model_.player.pos
                                                                     .y},
                       56);
        set.add_sprite(line_sprite, {180,300},56,ge211::Transform::rotation
                (90));
        set.add_sprite(line_sprite, {-180,300},56,ge211::Transform::rotation
                (90));
        for (auto wall : model_.player.walls) {
            set.add_sprite(wall_sprite, {wall.x+220, wall.y},56);
        }
    }
*/
    //////////////////////////////////////////////////

    // YOU MUST ALSO COMMENT OUT THE IF STATEMENT BELOW, WHICH DRAWS
    // EVERYTHING ALL PRETTY.
    // An easy way to do this is to right-click on the
    // "if", then go to Folding->Collapse, and then do some /* and */ around
    // that.

    if (!model_.title) {

        // draws the pause screen
        if (model_.paused) {
            // draws the text
            ge211::Text_sprite::Builder builder1(sans90);
            builder1.color({255, 0, 0});
            builder1 << "game paused";
            pause_text_sprite1.reconfigure(builder1);
            set.add_sprite(pause_text_sprite1, {120, 200}, 25);
            ge211::Text_sprite::Builder builder2(sans30);
            builder2.color({255, 0, 0});
            builder2 << "press space to unpause";
            pause_text_sprite2.reconfigure(builder2);
            set.add_sprite(pause_text_sprite2, {240, 350}, 25);

            // draws a little transparent block because it's pretty
            set.add_sprite(pause_block, {0, 100}, 24);
        }

        // draws the stopwatch/score
        ge211::Text_sprite::Builder builder3(sans30);
        builder3.color({255, 0, 0});
        builder3 << std::fixed << std::showpoint;
        score_sprite.reconfigure(builder3);

        // if you're dead, you're going to do a lot of things
        if (model_.death) {
            // draw your score
            set.add_sprite(deathtext,{0,0},30);
            builder3 << "score: "<< std::setprecision(2) << model_.score
                     << " seconds";
            score_sprite.reconfigure(builder3);
            set.add_sprite(score_sprite, {260, 150}, 25);

            // draw the death screen
            titlebg_sprite1.recolor({88,88,88,150});
            set.add_sprite(titlebg_sprite1,{0,0},24);
            // draw the two buttons (retry and title screen)
            set.add_sprite(death_button_sprite,{175,262},24);
            set.add_sprite(death_button_sprite,{180,422},24);

            // draw the little red highlights over the buttons
            if (overbutton(model_.mousepos,2)) {
                set.add_sprite(deathtexthighlight,{162,249},31);
            } else if (overbutton(model_.mousepos,3)) {
                set.add_sprite(deathtexthighlight,{163,410},31);
            }
        } else {
            // else, you're not dead, so just draw the score in the top right
            builder3 << std::setprecision(2) << model_.score;
            score_sprite.reconfigure(builder3);
            set.add_sprite(score_sprite, {700, 20}, 25);
        }


        // draw the center hexagon and the 6 lines that come out of each vertex
        set.add_sprite(hexagon, {400 - 52, 300 - 45}, 20);
        set.add_sprite(line_sprite, {0, 300}, 10);
        set.add_sprite(line_sprite,
                       {0, 300},
                       10,
                       ge211::Transform::rotation(60));
        set.add_sprite(line_sprite,
                       {0, 300},
                       10,
                       ge211::Transform::rotation(-60));

        // alternate the colors of the background
        int t = model_.score;
        if (t % 2 == 1) {
            set.add_sprite(block_bottom2, {225, 304}, 5);
            set.add_sprite(block_top1, {225, -5}, 5);
            set.add_sprite(bg_sprite1, {0, 0}, 2);
            set.add_sprite(bg_sprite2, {0, 300}, 2);
        } else {
            set.add_sprite(block_bottom1, {225, 304}, 5);
            set.add_sprite(block_top2, {225, -5}, 5);
            set.add_sprite(bg_sprite2, {0, 0}, 2);
            set.add_sprite(bg_sprite1, {0, 300}, 2);
        }

        // draw the player
        // convert hit box pos to circle pos
        ge211::Posn<int> circlepos = model_.player.x_to_posn();
        // get the normal angle
        float drawangle = model_.player.posn_to_angle(circlepos);
        // get the drawing position (the center of the screen minus half the
        // width and height)
        ge211::Posn<int>
                drawpos = {circlepos.x + 400 - 4, circlepos.y + 300 - 4};

        // and draw it with the draw position and normal angle
        set.add_sprite(player_s, drawpos,
                       11, ge211::Transform::rotation(drawangle));



        // walls are complicated. the top and bottom walls (x = 60 and 240) are
        // simply rectangle sprites, whereas the other four walls are image
        // sprites. We loop over every wall in walls, and have if elses for
        // each x position {0,60,120,180,240,300}:
        for (auto wall: model_.player.walls) {
            // if you're inside the hexagon or outside the screen, don't
            // draw it
            if (wall.y > 15 && wall.y < 500) {

                // bottom and top walls first:
                // convert the y position to a float, because we're doing math
                float wall_y = wall.y;
                // we want the x position to basically go along the line
                // coming out from the hexagon, given by:
                int xpos = (700 - wall_y) / tan(M_PI / 3);

                if (wall.x == 60) {
                    // as we slide the bottom wall to the top right, we scale
                    // it by its y position/53 to perfectly shrink it along
                    // the right line coming out of the hexagon.
                    set.add_sprite(wall_sprite, {xpos - 3, wall.y + 300}, 6,
                                   ge211::Transform::scale_x(wall_y / 53));

                    // also draw little corner pieces so that it looks seamless
                    set.add_sprite(L60, {xpos - 20, wall.y + 300}, 11);
                    set.add_sprite(R60, {800 - 12 - xpos, wall.y + 300}, 11);
                } else if (wall.x == 240) {
                    // do the same for the top walls.
                    set.add_sprite(wall_sprite,
                                   {xpos - 3, -wall.y + 300 - wall.height},
                                   6,
                                   ge211::Transform::scale_x(wall_y / 53));
                    set.add_sprite(L240,
                                   {xpos - 21, -wall.y + 300 - wall.height},
                                   11);
                    set.add_sprite(R240,
                                   {800 - 11 - xpos, -wall.y + 300 - wall
                                   .height},
                                   11);
                // the side walls are a little harder. Take a look in
                // Resources/ to see what they look like. Basically, we slide
                // these diagonal walls along the horizontal line coming out
                // from the hexagon, and with some clever layering, we can
                // layer "blockbottom.png" and "blocktop.png" over the part
                // that sticks out.
                } else if (wall.x == 120) {
                    xpos = wall_y / sin(M_PI / 3);
                    set.add_sprite(wall120, {402 - wall.height - xpos, 300}, 4);
                    set.add_sprite(L240, {395 - wall.height - xpos, 302}, 11);
                } else if (wall.x == 180) {
                    xpos = wall_y / sin(M_PI / 3);
                    set.add_sprite(wall180, {402 - wall.height - xpos,-112},
                                   4);
                    set.add_sprite(L60, {395 - wall.height - xpos, 270}, 11);
                } else if (wall.x == 300) {
                    xpos = wall_y / sin(M_PI / 3);
                    set.add_sprite(wall120, {203 - wall.height + xpos, -112},
                                   4);
                    set.add_sprite(R60, {404 + xpos, 270}, 11);
                } else if (wall.x == 0) {
                    xpos = wall_y / sin(M_PI / 3);
                    set.add_sprite(wall180, {203 - wall.height + xpos, 300}, 4);
                    set.add_sprite(R240, {403 + xpos, 302}, 11);
                }
            }
        }
    }

    // Above is the end of the big if statement, if you want to comment it out.


}


// detects whether a given position is within a certain area, for detections
// of buttons.
bool View::overbutton(ge211::Posn<int> posn, int buttonnum) {
    // start game button
    // 127,237 top left : 669, 427 bottom right
    if (buttonnum == 1) {
        if (posn.x > 126 && posn.x < 670 && posn.y > 237 && posn.y < 428) {
            return true;
        } else {
            return false;
        }
        // retry button
        // 173,261 : 636,369
    } else if (buttonnum == 2) {
        if (posn.x > 173 && posn.x < 636 && posn.y > 261 && posn.y < 369) {
            return true;
        } else {
            return false;
        }
        // back to menu button
        // 179,421 : 641,529
    } else  if (buttonnum == 3){
        if (posn.x > 179 && posn.x < 641 && posn.y > 421 && posn.y < 529) {
            return true;
        } else {
            return false;
        }
    } else return false;
}
