#include <SFML/Graphics.hpp>

class Tile {
public:
  sf::Vector2f pos;
  sf::Vector2f size{90, 90};
  sf::RenderWindow *window;
  bool does_collide(sf::Vector2f pos, sf::Vector2f size);
  void render();
};