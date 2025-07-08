#include <SFML/Graphics.hpp>

enum TileId { BACKGROUND = 0, WALL };

const static float TILE_SIZE[] = {90, 90};
const static float CHUNK_SIZE[] = {5, 5};

class Tile {
public:
  sf::Vector2f pos;
  TileId id;
  sf::RenderWindow *window;
  bool does_collide(sf::Vector2f pos, sf::Vector2f size);
  void render();
};

class Chunk {
public:
  sf::Vector2f pos;
  std::vector<Tile> tiles;
  sf::RenderWindow *window;

  static sf::Vector2f nearest(sf::Vector2f point);
  Chunk() {}
  Chunk(sf::RenderWindow *window, sf::Vector2f pos, std::vector<TileId> ids);
  bool operator==(const Chunk &other) const;
  void render();
};

struct ChunkHash {
  size_t operator()(const Chunk &c) const;
};