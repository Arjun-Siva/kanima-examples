#include <iostream>
#include <kanima/util/renderScene.h>
#include <kanima/core/scene.h>
#include <kanima/util/pixelBuffer.h>
#include <cstdlib>

void explode(krt::Mesh& mesh)
{
    std::vector<krt::vec3> newVertices;

    for (size_t i = 0; i < mesh.triangleVertIndices.size() - 2; i += 3)
    {
        krt::vec3 v1 = mesh.vertices[mesh.triangleVertIndices[i]];
        krt::vec3 v2 = mesh.vertices[mesh.triangleVertIndices[i+1]];
        krt::vec3 v3 = mesh.vertices[mesh.triangleVertIndices[i+2]];

        krt::vec3& triangleNormal = mesh.triangleNormals[i/3];

        float offset = static_cast<float>(std::rand()) / RAND_MAX * 0.1f;

        v1 = v1 + triangleNormal * offset;
        v2 = v2 + triangleNormal * offset;
        v3 = v3 + triangleNormal * offset;

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
    config.ray_depth = 3;
    config.rebuild_BVH = true;
    config.sample_per_pixel = 1;
    config.use_BVH = true;

    scene.camera.dolly(1.0);

    krt::PixelBuffer buffer = krt::renderSceneToBuffer(scene, config);
    buffer.writeToPPM("ball_.ppm");

//    for (int i = 0; i < 32; i++)
//    {
//        krt::Mesh& glassball = scene.geometryObjects[0];
//        glassball.material.smoothShading = false;

//        duplicateMeshVertices(glassball);
//        explode(glassball);
//        glassball.computeTriangleNormals();
//        glassball.computeAABB();

//        krt::PixelBuffer buffer = krt::renderSceneToBuffer(scene, config);
//        buffer.writeToPPM("ball_" + std::to_string(i) + ".ppm");
//    }

    return 0;
}
