#include "ui.hpp"

void GenericButton::init() { bool _ = font.openFromFile("./Roboto.ttf"); }
void GenericButton::update(double dt) {}
void GenericButton::render() {
  sf::RectangleShape button_bg(size);
  button_bg.setPosition(pos);
  button_bg.setFillColor(background);
  window->draw(button_bg);

  sf::Text button_fg(font, text, text_size);
  button_fg.setFillColor(foreground);
  button_fg.setPosition(pos +
                        sf::Vector2f(10, size.y / 2 - (float)text_size / 2));
  window->draw(button_fg);
}

void ProgressBar::init() {}
void ProgressBar::update(double dt) {
  if (is_timed) {
    auto progress = measure(finish, std::chrono::steady_clock::now());

    if (progress <= 0) {
      percent = 1;
    } else {
      percent = 1 - progress / duration;
    }
  }
}
void ProgressBar::render() {
  sf::RectangleShape bg(size);
  bg.setPosition(pos);
  bg.setFillColor(background);
  bg.setOutlineColor(background);
  bg.setOutlineThickness(2);

  window->draw(bg);

  sf::RectangleShape prog({size.x * percent, size.y});
  prog.setPosition(pos);
  prog.setFillColor(percent == 1 ? ready : loading);

  window->draw(prog);
}