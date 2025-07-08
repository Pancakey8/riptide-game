#include "camera.hpp"

Camera::Camera() {
  cursor_last = (sf::Vector2f)sf::Mouse::getPosition();
  center = (float)0.5 * (sf::Vector2f)sf::VideoMode::getDesktopMode().size;
  last_update = std::chrono::steady_clock::now();
}

void Camera::set() {
  sf::View view(window->getDefaultView());
  view.setCenter(center);
  view.setSize(scale * view.getSize());
  window->setView(view);
  printf("%f\n", measure(std::chrono::steady_clock::now(), last_update));
  last_update = std::chrono::steady_clock::now();
  need_change = true;
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
  if (scale < 0.5)
    scale = 0.5;
  if (scale > 2)
    scale = 2;
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

  if (need_change &&
      measure(std::chrono::steady_clock::now(), last_update) >= 0.5) {
    need_change = false;
    auto nearest = Chunk::nearest(center);
    printf("Centered on %f,%f\n", nearest.x, nearest.y);
  }
}