#include "player.hpp"
#include "scene.hpp"

class GameScene : public Scene {
public:
  GameScene(sf::RenderWindow *window) : Scene(window) {}

  Player player;
  Camera camera;

  void init() override;
  void update(double dt) override;
  void render() override;
  void event(sf::Event event) override;
};