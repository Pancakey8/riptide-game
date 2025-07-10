#include "scene.hpp"
#include "ui.hpp"

class MainMenuScene : public Scene {
public:
  std::vector<std::unique_ptr<UIElem>> menu{};

  MainMenuScene(sf::RenderWindow *window) : Scene(window) {}

  void init() override;
  void update(double dt) override;
  void render() override;
  void event(sf::Event event) override;
};