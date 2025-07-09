#include "src/loop.hpp"
#include "src/time.hpp"
#include <cmath>
#include <map>

std::unordered_set<Chunk, ChunkHash>
update_chunks(sf::RenderWindow *win,
              std::map<std::pair<float, float>, std::vector<TileId>> map) {
  std::unordered_set<Chunk, ChunkHash> set;
  for (auto pair : map) {
    Chunk ch(win, sf::Vector2f{pair.first.first, pair.first.second},
             pair.second);
    set.insert(ch);
  }
  return set;
}

int main() {
  auto desktop = sf::VideoMode::getDesktopMode();
  sf::RenderWindow window(desktop, "Riptide Editor");
  Camera camera;
  camera.window = &window;
  sf::Font font;
  auto _ = font.openFromFile("./Roboto.ttf");
  std::map<std::pair<float, float>, std::vector<TileId>> chunks{};
  auto chunk_set = update_chunks(&window, chunks);

  Time last = std::chrono::steady_clock::now();
  bool have_key = false;
  sf::Keyboard::Key key = sf::Keyboard::Key::Unknown;
  bool have_mouse = false;
  sf::Mouse::Button unknown_button = (sf::Mouse::Button)-1;
  sf::Mouse::Button button = unknown_button;
  int current_tile = 0;
  while (window.isOpen()) {
    while (const auto event = window.pollEvent()) {
      if (event->is<sf::Event::Closed>())
        window.close();
      if (event->is<sf::Event::MouseWheelScrolled>()) {
        camera.zoom(event->getIf<sf::Event::MouseWheelScrolled>()->delta);
      }
      if (event->is<sf::Event::KeyPressed>()) {
        have_key = true;
        key = event->getIf<sf::Event::KeyPressed>()->code;
      }
      if (event->is<sf::Event::MouseButtonPressed>()) {
        have_mouse = true;
        button = event->getIf<sf::Event::MouseButtonPressed>()->button;
      }
    }
    Time now = std::chrono::steady_clock::now();
    double dt = measure(now, last);

    camera.update(dt);

    if (have_key && sf::Keyboard::Key::Num0 <= key &&
        key <= sf::Keyboard::Key::Num9) {
      current_tile =
          10 * current_tile + (int)key - (int)sf::Keyboard::Key::Num0;
    }

    if ((have_key && sf::Keyboard::Key::Enter == key) ||
        (have_mouse && sf::Mouse::Button::Left == button)) {
      auto tile_pos = camera.get_cursor();
      auto chunk_pos = Chunk::nearest(tile_pos);
      std::pair<float, float> query = std::make_pair(chunk_pos.x, chunk_pos.y);

      sf::Vector2f tile = {tile_pos.x / TILE_SIZE[0],
                           tile_pos.y / TILE_SIZE[1]};
      tile.x -= chunk_pos.x * CHUNK_SIZE[0];
      tile.y -= chunk_pos.y * CHUNK_SIZE[1];
      tile.x = std::floor(tile.x);
      tile.y = std::floor(tile.y);
      printf("%f,%f\n", tile.x, tile.y);

      auto chunk = chunks.find(query);
      if (chunk != chunks.end()) {
        chunk->second[(int)tile.y * (int)CHUNK_SIZE[0] + (int)tile.x] =
            (TileId)current_tile;
      } else {
        std::vector<TileId> tiles;
        tiles.reserve(CHUNK_SIZE[0] * CHUNK_SIZE[1]);
        for (int i = 0; i < CHUNK_SIZE[0] * CHUNK_SIZE[1]; ++i) {
          tiles.push_back(BACKGROUND);
        }
        tiles[(int)tile.y * (int)CHUNK_SIZE[0] + (int)tile.x] =
            (TileId)current_tile;
        chunks.emplace(query, tiles);
      }
      chunk_set = update_chunks(&window, chunks);
    }

    if (have_key && sf::Keyboard::Key::Backspace == key) {
      current_tile = 0;
    }

    have_key = false;
    have_mouse = false;

    last = now;

    window.clear(sf::Color::White);
    sf::CircleShape cursor{10};
    cursor.setFillColor(sf::Color::Black);
    cursor.setPosition(camera.get_cursor() - sf::Vector2f{10, 10});
    window.draw(cursor);
    for (auto chunk : chunk_set) {
      chunk.render();
    }
    auto top_left = Chunk::nearest(window.mapPixelToCoords({0, 0}));
    auto bottom_right =
        Chunk::nearest(window.mapPixelToCoords((sf::Vector2i)desktop.size));
    for (int i = top_left.x; i <= bottom_right.x; ++i) {
      for (int j = top_left.y; j <= bottom_right.y; ++j) {
        sf::RectangleShape chunk_border(
            {CHUNK_SIZE[0] * TILE_SIZE[0], CHUNK_SIZE[1] * TILE_SIZE[1]});
        chunk_border.setOutlineThickness(2);
        chunk_border.setOutlineColor(sf::Color::Blue);
        chunk_border.setPosition({i * CHUNK_SIZE[0] * TILE_SIZE[0],
                                  j * CHUNK_SIZE[1] * TILE_SIZE[1]});
        chunk_border.setFillColor(sf::Color::Transparent);
        window.draw(chunk_border);
        sf::Text chunk_id(
            font, "(" + std::to_string(i) + "," + std::to_string(j) + ")", 15);
        chunk_id.setFillColor(sf::Color::Blue);
        chunk_id.setPosition({i * CHUNK_SIZE[0] * TILE_SIZE[0] + 10,
                              j * CHUNK_SIZE[1] * TILE_SIZE[1] + 10});
        window.draw(chunk_id);
      }
    }
    sf::Text tile_display(font, "ID: " + std::to_string(current_tile), 30);
    tile_display.setPosition(window.mapPixelToCoords({50, 50}));
    tile_display.setFillColor(sf::Color::Red);
    window.draw(tile_display);
    window.display();
  }

  return 0;
}
