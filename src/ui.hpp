#pragma once
#include <SFML/Graphics.hpp>
#include <functional>

class UIElem {
public:
  sf::Vector2f pos;  // top-left
  sf::Vector2f size; // hitbox
  sf::RenderWindow *window;

  UIElem(sf::RenderWindow *window, sf::Vector2f pos, sf::Vector2f size)
      : window(window), pos(pos), size(size) {}
  std::function<void()> on_click;
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