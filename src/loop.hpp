#include "player.hpp"
#include <SFML/Graphics.hpp>

class Loop {
public:
  sf::RenderWindow *window;
  Camera camera{};
  Player player{};
  std::vector<Tile> tiles;
  Loop(sf::RenderWindow *window);
  void init();
  void update(double dt);
  void render();
};