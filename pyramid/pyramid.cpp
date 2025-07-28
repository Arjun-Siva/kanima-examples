#include <iostream>
#include <kanima/util/renderScene.h>
#include <kanima/core/scene.h>
#include <kanima/util/pixelBuffer.h>

#include "sdl_gui.h"

int main()
{
    krt::Scene scene;
    krt::Mesh pyramid;

    pyramid.insertVertex(-3.0, 0.0, -5.0);
    pyramid.insertVertex(3.0, 0.0, -5.0);
    pyramid.insertVertex(0.0, 4.0, -8.0);

    pyramid.insertVertex(3.0, 0.0, -11.0);
    pyramid.insertVertex(-3.0, 0.0, -11.0);

    pyramid.insertTriangleIndex(0, 1, 2); //front
    pyramid.insertTriangleIndex(1, 3, 2); //right
    pyramid.insertTriangleIndex(0, 2, 4); //left
    pyramid.insertTriangleIndex(3, 4, 2); //back

    std::string name = "pyramid";

    krt::AlbedoTexture albedo = krt::AlbedoTexture("albedo", krt::Color(1, 1, 1));
    std::shared_ptr<krt::AlbedoTexture> albedoPtr = std::make_shared<krt::AlbedoTexture>(albedo);
    scene.addTexture(name, albedoPtr);


    krt::Material pyramidMaterial;
    pyramidMaterial.type = krt::MaterialType::Diffuse;
    pyramidMaterial.albedoTex = albedoPtr;


    pyramid.setMaterial(pyramidMaterial);
    pyramid.computeAABB();
    pyramid.computeTriangleNormals();
    pyramid.computeVertexNormals();

    // ground
    krt::Mesh ground;

    ground.insertVertex(-50.0, 0.0, 50.0);
    ground.insertVertex(50.0, 0.0, 50.0);
    ground.insertVertex(50.0, 0.0, -50.0);
    ground.insertVertex(-50.0, 0.0, -50.0);


    ground.insertTriangleIndex(0, 1, 3);
    ground.insertTriangleIndex(1, 2, 3);

    std::string name2 = "ground";

    krt::AlbedoTexture albedo2 = krt::AlbedoTexture("albedo2", krt::Color(1, 1, 0));
    std::shared_ptr<krt::AlbedoTexture> albedo2Ptr = std::make_shared<krt::AlbedoTexture>(albedo2);
    scene.addTexture(name2, albedo2Ptr);


    krt::Material groundMaterial;
    groundMaterial.type = krt::MaterialType::Diffuse;
    groundMaterial.albedoTex = albedo2Ptr;


    ground.setMaterial(groundMaterial);
    ground.computeAABB();
    ground.computeTriangleNormals();
    ground.computeVertexNormals();

    scene.addMaterial(groundMaterial);
    scene.addMaterial(pyramidMaterial);

    scene.addMesh(ground);
    scene.addMesh(pyramid);

    krt::vec3 lightpos(2, 5, -4);
    krt::Light sun(lightpos, 500);

    scene.addLight(sun);

    scene.camera.dolly(-1);
    scene.camera.boom(0.25);

    scene.bgColor = krt::Color(170.f/255.0, 231.f/255.0, 250.f/255.0);

    krt::RenderConfig config;

    config.buffer_height = 720;
    config.buffer_width = 1280;
    config.print_info = false;
    config.bucket_size = 24;
    config.gi_ray_count = 0;
    config.max_tree_depth = 24;
    config.min_triangles_per_leaf = 4;
    config.num_threads = 8;
    config.ray_depth = 2;
    config.rebuild_BVH = false;
    config.sample_per_pixel = 1;
    config.use_BVH = true;

    // make sure SDL.dll is in the build folder

    krt::PixelBuffer buffer = krt::renderSceneToBuffer(scene, config);

    displayInteractive(buffer, scene, config);


    return 0;
}
