#include "src/loop.hpp"
#include "src/time.hpp"

int main() {
  auto desktop = sf::VideoMode::getDesktopMode();
  sf::RenderWindow window(desktop, "Riptide");
  Loop loop(&window);

  loop.init();

  Time last = std::chrono::steady_clock::now();
  while (window.isOpen()) {
    while (const auto event = window.pollEvent()) {
      if (event->is<sf::Event::Closed>())
        window.close();
      if (event->is<sf::Event::MouseWheelScrolled>()) {
        loop.camera.zoom(event->getIf<sf::Event::MouseWheelScrolled>()->delta);
      }
    }

    Time now = std::chrono::steady_clock::now();
    loop.update(measure(now, last));
    last = now;
    loop.render();
  }

  return 0;
}
