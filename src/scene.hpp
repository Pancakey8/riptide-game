#pragma once
#include <SFML/Graphics.hpp>

class Scene {
public:
  sf::RenderWindow *window;
  bool do_switch_scene = false;
  std::unique_ptr<Scene> next_scene = nullptr;
  Scene(sf::RenderWindow *window) : window(window) {}
  virtual void init() = 0;
  virtual void update(double dt) = 0;
  virtual void render() = 0;
  virtual void event(sf::Event event) = 0;
};