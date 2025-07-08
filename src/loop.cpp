#include "loop.hpp"
#include <math.h>

void Loop::init() {
  player.camera = &camera;
  player.update_tiles();
  window->setMouseCursorVisible(false);
}
void Loop::update(double dt) {
  player.update(dt);
  camera.update(dt);
}
void Loop::render() {
  window->clear(sf::Color::White);
  player.render();
  sf::CircleShape cursor{10};
  cursor.setFillColor(sf::Color::Black);
  cursor.setPosition(camera.get_cursor() - sf::Vector2f{10, 10});
  window->draw(cursor);
  window->display();
}

Loop::Loop(sf::RenderWindow *window) {
  this->window = window;
  this->player.window = window;
  this->camera.window = window;
}