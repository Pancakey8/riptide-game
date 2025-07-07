#include "camera.hpp"

Camera::Camera() { cursor_last = (sf::Vector2f)sf::Mouse::getPosition(); }

void Camera::set() {
  sf::View view(window->getDefaultView());
  view.setCenter(center);
  view.setSize(scale * view.getSize());
  window->setView(view);
}

void Camera::move(sf::Vector2f change) {
  center += change;
  set();
}

sf::Vector2f Camera::get_cursor() {
  auto mouse = sf::Mouse::getPosition(*window);
  return window->mapPixelToCoords(mouse);
}

void Camera::zoom(int ticks) {
  scale -= ticks * 0.05;
  set();
}

void Camera::update(double dt) {
  bool is_pressed = sf::Mouse::isButtonPressed(sf::Mouse::Button::Middle);
  auto cursor = (sf::Vector2f)sf::Mouse::getPosition();
  if (is_pressed && middle_button) {
    move((float)(is_control_flipped ? -1 : 1) * (cursor - cursor_last));
    cursor_last = cursor;
  } else if (is_pressed && !middle_button) {
    middle_button = true;
    cursor_last = cursor;
  } else if (!is_pressed) {
    middle_button = false;
  }
}