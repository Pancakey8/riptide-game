#include "src/scene_main_menu.hpp"
#include "src/time.hpp"

int main() {
  auto desktop = sf::VideoMode::getDesktopMode();
  sf::RenderWindow window(desktop, "Riptide");

  std::unique_ptr<Scene> scene = std::make_unique<MainMenuScene>(&window);
  scene->init();

  Time last = std::chrono::steady_clock::now();
  while (window.isOpen()) {
    Time now = std::chrono::steady_clock::now();
    double dt = measure(now, last);
    last = now;

    scene->update(dt);

    while (const auto event = window.pollEvent()) {
      if (event->is<sf::Event::Closed>())
        window.close();
      scene->event(event.value());
    }

    scene->render();

    if (scene->do_switch_scene) {
      scene = std::move(scene->next_scene);
      window.setView(window.getDefaultView());
    }
  }

  return 0;
}
