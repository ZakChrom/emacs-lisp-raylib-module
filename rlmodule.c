#include <stdio.h>
#include <stdbool.h>
#include <emacs-module.h>
#include <raylib.h>
#include "utils.c"

int plugin_is_GPL_compatible;

static emacs_value Frl_init_window(emacs_env *env, ptrdiff_t nargs, emacs_value *args, void *data) {
    (void)data;

    EXPECT_NARGS(3);
    
    ptrdiff_t size;
    env->copy_string_contents(env, args[2], NULL, &size); // Get len
    
    char title[size];
    env->copy_string_contents(env, args[2], title, &size); // Copy string

    InitWindow(
        env->extract_integer(env, args[0]),
        env->extract_integer(env, args[1]),
        title
    );
    return env->intern(env, "nil");
}

static emacs_value Frl_close_window(emacs_env *env, ptrdiff_t nargs, emacs_value *args, void *data) {
    (void)nargs;
    (void)args;
    (void)data;
    CloseWindow();
    return env->intern(env, "nil");
}

static emacs_value Frl_set_target_fps(emacs_env *env, ptrdiff_t nargs, emacs_value *args, void *data) {
    (void)data;

    EXPECT_NARGS(1);
    
    SetTargetFPS(env->extract_integer(env, args[0]));
    return env->intern(env, "nil");
}

static emacs_value Frl_begin_drawing(emacs_env *env, ptrdiff_t nargs, emacs_value *args, void *data) {
    (void)nargs;
    (void)args;
    (void)data;
    BeginDrawing();
    return env->intern(env, "nil");
}

static emacs_value Frl_end_drawing(emacs_env *env, ptrdiff_t nargs, emacs_value *args, void *data) {
    (void)nargs;
    (void)args;
    (void)data;
    EndDrawing();
    return env->intern(env, "nil");
}

static emacs_value Frl_clear_background(emacs_env *env, ptrdiff_t nargs, emacs_value *args, void *data) {
    (void)data;

    EXPECT_NARGS(1);

    Color col;
    if (!extract_color(env, args[0], &col)) {
        env->non_local_exit_signal(env, env->intern(env, "wrong-type-argument"), args[0]);
        return env->intern(env, "nil");
    }

    ClearBackground(col);
    return env->intern(env, "nil");
}

static emacs_value Frl_window_should_close(emacs_env *env, ptrdiff_t nargs, emacs_value *args, void *data) {
    (void)nargs;
    (void)args;
    (void)data;
    if (WindowShouldClose()) {
        return env->intern(env, "t");
    } else {
        return env->intern(env, "nil");
    }
}

static emacs_value Frl_draw_fps(emacs_env *env, ptrdiff_t nargs, emacs_value *args, void *data) {
    (void)data;

    EXPECT_NARGS(2);

    DrawFPS(
        env->extract_integer(env, args[0]),
        env->extract_integer(env, args[1])
    );
    return env->intern(env, "nil");
}

static emacs_value Frl_draw_rectangle(emacs_env *env, ptrdiff_t nargs, emacs_value *args, void *data) {
    (void)data;

    EXPECT_NARGS(5);

    Color col;
    if (!extract_color(env, args[4], &col)) {
        env->non_local_exit_signal(env, env->intern(env, "wrong-type-argument"), args[4]);
        return env->intern(env, "nil");
    }

    DrawRectangle(
        env->extract_integer(env, args[0]),
        env->extract_integer(env, args[1]),
        env->extract_integer(env, args[2]),
        env->extract_integer(env, args[3]),
        col
    );
    return env->intern(env, "nil");
}

static emacs_value Frl_get_frame_time(emacs_env *env, ptrdiff_t nargs, emacs_value *args, void *data) {
    (void)nargs;
    (void)args;
    (void)data;
    return env->make_float(env, (double)GetFrameTime());
}

static emacs_value Frl_draw_rectangle_v(emacs_env *env, ptrdiff_t nargs, emacs_value *args, void *data) {
    (void)data;

    EXPECT_NARGS(3);

    Vector2 pos;
    if (!extract_vector2(env, args[0], &pos)) {
        env->non_local_exit_signal(env, env->intern(env, "wrong-type-argument"), args[0]);
        return env->intern(env, "nil");
    }

    Vector2 size;
    if (!extract_vector2(env, args[1], &size)) {
        env->non_local_exit_signal(env, env->intern(env, "wrong-type-argument"), args[1]);
        return env->intern(env, "nil");
    }

    Color col;
    if (!extract_color(env, args[2], &col)) {
        env->non_local_exit_signal(env, env->intern(env, "wrong-type-argument"), args[2]);
        return env->intern(env, "nil");
    }

    DrawRectangleV(
        pos,
        size,
        col
    );

    return env->intern(env, "nil");
}

static emacs_value Frl_get_screen_width(emacs_env *env, ptrdiff_t nargs, emacs_value *args, void *data) {
    (void)nargs;
    (void)args;
    (void)data;
    return env->make_integer(env, GetScreenWidth());
}

static emacs_value Frl_get_screen_height(emacs_env *env, ptrdiff_t nargs, emacs_value *args, void *data) {
    (void)nargs;
    (void)args;
    (void)data;
    return env->make_integer(env, GetScreenHeight());
}

int emacs_module_init(struct emacs_runtime *runtime) {
    emacs_env *env = runtime->get_environment(runtime);

    //          Name,                     function,            arity, description
    deffun(env, "rl/init-window",         Frl_init_window,         3, "Initalize window");
    deffun(env, "rl/close-window",        Frl_close_window,        0, "Close window");
    deffun(env, "rl/set-target-fps",      Frl_set_target_fps,      1, "Set fps");
    deffun(env, "rl/begin-drawing",       Frl_begin_drawing,       0, "Begin drawing");
    deffun(env, "rl/end-drawing",         Frl_end_drawing,         0, "End drawing");
    deffun(env, "rl/clear-background",    Frl_clear_background,    1, "Clear the background");
    deffun(env, "rl/window-should-close", Frl_window_should_close, 0, "Returns t if window should close else nil");
    deffun(env, "rl/draw-fps",            Frl_draw_fps,            2, "Draw fps at a location");
    deffun(env, "rl/draw-rectangle",      Frl_draw_rectangle,      5, "Draw a rectangle");
    deffun(env, "rl/get-frame-time",      Frl_get_frame_time,      0, "Get frame time (delta time)");
    deffun(env, "rl/draw-rectangle-v",    Frl_draw_rectangle_v,    3, "Draw a rectangle but vector2");
    deffun(env, "rl/get-screen-width",    Frl_get_screen_width,    0, "Get screen width");
    deffun(env, "rl/get-screen-height",   Frl_get_screen_height,   0, "Get screen height");

    printf("Initialized rl module\n");
    return 0;
}