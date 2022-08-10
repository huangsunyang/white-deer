#include "catch2/catch_test_macros.hpp"
#include "chibi/eval.h"
#include "filesystem/localfilesystem.h"

using namespace WhiteDeer::Engine;

void dostuff(sexp ctx) {
  /* declare and preserve local variables */
  sexp_gc_var2(obj1, obj2);
  sexp_gc_preserve2(ctx, obj1, obj2);

  /* load a file containing Scheme code */
  auto localfs = GetLocalFileSystem();
  auto path = localfs->ToAbsolute("package/script/scheme/test.scm").string();
  obj1 = sexp_c_string(ctx, path.c_str(), -1);
  sexp_load(ctx, obj1, NULL);

  /* eval a C string as Scheme code */
  sexp_eval_string(ctx, "(display \"Hello world from Scheme.\n\")", -1, NULL);

  /* construct a Scheme expression to eval */
  obj1 = sexp_intern(ctx, "my-procedure", -1);
  obj2 = sexp_cons(ctx, obj1, SEXP_NULL);
  sexp_eval(ctx, obj2, NULL);

  /* release the local variables */
  sexp_gc_release2(ctx);
}

TEST_CASE("Chibi Scheme", "Chibi") {
  sexp ctx;
  sexp_scheme_init();
  ctx = sexp_make_eval_context(NULL, NULL, NULL, 0, 0);

  // add path
  {
    sexp_push(ctx, sexp_global(ctx, SEXP_G_MODULE_PATH), SEXP_VOID);

    auto localfs = GetLocalFileSystem();
    auto path = localfs->ToAbsolute("package/script/scheme/lib").string();

    sexp_car(sexp_global(ctx, SEXP_G_MODULE_PATH)) =
        sexp_c_string(ctx, path.c_str(), path.size());
    sexp_immutablep(sexp_global(ctx, SEXP_G_MODULE_PATH)) = 1;
  }

  sexp_load_standard_env(ctx, NULL, SEXP_SEVEN);
  sexp_load_standard_ports(ctx, NULL, stdin, stdout, stderr, 1);
  dostuff(ctx);
  sexp_destroy_context(ctx);
}