#include "tile.hpp"
#include <cmath>

bool Tile::does_collide(sf::Vector2f opos, sf::Vector2f osize) {
  return pos.x < opos.x + osize.x && pos.x + TILE_SIZE[0] > opos.x &&
         pos.y < opos.y + osize.y && pos.y + TILE_SIZE[1] > opos.y;
}

void Tile::render() {
  sf::RectangleShape rect{{TILE_SIZE[0], TILE_SIZE[1]}};
  rect.setPosition(pos);
  if (id == WALL)
    rect.setFillColor(sf::Color::Yellow);
  rect.setOutlineThickness(1);
  rect.setOutlineColor(sf::Color::Red);
  window->draw(rect);
}

sf::Vector2f Chunk::nearest(sf::Vector2f point) {
  return {std::floor(point.x / TILE_SIZE[0] / CHUNK_SIZE[0]),
          std::floor(point.y / TILE_SIZE[1] / CHUNK_SIZE[1])};
}

Chunk::Chunk(sf::RenderWindow *window, sf::Vector2f pos,
             std::vector<TileId> ids) {
  this->pos = pos;
  std::vector<Tile> tiles;

  for (int row = 0; row < CHUNK_SIZE[1]; ++row) {
    for (int col = 0; col < CHUNK_SIZE[0]; ++col) {
      TileId id = ids[row * CHUNK_SIZE[1] + col];
      if (id) {
        Tile t;
        t.window = window;
        t.id = id;
        t.pos = {pos.x * CHUNK_SIZE[0] * TILE_SIZE[0] + col * TILE_SIZE[0],
                 pos.y * CHUNK_SIZE[1] * TILE_SIZE[1] + row * TILE_SIZE[1]};
        tiles.push_back(t);
      }
    }
  }

  this->tiles = tiles;
  this->window = window;
}

void Chunk::render() {
  for (auto tile : tiles) {
    tile.render();
  }
}

bool Chunk::operator==(const Chunk &other) const { return pos == other.pos; }
size_t ChunkHash::operator()(const Chunk &c) const {
  return std::hash<float>{}(c.pos.x) ^ (std::hash<float>{}(c.pos.y) << 1);
}