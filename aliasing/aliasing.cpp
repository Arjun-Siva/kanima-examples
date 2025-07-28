#include <iostream>
#include <kanima/util/renderScene.h>
#include <kanima/core/scene.h>
#include <kanima/util/pixelBuffer.h>

int main()
{
    krt::Scene scene("glassball.crtscene");
    krt::RenderConfig config;

    config.buffer_height = 720;
    config.buffer_width = 1280;
    config.print_info = true;
    config.bucket_size = 24;
    config.gi_ray_count = 0;
    config.max_tree_depth = 24;
    config.min_triangles_per_leaf = 4;
    config.num_threads = 8;
    config.ray_depth = 2;
    config.rebuild_BVH = false;
    config.sample_per_pixel = 64;
    config.use_BVH = true;


    for (int i = 0; i < 16; i++)
    {
        krt::PixelBuffer buffer = krt::renderSceneToBuffer(scene, config);
        buffer.writeToPPM("with_aa_" + std::to_string(i) + ".ppm");
        scene.camera.dolly(0.125);
    }

    return 0;
}
