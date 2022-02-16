#include "common.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <imgui-SFML.h>
#include <imgui.h>

// #include "cuda_wrapper.h"
#include "options.h"
#include "scenes/full_screen_opengl.h"

void handleEvents(sf::RenderWindow &window);

int main(int argc, const char **argv) {
  Options opt({std::next(argv), std::next(argv, argc)});
  opt.checkOptions();

  spdlog::info("Starting application");

  // int gpuId = cuda::findBestDevice();
  // cuda::gpuDeviceInit(gpuId);

  sf::RenderWindow window(sf::VideoMode(opt.width, opt.height), "SFML + CUDA",
                          sf::Style::Titlebar | sf::Style::Close);
  window.setVerticalSyncEnabled(true);

  ImGui::SFML::Init(window);
  spdlog::info("SFML window created");

  FullScreenOpenGLScene scene(window);

  AppContext ctx;
  sf::Clock deltaClock;
  while (window.isOpen()) {
    ImGui::SFML::Update(window, deltaClock.restart());

    scene.update(ctx);

    ImGui::Begin("FPS");
    ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);
    // ImGui::Text("%d SPP at %.3f s", ctx.frame, ctx.elapsed_seconds);
    ImGui::End();

    // ImGui::Begin("Control");
    // ImGui::SetNextItemWidth(100);
    // if (ImGui::Combo("Display mode", reinterpret_cast<int *>(&ctx.mode),
    //                  mode_strings, DisplayMode::MODE_COUNT)) {
    //   scene.resetBuffer(ctx);
    // }
    // if (ctx.mode == MODE_DEPTH) {
    //   if (ImGui::SliderFloat("Far plane", &ctx.far_plane, 0.0f, 30.0f)) {
    //     scene.resetBuffer(ctx);
    //   }
    // }
    ImGui::End();

    window.clear();
    scene.render(window);
    ImGui::SFML::Render(window);
    window.display();

    handleEvents(window);
    ctx.dtime = deltaClock.getElapsedTime().asSeconds();
    ctx.frame++;
  }

  spdlog::info("Shutting down");
  ImGui::SFML::Shutdown();

  return 0;
}

void handleEvents(sf::RenderWindow &window) {
  sf::Event event{};
  while (window.pollEvent(event)) {
    ImGui::SFML::ProcessEvent(event);

    if (event.type == sf::Event::Closed) {
      window.close();
    }

    if (event.type == sf::Event::KeyPressed) {
      if (event.key.code == sf::Keyboard::Escape) {
        window.close();
      }
    }
  }
}