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