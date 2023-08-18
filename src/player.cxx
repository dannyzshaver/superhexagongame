#include "player.hxx"
#include <math.h>

// Random sources for use in creating random walls
ge211::Random_source<int> two_sides(0,1);
ge211::Random_source<int> three_sides(0,2);
ge211::Random_source<int> six_sides(0,5);

// constructor
Player::Player()
        : pos {270,58},
          velocity(0),
          wall({0,0,60,30}),
          GAME_SPEED({200,260,266,271,285,290,370,400}),
          WALL_SET_SEPARATION({230,212,210,208,204,200,180,170}),
          WALL_SPEED_MULT({0.6,0.51,0.53,0.6,0.65,0.65,0.65,0.7}),
          RIGHT_SPEED_MULT({1.52,1.5,1.5,1.5,1.5,1.5,1.4,1.35}),
          index(0)
{
    generate_walls();
}


void
Player::wrap_around_right() {
    pos.x -= 360;
}
void Player::wrap_around_left() {
    pos.x += 360;
}
void Player::move_right() {
    velocity = GAME_SPEED[index]*RIGHT_SPEED_MULT[index];
}
void Player::move_left() {
    velocity = -GAME_SPEED[index];
}
void Player::stand_still() {
    velocity = 0;
}

int Player::moving() {
    if (velocity > 0) {
        return 1;
    } else if (velocity < 0) {
        return -1;
    } else {
        return 0;
    }
}

int
Player::would_crash(Wall wall1) {
    // if there's an intersection, return 1
    if ((wall1.x < pos.x + 8 && wall1.x + wall1.width > pos.x) &&
    (wall1.y < pos.y + 8 && wall1.y + wall1.height > pos.y)){
        return 1;
    // else there's no intersection, so:
    } else {
        // if there's an intersection on y levels only, return 2
        if (wall1.y < pos.y + 8) {
            return 2;
        // else return 0
        } else {
            return 0;
        }
    }
}


Position
Player::x_to_posn() const {
    // (0,0) is the center of the circle with radius 62
    // (62,0) -> x = 0 ; (-62,0) -> x = 180
    ge211::Posn<float> coords = {0,0};
    float angle = M_PI/180*(pos.x + 3);
    coords.x = cos(angle)*62;
    coords.y = sin(angle)*62;
    return coords.into<int>();
}


int
Player::posn_to_angle(Position posn) const
{
    ge211::Posn<float> posn1 = posn.into<float>();
    float angle = atan(posn1.y / posn1.x) * 180 / M_PI;
    if (posn.x > 0) {
        return angle + 90;
    } else {
        return angle - 90;
    }
}


Player
Player::next(double dt) const
{
    Player result(*this);
    result.pos.x += velocity*dt;

    // cosine function to more accurately represent the hit boxes in the
    // middle of the hexagon
    result.pos.y = 58+5*cos(M_PI/180*result.pos.x*6+M_PI);

    // changed from 360 -> 355 because it was drawing weird : there's no
    // functional difference
    if (result.pos.x > 355) {
        result.wrap_around_right();
    } else if (result.pos.x < 0) {
        result.wrap_around_left();
    }
    return result;
}


void
Player::update_walls(double dt) {
    // update wall
    wall.y -= GAME_SPEED[index]*WALL_SPEED_MULT[index] * dt;

    // update every wall in walls
    for (size_t ii = 0; ii < walls.size(); ii++) {
        if (walls[ii].y > 0) {
            walls[ii].y -= GAME_SPEED[index]*WALL_SPEED_MULT[index] * dt;
        }
    }
}


void
Player::remove_walls() {
        for (unsigned int ii = 0; ii < walls.size(); ii++) {
            if (walls[ii].y < 15) {
                // if the wall is past the player, replace it with the wall
                // in the back and pop that out
                walls[ii] = walls.back();
                walls.pop_back();
            }
        }
}


void
Player::generate_walls() {
    int random_num1 = three_sides.next(); //{0,2}
    int random_num2 = six_sides.next(); //{0,5}

    // if there's no walls, call startingwalls() and one transition set
    if (walls.empty()) {
        walls.push_back(wall);
        wall.y = 400;
        startingwalls();

        if (random_num1 == 0) {
            onehex();
        } else if (random_num1 == 1) {
            threeandone();
        } else {
            three();
        }
    }

    // call a random obstacle set
    if (random_num2 == 0) {
        manylittle();
    } else if (random_num2 == 1) {
        threehex();
    } else if (random_num2 == 2) {
        spiral();
    } else if (random_num2 == 3) {
        slide();
    } else if (random_num2 == 4) {
        tworthreel();
    } else {
        strafe();
    }

    // 1/3 chance to have another transition set
    random_num1 = three_sides.next(); //{0,2}
    if (random_num1 == 0) {
        random_num2 = six_sides.next(); //{0,5}
        if (random_num2 == 0) {
            onehex();
        } else if (random_num2 == 1) {
            threeandone();
        } else if (random_num2 == 2) {
            threethick();
        } else if (random_num2 == 3) {
            twothick();
        } else if (random_num2 == 4) {
            threedouble();
        } else {
            twodouble();
        }
    }
}


///////////////////////////
// starting wall set
void
Player::startingwalls(void)
{
    // two then three
    int random_num1 = three_sides.next();//{0,2}
    wall.x = random_num1*60;
    for (int jj = 0; jj < 2; jj++) {
        walls.push_back(wall);
        wall.x += 180;
        wall.x %= 360;

    }
    wall.y += WALL_SET_SEPARATION[index];
    int random_num2 = two_sides.next(); // {0,1}
    wall.x = random_num2*60;
    for (int jj = 0; jj < 3; jj++) {
        walls.push_back(wall);
        wall.x += 120;
        wall.x %= 360;
    }
    wall.y += WALL_SET_SEPARATION[index];
}

///////////////////////////
// transition wall sets
void
Player::onehex(void)
{   // could happen at start
    int random_num = six_sides.next(); // {0,5}
    wall.x = random_num * 60;
    for (int ii = 0; ii < 5; ii++) {
        walls.push_back(wall);
        wall.x = (wall.x + 60) % 360;
    }
    wall.y += WALL_SET_SEPARATION[index];
}

void
Player::threeandone(void)
{
    int random_num = six_sides.next(); // {0,5}
    wall.x = random_num*60;
    for (int jj = 0; jj < 3; jj++) {
        walls.push_back(wall);
        wall.x += 60;
        wall.x %= 360;
    }
    wall.x += 60;
    wall.x %= 360;
    walls.push_back(wall);
    wall.y += WALL_SET_SEPARATION[index];
}

void
Player::three(void)
{
    int random_num = two_sides.next(); // {0,1}
    wall.x = random_num*60;
    for (int jj = 0; jj < 3; jj++) {
        walls.push_back(wall);
        wall.x += 120;
        wall.x %= 360;
    }
    wall.y += WALL_SET_SEPARATION[index];
}

void
Player::threethick(void)
{
   int random_num = two_sides.next(); // {0,1}
    wall.x = random_num*60;
    for (int jj = 0; jj < 3; jj++) {
        for (int ii = 0; ii < 2; ii++) {
            walls.push_back(wall);
            wall.y += 29;
        }
        wall.y -= 2*29;
        wall.x += 120;
        wall.x %= 360;
    }
    wall.y += WALL_SET_SEPARATION[index];
}

void
Player::twothick(void)
{
    int random_num = two_sides.next(); // {0,2}
    wall.x = random_num*60;
    for (int jj = 0; jj < 2; jj++) {
        for (int ii = 0; ii < 2; ii++) {
            walls.push_back(wall);
            wall.y += 29;
        }
        wall.y -= 2*29;
        wall.x += 180;
        wall.x %= 360;
    }
    wall.y += WALL_SET_SEPARATION[index];
}

void
Player::threedouble(void)
{
    int random_num = two_sides.next(); // {0,1}
    wall.x = random_num*60;
    for (int jj = 0; jj < 3; jj++) {
        for (int ii = 0; ii < 2; ii++) {
            walls.push_back(wall);
            wall.y += 60;
        }
        wall.y -= 2*60;
        wall.x += 120;
        wall.x %= 360;
    }
    wall.y += WALL_SET_SEPARATION[index];
}

void
Player::twodouble(void)
{
    int random_num = three_sides.next(); // {0,2}
    wall.x = random_num*60;
    for (int jj = 0; jj < 2; jj++) {
        for (int ii = 0; ii < 2; ii++) {
            walls.push_back(wall);
            wall.y += 60;
        }
        wall.y -= 2*60;
        wall.x += 180;
        wall.x %= 360;
    }
    wall.y += WALL_SET_SEPARATION[index];
}


/////////////////////////////
// real obstacles
void
Player::manylittle(void)
{
    int random_num = two_sides.next(); //{0,1}
    wall.x = random_num*60;
    for (int jj = 0; jj < 5; jj++) {
        for (int ii = 0; ii < 6; ii++) {
            if ((jj + ii) % 2 == 0) {
                walls.push_back(wall);
            }
            wall.x += 60;
            wall.x %= 360;
        }
        wall.x = random_num*60;
        wall.y += WALL_SET_SEPARATION[index]/2;
    }
    wall.y += WALL_SET_SEPARATION[index]-100;

}

void
Player::threehex(void)
{
    int random_num = six_sides.next(); // {0,5}
    for (int jj = 0; jj < 3; jj++) {
        wall.x = (random_num*60 + (jj%2)*180) % 360;
        for (int ii = 0; ii < 5; ii++) {
            walls.push_back(wall);
            wall.x = (wall.x+60) % 360;
        }
        wall.x = 0;
        wall.y += WALL_SET_SEPARATION[index];
    }

}

void
Player::spiral(void)
{
    int random_num = three_sides.next(); // {0,2}
    wall.x = random_num*60;
    for (int jj = 0; jj < 9; jj++) {
        for (int ii = 0; ii < 2; ii++) {
            for (int kk = 0; kk < 2; kk++) {
                walls.push_back(wall);
                wall.y += 28;
            }
            wall.y -= 28*2;

            if (jj == 0) {
                wall.x += 60;
                wall.x %= 360;
                for (int kk = 0; kk < 2; kk++) {
                    walls.push_back(wall);
                    wall.y += 28;
                }
                wall.y -= 28*2;
                wall.x += 360 - 60;
                wall.x %= 360;
            }
            if (jj == 8) {
                wall.x += 300;
                wall.x %= 360;
                for (int kk = 0; kk < 2; kk++) {
                    walls.push_back(wall);
                    wall.y += 28;
                }
                wall.y -= 28*2;
                wall.x += 360 - 300;
                wall.x %= 360;
            }
            wall.x += 180;
            wall.x %= 360;
        }
        wall.x += 60;
        wall.x %= 360;
        wall.y += 28*2;
    }
    wall.y += WALL_SET_SEPARATION[index]-28*2;

}

void
Player::slide(void)
{
    wall.y += WALL_SET_SEPARATION[index]/2;
    int random_num = six_sides.next(); // {0,5}
    wall.x = random_num*60;
    for (int jj = 0; jj < 3; jj++) {
        for (int ii = 0; ii < 5; ii++) {
            walls.push_back(wall);
            if (jj % 2 == 0) {
                wall.x += 60;
            } else {
                wall.x += 300;
            }
            wall.x %= 360;
        }
        wall.x = random_num*60;
        if (jj < 2) {
            for (int ii = 0; ii < WALL_SET_SEPARATION[index]/28 + 3; ii++) {
                walls.push_back(wall);
                wall.y += 28;
            }
        }
    }
    wall.y += WALL_SET_SEPARATION[index];
}

void
Player::tworthreel(void)
{
    int random_num = three_sides.next(); // {0,2}
    wall.x = random_num*60;
    for (int jj = 0; jj < 6; jj++) {
        for (int ii = 0; ii < 2; ii++) {
            for (int kk = 0; kk < 2; kk++) {
                walls.push_back(wall);
                wall.x += 60;
                wall.x %= 360;
            }
            wall.x += 60;
            wall.x %= 360;
        }
        if (jj < 2) {
            wall.x += 60;
        } else {
            wall.x += 300;
        }

        wall.x %= 360;
        wall.y += WALL_SET_SEPARATION[index]/1.8;
    }
    wall.y += WALL_SET_SEPARATION[index]-100;
}

void
Player::strafe(void)
{
    int random_num = six_sides.next(); // {0,5}
    wall.x = random_num*60;
    for (int jj = 0; jj < 5; jj++) {
        walls.push_back(wall);
        wall.x += 60;
        wall.x %= 360;
    }
    wall.y += WALL_SET_SEPARATION[index]/1.3;
    wall.x += 300;
    wall.x %= 360;
    for (int jj = 0; jj < 3; jj++) {
        for (int kk = 0; kk < 2; kk++) {
            walls.push_back(wall);
            wall.y += 28;
        }
        wall.y -= 28*2;
        wall.x += 60;
        wall.x %= 360;
    }
    wall.y += 28*2;
    for (int jj = 0; jj < 5; jj++) {
        for (int kk = 0; kk < 2; kk++) {
            walls.push_back(wall);
            wall.y += 28;
        }
        wall.y -= 28*2;
        wall.x += 300;
        wall.x %= 360;
    }
    wall.y += 120;
    for (int kk = 0; kk < 2; kk++) {
        walls.push_back(wall);
        wall.y += 28;
    }
    wall.x += 300;
    wall.x %= 360;
    for (int jj = 0; jj < 3; jj++) {
        for (int kk = 0; kk < 2; kk++) {
            walls.push_back(wall);
            wall.y += 28;
        }
        wall.y -= 28*2;
        wall.x += 60;
        wall.x %= 360;
    }
    wall.x += 60;
    wall.x %= 360;
    for (int jj = 0; jj < 3; jj++) {
        for (int kk = 0; kk < 2; kk++) {
            walls.push_back(wall);
            wall.y += 28;
        }
        wall.y -= 28*2;
        wall.x += 120;
        wall.x %= 360;
    }

    for (int kk = 0; kk < 6; kk++) {
        walls.push_back(wall);
        wall.y -= 28;
    }
    wall.y += WALL_SET_SEPARATION[index] + 28*6;
}