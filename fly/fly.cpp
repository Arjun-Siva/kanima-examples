#include <iostream>
#include <kanima/util/renderScene.h>
#include <kanima/core/scene.h>
#include <kanima/util/pixelBuffer.h>
#include <cstdlib>

void fly(krt::Mesh& mesh)
{
    std::vector<krt::vec3> newVertices;

    float up = 2.0;
    float side = 1.5;

    krt::vec3 offset = krt::vec3(side, up, 0);

    for (size_t i = 0; i < mesh.triangleVertIndices.size() - 2; i += 3)
    {
        krt::vec3 v1 = mesh.vertices[mesh.triangleVertIndices[i]];
        krt::vec3 v2 = mesh.vertices[mesh.triangleVertIndices[i+1]];
        krt::vec3 v3 = mesh.vertices[mesh.triangleVertIndices[i+2]];

        v1 = v1 + offset;
        v2 = v2 + offset;
        v3 = v3 + offset;

        newVertices.push_back(v1);
        newVertices.push_back(v2);
        newVertices.push_back(v3);
    }

    mesh.vertices = newVertices;
}

void duplicateMeshVertices(krt::Mesh& mesh)
{
    std::vector<krt::vec3> newVertices;
    std::vector<int> newTriangleVertIndices;

    for (size_t i = 0; i < mesh.triangleVertIndices.size() - 2; i += 3)
    {
        krt::vec3 v1 = mesh.vertices[mesh.triangleVertIndices[i]];
        krt::vec3 v2 = mesh.vertices[mesh.triangleVertIndices[i+1]];
        krt::vec3 v3 = mesh.vertices[mesh.triangleVertIndices[i+2]];

        newVertices.push_back(v1);
        newVertices.push_back(v2);
        newVertices.push_back(v3);

        newTriangleVertIndices.push_back(i);
        newTriangleVertIndices.push_back(i+1);
        newTriangleVertIndices.push_back(i+2);
    }

    mesh.vertices = newVertices;
    mesh.triangleVertIndices = newTriangleVertIndices;
}

int main()
{
    krt::Scene scene("dragon.crtscene");
    krt::RenderConfig config;

    config.buffer_height = 720;
    config.buffer_width = 1280;
    config.print_info = true;
    config.bucket_size = 24;
    config.gi_ray_count = 0;
    config.max_tree_depth = 24;
    config.min_triangles_per_leaf = 4;
    config.num_threads = 8;
    config.ray_depth = 3;
    config.rebuild_BVH = true;
    config.sample_per_pixel = 1;
    config.use_BVH = true;


    krt::PixelBuffer buffer = krt::renderSceneToBuffer(scene, config);
    buffer.writeToPPM("dragon_.ppm");

    for (int i = 0; i < 16; i++)
    {
        krt::Mesh& dragon = scene.geometryObjects[1];
        dragon.material.smoothShading = false;

        duplicateMeshVertices(dragon);
        fly(dragon);
        dragon.computeAABB();

        krt::PixelBuffer buffer = krt::renderSceneToBuffer(scene, config);
        buffer.writeToPPM("dragon_" + std::to_string(i) + ".ppm");
    }

    return 0;
}
