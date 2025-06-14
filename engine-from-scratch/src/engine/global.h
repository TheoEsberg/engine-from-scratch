#pragma once 

#include "render/render.h"
#include "config.h"
#include "input.h"
#include "time.h"

typedef struct global{
    Render_State render;
    Config_State config;
    Input_State input;
    Time_State time;
} Global;

extern Global global;

