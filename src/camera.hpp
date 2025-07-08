#include "tile.hpp"
#include "time.hpp"
#include <SFML/Graphics.hpp>

class Camera {
public:
  sf::Vector2f center{400, 300};
  sf::RenderWindow *window;
  sf::Vector2f cursor_last;
  Time last_update;
  bool need_change = false;
  float scale = 1;
  bool middle_button = false;
  bool is_control_flipped = true;
  Camera();
  void set();
  void move(sf::Vector2f change);
  void zoom(int ticks);
  void update(double dt);
  sf::Vector2f get_cursor();
};