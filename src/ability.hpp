#include "time.hpp"
#include <SFML/Graphics.hpp>

class Ability {
public:
  double cooldown = 0;
  Time start = std::chrono::steady_clock::now();
  sf::RenderWindow *window;

  Ability(sf::RenderWindow *window) : window(window) {}
  bool is_alive();
  virtual void update(double dt) = 0;
  virtual void render() = 0;
};

class Projectile : public Ability {
public:
  sf::Vector2f pos;
  sf::Vector2f size{30, 30};
  sf::Vector2f direction;
  float velo = 150;

  Projectile(sf::RenderWindow *window) : Ability(window) {}
  void update(double dt) override;
  void render() override;
};