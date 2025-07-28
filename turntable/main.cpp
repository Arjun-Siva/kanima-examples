#include <iostream>
#include <kanima/util/renderScene.h>
#include <kanima/core/scene.h>
#include <kanima/util/pixelBuffer.h>

int main()
{
    krt::Scene scene("cube_sphere.crtscene");
    krt::RenderConfig config;

    config.buffer_height = 480;
    config.buffer_width = 480;
    config.print_info = true;
    config.bucket_size = 24;
    config.gi_ray_count = 1;
    config.max_tree_depth = 24;
    config.min_triangles_per_leaf = 4;
    config.num_threads = 8;
    config.ray_depth = 5;
    config.rebuild_BVH = false;
    config.sample_per_pixel = 64;
    config.use_BVH = true;

    // make sure SDL.dll is in the build folder

    scene.camera.dolly(1.5);

    for (int i = 0; i < 64; i++)
    {
        krt::PixelBuffer buffer = krt::renderSceneToBuffer(scene, config);
        buffer.writeToPPM("cube_sph_" + std::to_string(i) + ".ppm");
        scene.camera.turntable(krt::vec3(0, 0, -2.25), 5.625);
        std::cout<<"Image:"<<i<<" completed.\n";
    }


    return 0;
}
