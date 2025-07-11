#pragma once
#include "ui.hpp"
#include <SFML/Graphics.hpp>

class Scene {
public:
  sf::RenderWindow *window;
  bool do_switch_scene = false;
  std::unique_ptr<Scene> next_scene = nullptr;
  std::vector<std::shared_ptr<UIElem>> menu{};
  Scene(sf::RenderWindow *window) : window(window) {}
  void handle_ui_events(sf::Event event);
  void handle_ui_render();
  void handle_ui_update(double dt);
  virtual void init() = 0;
  virtual void update(double dt) = 0;
  virtual void render() = 0;
  virtual void event(sf::Event event) = 0;
};