#include "sdl_gui.h"
#include <unordered_map>
#include <chrono>

float moveSpeed = 5.0f;    // units per second
float rotateSpeed = 45.0f; // degrees per second

void displayPixelBuffer(const krt::PixelBuffer& buffer)
{
    SDL_Init(SDL_INIT_VIDEO);
    int width = buffer.getWidth();
    int height = buffer.getHeight();

    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::cerr << "SDL Init failed: " << SDL_GetError() << "\n";
        return;
    }

    SDL_Window* window = SDL_CreateWindow("Scene",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        width, height, 0);

    if (!window)
    {
        std::cerr << "Window creation failed: " << SDL_GetError() << "\n";
        SDL_Quit();
        return;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_Texture* texture = SDL_CreateTexture(renderer,
        SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_STREAMING,
        width, height);

    std::vector<unsigned char> pixels(width * height * 3);

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            const auto& color = buffer.getColor(x, y);
            int idx = (y * width + x) * 3;
            pixels[idx + 0] = static_cast<Uint8>(std::min(1.0f, color.r) * 255);
            pixels[idx + 1] = static_cast<Uint8>(std::min(1.0f, color.g) * 255);
            pixels[idx + 2] = static_cast<Uint8>(std::min(1.0f, color.b) * 255);
        }
    }

    SDL_UpdateTexture(texture, nullptr, pixels.data(), width * 3);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, nullptr, nullptr);
    SDL_RenderPresent(renderer);

    // Wait until window closed
    SDL_Event event;
    bool running = true;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
                break;
            }
        }
        SDL_Delay(10);
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}


void displayInteractive(krt::PixelBuffer& buffer, krt::Scene& scene, krt::RenderConfig& config)
{
    SDL_Init(SDL_INIT_VIDEO);

    int width = buffer.getWidth();
    int height = buffer.getHeight();

    SDL_Window* window = SDL_CreateWindow("Scene",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        width, height, 0);

    if (!window) {
        std::cerr << "Window creation failed: " << SDL_GetError() << "\n";
        return;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_Texture* texture = SDL_CreateTexture(renderer,
        SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_STREAMING,
        width, height);

    auto updateTexture = [&]()
    {
        std::vector<unsigned char> pixels(width * height * 3);
        for (int y = 0; y < height; ++y)
            for (int x = 0; x < width; ++x)
            {
                const auto& color = buffer.getColor(x, y);
                int idx = (y * width + x) * 3;
                pixels[idx + 0] = static_cast<Uint8>(clamp(static_cast<int>(color.r * 255), 0, 255));
                pixels[idx + 1] = static_cast<Uint8>(clamp(static_cast<int>(color.g * 255), 0, 255));
                pixels[idx + 2] = static_cast<Uint8>(clamp(static_cast<int>(color.b * 255), 0, 255));
            }

        SDL_UpdateTexture(texture, nullptr, pixels.data(), width * 3);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, nullptr, nullptr);
        SDL_RenderPresent(renderer);
    };

    std::unordered_map<SDL_Keycode, bool> keyStates;

    auto lastTime = std::chrono::high_resolution_clock::now();
    auto lastRenderTime = lastTime;

    bool running = true;
    while (running)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                running = false;
            else if (event.type == SDL_KEYDOWN)
                keyStates[event.key.keysym.sym] = true;
            else if (event.type == SDL_KEYUP)
                keyStates[event.key.keysym.sym] = false;
        }

        auto now = std::chrono::high_resolution_clock::now();
        float deltaSec = std::chrono::duration<float>(now - lastTime).count();
        float renderDelta = std::chrono::duration<float>(now - lastRenderTime).count();

        krt::Camera& camera = scene.camera;

        // Apply movement if enough time has passed
        if (renderDelta > 0.1f)
        {
            // Translation
            if (keyStates[SDLK_w]) camera.dolly( +moveSpeed * renderDelta );
            if (keyStates[SDLK_s]) camera.dolly( -moveSpeed * renderDelta );
            if (keyStates[SDLK_a]) camera.truck( -moveSpeed * renderDelta );
            if (keyStates[SDLK_d]) camera.truck( +moveSpeed * renderDelta );
            if (keyStates[SDLK_LSHIFT]) camera.boom( +moveSpeed * renderDelta );
            if (keyStates[SDLK_LCTRL]) camera.boom( -moveSpeed * renderDelta );

            // Rotation
            if (keyStates[SDLK_KP_4] || keyStates[SDLK_4]) camera.pan( +rotateSpeed * renderDelta );
            if (keyStates[SDLK_KP_6] || keyStates[SDLK_6]) camera.pan( -rotateSpeed * renderDelta );
            if (keyStates[SDLK_KP_8] || keyStates[SDLK_8]) camera.tilt( +rotateSpeed * renderDelta );
            if (keyStates[SDLK_KP_2] || keyStates[SDLK_2]) camera.tilt( -rotateSpeed * renderDelta );
            if (keyStates[SDLK_KP_7] || keyStates[SDLK_7]) camera.roll( +rotateSpeed * renderDelta );
            if (keyStates[SDLK_KP_9] || keyStates[SDLK_9]) camera.roll( -rotateSpeed * renderDelta );

            // Render only when camera has moved
            buffer = krt::renderSceneToBuffer(scene, config);
            updateTexture();

            lastRenderTime = now;
        }

        lastTime = now;
        SDL_Delay(10);
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
