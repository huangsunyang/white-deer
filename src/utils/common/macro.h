#define VA_NARGS_IMPL(_1, _2, _3, _4, _5, _6, _7, _8, _9, N, ...) N
#define VA_NARGS(...) VA_NARGS_IMPL(__VA_ARGS__, 9, 8, 7, 6, 5, 4, 3, 2, 1)

#define FORMACRO0(x) FOREACH(x)
#define FORMACRO1(x, ...) FOREACH(x)
#define FORMACRO2(x, ...) FOREACH(x), FORMACRO1(__VA_ARGS__)
#define FORMACRO3(x, ...) FOREACH(x), FORMACRO2(__VA_ARGS__)
#define FORMACRO4(x, ...) FOREACH(x), FORMACRO3(__VA_ARGS__)
#define FORMACRO5(x, ...) FOREACH(x), FORMACRO4(__VA_ARGS__)
#define FORMACRO6(x, ...) FOREACH(x), FORMACRO5(__VA_ARGS__)
#define FORMACRO7(x, ...) FOREACH(x), FORMACRO6(__VA_ARGS__)
#define FORMACRO8(x, ...) FOREACH(x), FORMACRO7(__VA_ARGS__)
#define FORMACRO9(x, ...) FOREACH(x), FORMACRO8(__VA_ARGS__)

#define FORMACRO_IMPL2(i, ...) FORMACRO##i(__VA_ARGS__)
#define FORMACRO_IMPL(i, ...) FORMACRO_IMPL2(i, __VA_ARGS__)
#define FORMACRO(...) FORMACRO_IMPL(VA_NARGS(__VA_ARGS__), __VA_ARGS__)