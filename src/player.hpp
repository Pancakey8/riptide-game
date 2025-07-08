#include "camera.hpp"
#include "time.hpp"
#include <SFML/Graphics.hpp>
#include <unordered_set>

class Player {
public:
  float vel = 120;
  sf::Vector2f pos{120, 120};
  sf::Vector2f size{60, 60};
  sf::Vector2f target{0, 0};
  sf::Angle last_angle;
  std::unordered_set<Chunk, ChunkHash> chunks;
  Camera *camera;
  bool is_moving_to_target = false;
  Time last_dashed;
  double dash_duration = 0.2;
  double dash_cooldown = 2;
  float dash_distance = 3 * vel;
  sf::Vector2f dash_direction;
  sf::RenderWindow *window;
  sf::Vector2f last_chunk{0, 0};
  Player() {}
  sf::Vector2f top_left();
  void update_tiles();
  void update(double dt);
  void render();
};