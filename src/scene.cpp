#include "scene.hpp"

void Scene::handle_ui_events(sf::Event event) {
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

void Scene::handle_ui_update(double dt) {
  for (auto &elem : menu) {
    elem->update(dt);
  }
}

void Scene::handle_ui_render() {
  auto old_view = window->getView();
  window->setView(window->getDefaultView());
  for (auto &elem : menu) {
    elem->render();
  }
  window->setView(old_view);
}