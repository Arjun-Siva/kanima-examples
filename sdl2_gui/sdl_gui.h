#ifndef SDL_GUI_H
#define SDL_GUI_H

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <kanima/util/pixelBuffer.h>
#include <kanima/util/renderScene.h>
#include <kanima/core/scene.h>
#include <kanima/util/clamp.h>
#include <unordered_map>

void displayPixelBuffer(const krt::PixelBuffer& buffer);

void displayInteractive(krt::PixelBuffer& buffer, krt::Scene& scene, krt::RenderConfig& config);

#endif // SDL_GUI_H
