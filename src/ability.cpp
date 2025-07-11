#include "ability.hpp"

bool Ability::is_alive() {
  return measure(std::chrono::time_point_cast<Time::duration>(
                     start + std::chrono::duration<double>(cooldown)),
                 start) > 0;
}

void Projectile::update(double dt) {
  this->pos += direction * velo * (float)dt;
}

void Projectile::render() {
  sf::RectangleShape proj(size);
  proj.setPosition(pos);
  proj.setFillColor(sf::Color::Magenta);
  window->draw(proj);
}