#include "scene_main_menu.hpp"
#include "scene_game.hpp"

void MainMenuScene::init() {
  sf::Vector2f dsize = {100, 50};
  auto dpos = (sf::Vector2f)sf::VideoMode::getDesktopMode().size;
  dpos /= (float)2;
  dpos -= dsize;
  std::unique_ptr<GenericButton> button =
      std::make_unique<GenericButton>(window, dpos, dsize);
  button->text = "START";
  button->on_click = [this]() {
    this->do_switch_scene = true;
    auto game_scene = std::make_unique<GameScene>(window);
    game_scene->init();
    this->next_scene = std::move(game_scene);
  };
  button->init();
  menu.push_back(std::move(button));
}
void MainMenuScene::update(double dt) {
  for (auto &elem : menu) {
    elem->update(dt);
  }
}
void MainMenuScene::render() {
  window->clear(sf::Color::White);
  for (auto &elem : menu) {
    elem->render();
  }
  window->display();
}
void MainMenuScene::event(sf::Event event) {
  if (event.is<sf::Event::MouseButtonPressed>() &&
      event.getIf<sf::Event::MouseButtonPressed>()->button ==
          sf::Mouse::Button::Left) {
    auto mouse = event.getIf<sf::Event::MouseButtonPressed>()->position;
    for (auto &elem : menu) {
      if (elem->pos.x <= mouse.x && mouse.x <= elem->pos.x + elem->size.x &&
          elem->pos.y <= mouse.y && mouse.y <= elem->pos.y + elem->size.y) {
        if (elem->on_click != nullptr) {
          elem->on_click();
        }
      }
    }
  }
}