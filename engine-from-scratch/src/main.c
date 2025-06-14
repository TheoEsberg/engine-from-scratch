#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <glad/glad.h>
#define SDL_MAIN_HANDLED
#include <SDL3/SDL.h>

#include "engine/global.h"
#include "engine/config.h"
#include "engine/input.h"
#include "engine/time.h"
#include "engine/physics.h"
#include "engine/util.h"

static bool should_quit = false;
static vec2 pos;

static void input_handle(void) {
    if (global.input.escape == KS_PRESSED || global.input.escape == KS_HELD) {
        should_quit = true;
    }

    float x, y;
    SDL_GetMouseState(&x, &y);
    pos[0] = x;
    pos[1] = global.render.height - y;
}

int main(int argc, char *argv[]) {
    time_init(60);
    config_init();
    render_init();
    physics_init();

    pos[0] = global.render.width * 0.5;
    pos[1] = global.render.height * 0.5;

    SDL_HideCursor();

    AABB test_aabb = {
        .position = {global.render.width * 0.5, global.render.height * 0.5},
        .half_size = {50, 50}
    };

    AABB cursor_aabb = {.half_size = {75, 75}};

    AABB sum_aabb = {
        .position = {test_aabb.position[0], test_aabb.position[1]},
        .half_size = {
            test_aabb.half_size[0] + cursor_aabb.half_size[0],
            test_aabb.half_size[1] + cursor_aabb.half_size[1]
        }
    };

    while (!should_quit) {
        time_update();
        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_EVENT_QUIT:
                should_quit = true;
                break;
            default:
                break;
            }
        }
        input_update();
        input_handle();
        physics_update();

        render_begin();

        cursor_aabb.position[0] = pos[0];
        cursor_aabb.position[1] = pos[1];

        render_aabb((f32*)&test_aabb, (vec4){1, 1, 1, 0.5});

        render_aabb((f32*)&sum_aabb, (vec4){1, 1, 1, 0.5});

        AABB minkowski_difference = aabb_minkowski_difference(test_aabb, cursor_aabb);
        render_aabb((f32*)&minkowski_difference, ORANGE);
        
        vec2 pv;
        aabb_penetration_vector(pv, minkowski_difference);

        AABB collision_aabb = cursor_aabb;
        collision_aabb.position[0] += pv[0];
        collision_aabb.position[1] += pv[1];

        if (physics_aabb_intersect_aabb(test_aabb, cursor_aabb)){
            render_aabb((f32*)&cursor_aabb, RED);
            render_aabb((f32*)&collision_aabb, CYAN);

            vec2_add(pv, pos, pv);
            render_line_segment(pos, pv, CYAN);
        } else {
            render_aabb((f32*)&cursor_aabb, WHITE);
        }

        if (physics_point_intersect_aabb(pos, test_aabb)){
            render_quad(pos, (vec2){10, 10}, RED);
        }
        else {
            render_quad(pos, (vec2){10, 10}, WHITE);
        }

        render_end();
        time_update_late();
    }

    return 0;
}