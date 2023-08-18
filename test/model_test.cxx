#include "model.hxx"
#include <player.hxx>
#include <catch.hxx>

TEST_CASE("player wraps around")
{
    Model m;
    m.title = false;
    m.player.pos.x = 2;
    m.player.move_left();
    m.on_frame(0.02);
    CHECK(m.player.pos.x > 350);

    m.player.pos.x = 353;
    m.player.move_right();
    m.on_frame(0.02);
    CHECK(m.player.pos.x < 5);
}

TEST_CASE("player hits wall")
{
    Model m;
    m.title = false;
    m.player.pos = {10,100};

    m.player.walls.push_back({0,110,20,10});

    m.on_frame(0.02);
    CHECK_FALSE(m.death);

    // have to reset the y pos of the player, because on_frame() updates it
    // (to something around 58) in the cosine function (to more accurately
    // emulate the hit boxes when converting from the raw hit box game to the
    // actual game). If we ignored this, (i.e. commented out line 103 in
    // player.cxx), then we wouldn't have to reset the y pos every time we
    // call on_frame.
    m.player.pos.y = 100;
    m.on_frame(0.02);
    CHECK(m.death);
}

// alternatively, we could use very specific numbers:
// (this took way too long to get right)
TEST_CASE("player hits wall - no reset y pos")
{
    Model m;
    m.title = false;
    m.player.pos.x = 15;
    m.player.walls.push_back({0,67,30,10});

    m.on_frame(0.02);
    CHECK_FALSE(m.death);

    m.on_frame(0.02);
    CHECK(m.death);
}


TEST_CASE("player slides off of wall")
{
    Model m;
    m.title = false;
    // player is 8 by 8, so the wall is right next to the player
    m.player.pos = {0,100};
    m.player.walls.push_back({8,100,20,30});

    m.player.move_right();
    m.on_frame(0.02);

    CHECK_FALSE(m.death);

    int currentxpos = m.player.pos.x;

    m.player.pos.y = 100;
    m.on_frame(0.02);
    CHECK(currentxpos == m.player.pos.x);

    m.player.pos.y = 100;
    m.on_frame(0.02);
    CHECK(currentxpos == m.player.pos.x);
}

TEST_CASE("pause works")
{
    Model m;
    m.title = false;
    m.player.pos.x = 10;

    m.pause();
    m.player.move_right();
    m.on_frame(0.02);

    CHECK(m.player.pos.x == 10);


    m.player.move_left();
    m.on_frame(0.02);

    CHECK(m.player.pos.x == 10);

    m.pause();

    m.player.move_left();
    m.on_frame(0.02);
    CHECK_FALSE(m.player.pos.x == 10);

}


TEST_CASE("player slides off of wall wrapped around")
{
    Model m;
    m.title = false;
    m.player.pos.x = 2;// player is 8x8

    m.player.walls.push_back({300,40,60,100}); // really tall wall
    m.player.move_left();
    m.on_frame(0.02);

    CHECK_FALSE(m.death);

    int currentxpos = m.player.pos.x;

    m.player.move_left();

    m.on_frame(0.02);
    CHECK(currentxpos == m.player.pos.x);

    m.player.move_left();

    m.on_frame(0.02);
    CHECK(currentxpos == m.player.pos.x);
}

TEST_CASE("game speed works")
{
    Model m;
    m.title = false;
    m.player.index = 0;
    m.player.pos = {10,100};
    m.player.walls.push_back({0,112,20,50});

    m.on_frame(0.02);
    CHECK_FALSE(m.death);

    m.player.pos.y = 100;
    m.on_frame(0.02);
    CHECK_FALSE(m.death);


    m.resetplayer(2);
    m.player.index = 7;
    m.player.pos = {10,100};
    m.player.walls.push_back({0,112,20,50});

    m.on_frame(0.02);
    CHECK_FALSE(m.death);

    m.player.pos.y = 100;
    m.on_frame(0.02);
    CHECK(m.death);
}