#include "camera.hpp"
#include "tile.hpp"
#include "time.hpp"
#include <SFML/Graphics.hpp>

class Player {
public:
  float vel = 120;
  sf::Vector2f pos{50, 50};
  sf::Vector2f size{60, 60};
  sf::Vector2f target{0, 0};
  sf::Angle last_angle;
  std::vector<Tile> *tiles;
  Camera *camera;
  bool is_moving_to_target = false;
  Time last_dashed;
  double dash_duration = 0.2;
  double dash_cooldown = 2;
  float dash_distance = 3 * vel;
  sf::Vector2f dash_direction;
  sf::RenderWindow *window;
  Player() {}
  sf::Vector2f top_left();
  void update(double dt);
  void render();
};