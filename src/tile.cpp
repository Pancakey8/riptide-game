#include "tile.hpp"

bool Tile::does_collide(sf::Vector2f opos, sf::Vector2f osize) {
  return pos.x < opos.x + osize.x && pos.x + size.x > opos.x &&
         pos.y < opos.y + osize.y && pos.y + size.y > opos.y;
}

void Tile::render() {
  sf::RectangleShape rect{size};
  rect.setPosition(pos);
  rect.setFillColor(sf::Color::Yellow);
  window->draw(rect);
}