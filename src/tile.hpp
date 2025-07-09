#include <SFML/Graphics.hpp>
#include <map>

enum TileId { BACKGROUND = 0, WALL };

std::vector<std::string> split(std::string s, std::string delimiter);

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
  static void
  save(std::map<std::pair<float, float>, std::vector<TileId>> chunks,
       const std::filesystem::path &fp);
  static std::optional<Chunk> load(sf::RenderWindow *win, int x, int y,
                                   const std::filesystem::path &fp);
  Chunk() {}
  Chunk(sf::RenderWindow *window, sf::Vector2f pos, std::vector<TileId> ids);
  bool operator==(const Chunk &other) const;
  void render();
};

struct ChunkHash {
  size_t operator()(const Chunk &c) const;
};