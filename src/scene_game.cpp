#include "scene_game.hpp"
#include "scene_main_menu.hpp"

void GameScene::init() {
  camera.window = window;
  player.window = window;
  player.camera = &camera;
  player.update_tiles();

  auto desktop = (sf::Vector2f)sf::VideoMode::getDesktopMode().size;
  dash_timer = std::make_shared<ProgressBar>(
      window, sf::Vector2f{desktop.x / 2 - 100, 50}, sf::Vector2f{200, 40});
  dash_timer->is_timed = true;
  menu.push_back(dash_timer);
}

void GameScene::update(double dt) {
  handle_ui_update(dt);
  camera.update(dt);
  player.update(dt);
  dash_timer->finish = std::chrono::time_point_cast<Time::duration>(
      player.last_dashed + std::chrono::duration<double>(player.dash_cooldown));
  dash_timer->duration = player.dash_cooldown;
}

void GameScene::render() {
  window->clear(sf::Color::White);
  player.render();
  handle_ui_render();
  window->display();
}

void GameScene::event(sf::Event event) {
  handle_ui_events(event);

  player.event(event);

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