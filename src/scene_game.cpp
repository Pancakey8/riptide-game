#include "scene_game.hpp"
#include "scene_main_menu.hpp"

void GameScene::init() {
  camera.window = window;
  player.window = window;
  player.camera = &camera;
  player.update_tiles();
}

void GameScene::update(double dt) {
  camera.update(dt);
  player.update(dt);
}

void GameScene::render() {
  window->clear(sf::Color::White);
  player.render();
  window->display();
}

void GameScene::event(sf::Event event) {
  if (event.is<sf::Event::KeyPressed>()) {
    auto key = event.getIf<sf::Event::KeyPressed>()->code;
    if (key == sf::Keyboard::Key::Escape) {
      do_switch_scene = true;
      auto menu = std::make_unique<MainMenuScene>(window);
      menu->init();
      next_scene = std::move(menu);
    }
  }

  if (event.is<sf::Event::MouseWheelScrolled>()) {
    camera.zoom(event.getIf<sf::Event::MouseWheelScrolled>()->delta);
  }
}