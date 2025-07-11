#include "scene.hpp"

class MainMenuScene : public Scene {
public:
  MainMenuScene(sf::RenderWindow *window) : Scene(window) {}

  void init() override;
  void update(double dt) override;
  void render() override;
  void event(sf::Event event) override;
};