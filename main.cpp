
// Standard headers
#include <numeric>
#include <random>

// 3rd party headers
#include <SDL2/SDL.h>
#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_sdlrenderer2.h>

// Project headers
#include "sorting_algo.hpp"

struct sorting_visualizer final
{
    static int run();

private:
    sorting_visualizer() noexcept;
    ~sorting_visualizer() noexcept;

    bool handle(SDL_Event const &e) noexcept;

    void update() noexcept;

    void render() noexcept;

    void imgui() noexcept;

    SDL_Window *win;
    SDL_Renderer *ren;

    std::default_random_engine gen;
    unsigned numbers[50] = {};
    tnt::colors cs; // red and blue pivot indices

    bool paused = true;

    int active_algo = 0;
    tnt::sorting_task sort;
};

int main(int, char **)
{
    return sorting_visualizer::run();
}

int sorting_visualizer::run()
{
    sorting_visualizer app{};

    SDL_Event e;
    bool running = true;

    while (running)
    {
        while (SDL_PollEvent(&e))
            running = app.handle(e);

        app.update();
        app.render();
    }

    return 0;
}

sorting_visualizer::sorting_visualizer() noexcept
    : sort{sortviz::bubble_sort(numbers)}
{
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        std::exit(1);
    }

    win = SDL_CreateWindow(
        "The TnT Sorting Visualizer",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        1280, 720,
        SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI //
    );

    if (win == nullptr)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        std::exit(1);
    }

    ren = SDL_CreateRenderer(
        win, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC //
    );

    if (ren == nullptr)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        std::exit(1);
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGui_ImplSDL2_InitForSDLRenderer(win, ren);
    ImGui_ImplSDLRenderer2_Init(ren);

    std::random_device rd;
    gen.seed(rd());

    std::iota(std::begin(numbers), std::end(numbers), 1);
    std::ranges::shuffle(numbers, gen);
}

sorting_visualizer::~sorting_visualizer() noexcept
{
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
}

bool sorting_visualizer::handle(SDL_Event const &e) noexcept
{
    if (e.type == SDL_QUIT)
        return false;
    else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)
        return false;

    ImGui_ImplSDL2_ProcessEvent(&e);

    return true;
}

void sorting_visualizer::update() noexcept
{
    if (!paused)
    {
        if (!sort.done())
        {
            sort.resume();
            cs = sort.value();
        }
    }

    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    imgui();
}

void sorting_visualizer::render() noexcept
{
    SDL_SetRenderDrawColor(ren, 0x00, 0x00, 0x00, 0xff);
    SDL_RenderClear(ren);

    SDL_SetRenderDrawColor(ren, 0xff, 0xff, 0xff, 0xff);

    for (int i = 0; i < std::size(numbers); ++i)
    {
        auto const area = SDL_Rect{
            .x = 12 + i * 25,
            .y = int(720 - numbers[i] * 10),
            .w = 25,
            .h = int(numbers[i] * 10),
        };

        SDL_RenderFillRect(ren, &area);
    }

    {
        SDL_SetRenderDrawColor(ren, 0xff, 0x00, 0x00, 0xff);

        auto const area = SDL_Rect{
            .x = int(12 + cs.red * 25),
            .y = int(720 - numbers[cs.red] * 10),
            .w = 25,
            .h = int(numbers[cs.red] * 10),
        };

        SDL_RenderFillRect(ren, &area);
    }

    {
        SDL_SetRenderDrawColor(ren, 0x00, 0x00, 0xff, 0xff);

        auto const area = SDL_Rect{
            .x = int(12 + cs.blue * 25),
            .y = int(720 - numbers[cs.blue] * 10),
            .w = 25,
            .h = int(numbers[cs.blue] * 10),
        };

        SDL_RenderFillRect(ren, &area);
    }

    ImGui::Render();
    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());

    SDL_RenderPresent(ren);
}

static constexpr const char *algo_names[] = {
    "Bubble Sort",
    "Selection Sort",
    "Quick Sort",
    "Heap Sort",
};

static constexpr tnt::sorting_task (*algo_ptr[])(std::span<unsigned>) = {
    sortviz::bubble_sort,
    sortviz::selection_sort,
    sortviz::quick_sort,
    sortviz::heap_sort,
};

void sorting_visualizer::imgui() noexcept
{
    if (ImGui::Begin("Controls"))
    {
        if (ImGui::Button(paused ? "Play" : "Pause"))
            paused = !paused;

        ImGui::SameLine();

        if (ImGui::Button("Shuffle"))
        {
            std::ranges::shuffle(numbers, gen);
            sort = algo_ptr[active_algo](numbers);
        }

        if (paused)
        {
            if (ImGui::BeginCombo("Algorithm", algo_names[active_algo]))
            {
                for (int i = 0; i < std::size(algo_names); ++i)
                {
                    bool const selected = active_algo == i;

                    if (ImGui::Selectable(algo_names[i], selected))
                    {
                        active_algo = i;
                        sort = algo_ptr[i](numbers);
                    }

                    if (selected)
                        ImGui::SetItemDefaultFocus();
                }

                ImGui::EndCombo();
            }
        }
    }
    ImGui::End();
}