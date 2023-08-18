#pragma once

#include "model.hxx"

class View
{
public:
    explicit View(Model const& model);

    void draw(ge211::Sprite_set& set);

    bool overbutton(ge211::Posn<int> posn, int buttonnum);

private:
    Model const& model_;

    ge211::Font sans90{"sans.ttf", 90};
    ge211::Font sans30{"sans.ttf", 30};
    ge211::Text_sprite pause_text_sprite1;
    ge211::Text_sprite pause_text_sprite2;
    ge211::Text_sprite score_sprite;

    ge211::Rectangle_sprite const player_sprite;
    ge211::Rectangle_sprite const wall_sprite;
    ge211::Circle_sprite const dot;
    ge211::Circle_sprite const center_sprite;
    ge211::Rectangle_sprite const line_sprite;
    ge211::Rectangle_sprite const pause_block;
    ge211::Rectangle_sprite const bg_sprite1;
    ge211::Rectangle_sprite const bg_sprite2;
    ge211::Rectangle_sprite titlebg_sprite1;
    ge211::Rectangle_sprite const title_button_sprite;
    ge211::Rectangle_sprite const death_button_sprite;


    ge211::Image_sprite const player_s;
    ge211::Image_sprite const hexagon;
    ge211::Image_sprite const block_bottom1;
    ge211::Image_sprite const block_top1;
    ge211::Image_sprite const block_bottom2;
    ge211::Image_sprite const block_top2;
    ge211::Image_sprite const L60;
    ge211::Image_sprite const R60;
    ge211::Image_sprite const L240;
    ge211::Image_sprite const R240;
    ge211::Image_sprite const wall120;
    ge211::Image_sprite const wall180;
    ge211::Image_sprite const playgamebutton;
    ge211::Image_sprite const playgamebuttonhighlight;
    ge211::Image_sprite const titletext;
    ge211::Image_sprite const deathtext;
    ge211::Image_sprite const deathtexthighlight;
    ge211::Image_sprite const titlebg;


};
