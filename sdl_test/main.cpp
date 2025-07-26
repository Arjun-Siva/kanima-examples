#include <iostream>
#include <kanima/util/renderScene.h>
#include <kanima/core/scene.h>
#include <kanima/util/pixelBuffer.h>

#include "sdl_gui.h"

int main()
{
    krt::Scene scene("dragon.crtscene");
    krt::RenderConfig config;

    config.buffer_height = 360;
    config.buffer_width = 640;

//    config.print_info = true;

    // make sure SDL.dll is in the build folder

    krt::PixelBuffer buffer = krt::renderSceneToBuffer(scene, config);

    displayInteractive(buffer, scene, config);

    return 0;
}
