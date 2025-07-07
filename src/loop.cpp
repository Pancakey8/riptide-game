#include "loop.hpp"
#include <math.h>

void Loop::init() {
  player.camera = &camera;
  player.tiles = &tiles;
  for (int i = 0; i < 10; ++i) {
    Tile t;
    t.pos = {(float)100 * i, 100};
    t.window = window;
    tiles.push_back(t);
  }
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
  for (auto tile : tiles) {
    tile.render();
  }
  window->display();
}

Loop::Loop(sf::RenderWindow *window) {
  this->window = window;
  this->player.window = window;
  this->camera.window = window;
}