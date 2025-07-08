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
          Chunk ch{window,
                   {last_chunk.x + (float)x, last_chunk.y + (float)y},
                   {WALL,       WALL,       BACKGROUND, WALL,       WALL,
                    WALL,       BACKGROUND, BACKGROUND, BACKGROUND, WALL,
                    BACKGROUND, BACKGROUND, BACKGROUND, BACKGROUND, BACKGROUND,
                    WALL,       BACKGROUND, BACKGROUND, BACKGROUND, WALL,
                    WALL,       WALL,       BACKGROUND, WALL,       WALL}};
          if (do_clean) {
            clean.insert(ch);
          } else {
            chunks.insert(ch);
          }
          printf("Loaded %f,%f\n", ch.pos.x, ch.pos.y);
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
    for (auto tile : tiles) {
      if (tile.does_collide(top_left() + delta_move, size)) {
        do_collide = true;
        collides_with = tile;
      }
    }

    if (do_collide) {
      while (!collides_with.does_collide(top_left(), size)) {
        pos += (float)0.01 * delta_move / delta_move.length();
      }
      pos -= (float)0.01 * (collides_with.pos - top_left());
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
}