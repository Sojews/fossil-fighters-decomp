# MSL C++ headers

A reconstruction of Metrowerks' MSL C++ library headers, taken from the
[Spirit Tracks decomp](https://github.com/zeldaret/st) (CC0). Changes for this game:

- `type_traits/intrinsics.hpp`: the trivial-member traits compare with `!= 0`, because this compiler
  version doesn't turn a `static const bool` initialized from a bit mask into 0 or 1.
- `memory`: `allocator::allocate` calls plain `operator new`.
- `vector`: `push_back` declares a temporary before its size check and calls `sub_push_back` directly, which
  is what puts the two tag temporaries in the order this game's code has them.
- `vector`: the capacity is held in a small `__vec_capacity` rather than `compressed_pair`, so that a vector's
  constructor inlines the way the game's do; and vectors of pointers share the `vector<void *>` code and have
  class iterators.
