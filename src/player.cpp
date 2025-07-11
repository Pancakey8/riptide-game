#include "player.hpp"

sf::Vector2f Player::top_left() {
  sf::Vector2f topleft = pos - (float)0.5 * size;
  // printf("Pos(%f,%f) TL(%f,%f)", pos.x, pos.y, topleft.x, topleft.y);
  return topleft;
}

void Player::update_tiles() {
  // |i| + |j| <= 10
  printf("Entered chunk %f,%f\n", last_chunk.x, last_chunk.y);

  bool do_clean = chunks.size() > 100;
  std::unordered_set<Chunk, ChunkHash> clean;

  for (int d = 0; d <= 5; ++d) {
    for (int i = 0; i <= d; ++i) {
      int j = d - i;
      for (int x : {i, -1 * i}) {
        for (int y : {j, -1 * j}) {
          auto chunk = Chunk::load(window, x, y, "./map.txt");
          if (chunk.has_value()) {
            if (do_clean) {
              clean.insert(chunk.value());
            } else {
              chunks.insert(chunk.value());
            }
          }
        }
      }
    }
  }

  if (do_clean) {
    chunks = clean;
  }
}

void Player::update(double dt) {
  Time now = std::chrono::steady_clock::now();
  if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right) &&
      measure(now, last_dashed) > dash_duration) {
    auto pos = camera->get_cursor();
    target = (sf::Vector2f)pos;
    is_moving_to_target = true;
  }

  sf::Vector2f delta_move = {0, 0};

  if (is_moving_to_target) {
    auto dir = target - pos;
    if (dir.length() < vel * dt) {
      delta_move = dir;
      is_moving_to_target = false;
    } else {
      delta_move = (float)dt * dir * vel / dir.length();
    }
    last_angle = dir.angle();
  }

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
    if (measure(now, last_dashed) >= dash_cooldown) {
      last_dashed = now;
      auto mouse = camera->get_cursor();
      dash_direction = mouse - pos;
      dash_direction /= dash_direction.length();
      is_moving_to_target = false;
      last_angle = dash_direction.angle();
    }
  }

  if (measure(now, last_dashed) <= dash_duration) {
    delta_move = dash_distance * (float)(dt / dash_duration) * dash_direction;
  }

  if (delta_move.length()) {
    Tile collides_with;
    bool do_collide = false;
    std::vector<Tile> tiles;
    for (int i : {-1, 0, 1}) {
      for (int j : {-1, 0, 1}) {
        Chunk chunk_find;
        chunk_find.pos = {last_chunk.x + (float)i, last_chunk.y + (float)j};
        auto neighbor = chunks.find(chunk_find);
        if (neighbor != chunks.end()) {
          tiles.insert(tiles.begin(), neighbor->tiles.begin(),
                       neighbor->tiles.end());
        }
      }
    }

    float earliest_t = 1.0f;
    Tile first_colliding_tile;

    for (auto &tile : tiles) {
      float t_hit;
      if (tile.does_collide(top_left(), size, delta_move, t_hit)) {
        if (t_hit < earliest_t) {
          earliest_t = t_hit;
          first_colliding_tile = tile;
          do_collide = true;
        }
      }
    }

    if (do_collide) {
      pos += delta_move * earliest_t;
      is_moving_to_target = false;
    } else {
      pos += delta_move;
    }
  }

  auto chunk_now = Chunk::nearest(pos);
  if (chunk_now != last_chunk) {
    last_chunk = chunk_now;
    update_tiles();
  }

  for (auto it = abilities.begin(); it != abilities.end();) {
    if ((*it)->is_alive()) {
      (*it)->update(dt);
      ++it;
    } else {
      it = abilities.erase(it);
    }
  }
}

void Player::event(sf::Event event) {
  if (event.is<sf::Event::KeyPressed>() &&
      event.getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Q) {
    std::shared_ptr<Projectile> proj = std::make_shared<Projectile>(window);
    proj->direction = window->mapPixelToCoords(sf::Mouse::getPosition()) - pos;
    proj->direction = proj->direction.normalized();
    proj->pos = pos + proj->direction * (size.length() / 2);
    proj->cooldown = 5;
    abilities.push_back(proj);
  }
}

void Player::render() {
  sf::RectangleShape player_arrow{size};
  player_arrow.setOrigin((float)0.5 * size);
  player_arrow.setPosition(pos);
  player_arrow.setRotation(last_angle);
  player_arrow.setFillColor(sf::Color::Black);
  window->draw(player_arrow);
  sf::RectangleShape player_hitbox{size};
  player_hitbox.setPosition(top_left());
  player_hitbox.setFillColor(sf::Color::Transparent);
  player_hitbox.setOutlineThickness(3);
  player_hitbox.setOutlineColor(sf::Color::Red), window->draw(player_hitbox);
  if (is_moving_to_target) {
    sf::CircleShape target{15};
    target.setFillColor(sf::Color::Green);
    target.setPosition(this->target - sf::Vector2f{15, 15});
    window->draw(target);
  }
  for (auto chunk : chunks) {
    chunk.render();
  }
  for (auto &ability : abilities) {
    ability->render();
  }
}