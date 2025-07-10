#include "tile.hpp"
#include <cmath>
#include <fstream>

bool Tile::does_collide(sf::Vector2f opos, sf::Vector2f osize,
                        sf::Vector2f velo, float &t_hit) {
  sf::Vector2f inv_entry, inv_exit;

  if (velo.x > 0.0f) {
    inv_entry.x = pos.x - (opos.x + osize.x);
    inv_exit.x = (pos.x + TILE_SIZE[0]) - opos.x;
  } else {
    inv_entry.x = (pos.x + TILE_SIZE[0]) - opos.x;
    inv_exit.x = pos.x - (opos.x + osize.x);
  }

  if (velo.y > 0.0f) {
    inv_entry.y = pos.y - (opos.y + osize.y);
    inv_exit.y = (pos.y + TILE_SIZE[1]) - opos.y;
  } else {
    inv_entry.y = (pos.y + TILE_SIZE[1]) - opos.y;
    inv_exit.y = pos.y - (opos.y + osize.y);
  }

  sf::Vector2f entry, exit;

  entry.x = (velo.x == 0.0f) ? -INFINITY : inv_entry.x / velo.x;
  exit.x = (velo.x == 0.0f) ? INFINITY : inv_exit.x / velo.x;

  entry.y = (velo.y == 0.0f) ? -INFINITY : inv_entry.y / velo.y;
  exit.y = (velo.y == 0.0f) ? INFINITY : inv_exit.y / velo.y;

  float entryTime = std::max(entry.x, entry.y);
  float exitTime = std::min(exit.x, exit.y);

  if (entryTime > exitTime || (entry.x < 0.0f && entry.y < 0.0f) ||
      entryTime > 1.0f) {
    t_hit = 1.0f;
    return false;
  }

  t_hit = entryTime;
  return true;
}

void Tile::render() {
  sf::RectangleShape rect{{TILE_SIZE[0], TILE_SIZE[1]}};
  rect.setPosition(pos);
  if (id == WALL)
    rect.setFillColor(sf::Color::Yellow);
  rect.setOutlineThickness(1);
  rect.setOutlineColor(sf::Color::Red);
  window->draw(rect);
}

sf::Vector2f Chunk::nearest(sf::Vector2f point) {
  return {std::floor(point.x / TILE_SIZE[0] / CHUNK_SIZE[0]),
          std::floor(point.y / TILE_SIZE[1] / CHUNK_SIZE[1])};
}

Chunk::Chunk(sf::RenderWindow *window, sf::Vector2f pos,
             std::vector<TileId> ids) {
  this->pos = pos;
  std::vector<Tile> tiles;

  for (int row = 0; row < CHUNK_SIZE[1]; ++row) {
    for (int col = 0; col < CHUNK_SIZE[0]; ++col) {
      TileId id = ids[row * CHUNK_SIZE[1] + col];
      if (id) {
        Tile t;
        t.window = window;
        t.id = id;
        t.pos = {pos.x * CHUNK_SIZE[0] * TILE_SIZE[0] + col * TILE_SIZE[0],
                 pos.y * CHUNK_SIZE[1] * TILE_SIZE[1] + row * TILE_SIZE[1]};
        tiles.push_back(t);
      }
    }
  }

  this->tiles = tiles;
  this->window = window;
}

void Chunk::render() {
  for (auto tile : tiles) {
    tile.render();
  }
}

std::vector<std::string> split(std::string s, std::string delimiter) {
  size_t pos_start = 0, pos_end, delim_len = delimiter.length();
  std::string token;
  std::vector<std::string> res;

  while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
    token = s.substr(pos_start, pos_end - pos_start);
    pos_start = pos_end + delim_len;
    res.push_back(token);
  }

  res.push_back(s.substr(pos_start));
  return res;
}

void Chunk::save(std::map<std::pair<float, float>, std::vector<TileId>> chunks,
                 const std::filesystem::path &fp) {
  auto fb = std::ofstream(fp);
  for (auto pair : chunks) {
    std::string data = std::to_string(pair.first.first) + "," +
                       std::to_string(pair.first.second);
    for (auto tid : pair.second) {
      data += "," + std::to_string(tid);
    }
    fb << data << std::endl;
  }
  fb.flush();
  fb.close();
}
std::optional<Chunk> Chunk::load(sf::RenderWindow *win, int x, int y,
                                 const std::filesystem::path &fp) {
  auto fb = std::ifstream(fp);

  for (std::string line; std::getline(fb, line);) {
    auto chunk_data = split(line, ",");
    if (chunk_data.size() < 2)
      continue;
    int cx = std::stof(chunk_data[0]);
    int cy = std::stof(chunk_data[1]);
    if (x == cx && y == cy) {
      std::vector<TileId> tiles;
      for (int i = 2; i < chunk_data.size(); ++i) {
        tiles.push_back((TileId)std::stoi(chunk_data[i]));
      }
      Chunk ch(win, {(float)x, (float)y}, tiles);
      return ch;
    }
  }

  fb.close();
  return std::nullopt;
}

bool Chunk::operator==(const Chunk &other) const { return pos == other.pos; }
size_t ChunkHash::operator()(const Chunk &c) const {
  return std::hash<float>{}(c.pos.x) ^ (std::hash<float>{}(c.pos.y) << 1);
}