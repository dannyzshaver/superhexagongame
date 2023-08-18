#pragma once

#include "model.hxx"
#include "view.hxx"

#include <ge211.hxx>

class Controller : public ge211::Abstract_game
{
public:
    Controller();

protected:
    void draw(ge211::Sprite_set& set) override;
    void on_key_down(ge211::Key) override;
    void on_key_up(ge211::Key) override;
    void on_frame(double dt) override;
    void on_mouse_up(ge211::Mouse_button, ge211::Posn<int>) override;
    void on_mouse_down(ge211::Mouse_button, ge211::Posn<int>) override;
    void on_mouse_move(ge211::Posn<int>) override;

private:
    Model model_;
    View view_;
    double elapsed_time_1;
    double elapsed_time_2;
    double elapsed_time_3;
};
