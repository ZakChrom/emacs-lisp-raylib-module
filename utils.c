#include <stdbool.h>
#include <emacs-module.h>
#include <raylib.h>

#define EXPECT_NARGS(n) { \
    if (nargs != (n)) { \
        env->non_local_exit_signal(env, env->intern(env, "wrong-number-of-arguments"), env->make_integer(env, nargs)); \
        return env->intern(env, "nil"); \
    } \
}

typedef emacs_value (*EmacsFunc)(emacs_env *env, ptrdiff_t nargs, emacs_value* args, void *data);

// TODO: Min and max arity?
emacs_value deffun(emacs_env* env, const char* name, EmacsFunc func, int arity, const char* desc) {
    return env->funcall(env, env->intern(env, "defalias"), 2, (emacs_value[]){
        env->intern(env, name),
        env->make_function(env, arity, arity, func, desc, NULL)
    });
}

bool is_cons(emacs_env* env, emacs_value val) {
    emacs_value ret = env->funcall(env, env->intern(env, "consp"), 1, (emacs_value[]){val});
    return env->is_not_nil(env, ret);
}

emacs_value car(emacs_env* env, emacs_value val) {
    return env->funcall(env, env->intern(env, "car"), 1, (emacs_value[]){val});
}

emacs_value cdr(emacs_env* env, emacs_value val) {
    return env->funcall(env, env->intern(env, "cdr"), 1, (emacs_value[]){val});
}

bool extract_color(emacs_env* env, emacs_value list, Color* out) {
    if (!is_cons(env, list)) {
        return false;
    }

    int r, g, b, a;
    int count = 0;

    while (env->is_not_nil(env, list)) {
        emacs_value head = car(env, list);
        emacs_value tail = cdr(env, list);

        int v = env->extract_integer(env, head);
        switch (count) {
            case 0: { r = v; } break;
            case 1: { g = v; } break;
            case 2: { b = v; } break;
            case 3: { a = v; } break;
        }

        count += 1;
        list = tail;
    }

    if (count != 4) {
        return false;
    }

    *out = (Color){ r&255, g&255, b&255, a&255 };

    return true;
}

bool extract_vector2(emacs_env* env, emacs_value list, Vector2* out) {
    if (!is_cons(env, list)) {
        return false;
    }

    double x, y;
    int count = 0;

    while (env->is_not_nil(env, list)) {
        emacs_value head = car(env, list);
        emacs_value tail = cdr(env, list);

        double v = env->extract_float(env, head);
        switch (count) {
            case 0: { x = v; } break;
            case 1: { y = v; } break;
        }

        count += 1;
        list = tail;
    }

    if (count != 2) {
        return false;
    }

    *out = (Vector2){ (float)x, (float)y };

    return true;
}
