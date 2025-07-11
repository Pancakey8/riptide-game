#pragma once
#include "time.hpp"
#include <SFML/Graphics.hpp>
#include <functional>

class UIElem {
public:
  sf::Vector2f pos;  // top-left
  sf::Vector2f size; // hitbox
  sf::RenderWindow *window;

  UIElem(sf::RenderWindow *window, sf::Vector2f pos, sf::Vector2f size)
      : window(window), pos(pos), size(size) {}
  std::function<void()> on_click = 0;
  virtual void init() = 0;
  virtual void update(double dt) = 0;
  virtual void render() = 0;
};

class GenericButton : public UIElem {
public:
  std::string text;
  sf::Font font;
  sf::Color background = sf::Color::Black, foreground = sf::Color::White;
  int text_size = 20;

  GenericButton(sf::RenderWindow *window, sf::Vector2f pos, sf::Vector2f size)
      : UIElem(window, pos, size) {}

  void init() override;
  void update(double dt) override;
  void render() override;
};

class ProgressBar : public UIElem {
public:
  sf::Color background = sf::Color(20, 20, 20), loading = sf::Color::Red,
            ready = sf::Color::Green;
  float percent = 1;
  Time finish = std::chrono::steady_clock::now();
  double duration;
  bool is_timed = false;
  ProgressBar(sf::RenderWindow *window, sf::Vector2f pos, sf::Vector2f size)
      : UIElem(window, pos, size) {}

  void init() override;
  void update(double dt) override;
  void render() override;
};