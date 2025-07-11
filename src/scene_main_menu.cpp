#include "scene_main_menu.hpp"
#include "scene_game.hpp"

void MainMenuScene::init() {
  sf::Vector2f dsize = {100, 50};
  auto dpos = (sf::Vector2f)sf::VideoMode::getDesktopMode().size;
  dpos /= (float)2;
  dpos -= dsize;
  std::shared_ptr<GenericButton> button =
      std::make_shared<GenericButton>(window, dpos, dsize);
  button->text = "START";
  button->on_click = [this]() {
    this->do_switch_scene = true;
    auto game_scene = std::make_unique<GameScene>(window);
    game_scene->init();
    this->next_scene = std::move(game_scene);
  };
  button->init();
  menu.push_back(button);
}
void MainMenuScene::update(double dt) { handle_ui_update(dt); }
void MainMenuScene::render() {
  window->clear(sf::Color::White);
  handle_ui_render();
  window->display();
}
void MainMenuScene::event(sf::Event event) { handle_ui_events(event); }