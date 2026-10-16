# MSL C++ headers

A reconstruction of Metrowerks' MSL C++ library headers, taken from the
[Spirit Tracks decomp](https://github.com/zeldaret/st) (CC0). Changes for this game:

- `type_traits/intrinsics.hpp`: the trivial-member traits compare with `!= 0`, because this compiler
  version doesn't turn a `static const bool` initialized from a bit mask into 0 or 1.
- `memory`: `allocator::allocate` calls plain `operator new`.
- `vector`: `push_back` declares a temporary before its size check and calls `sub_push_back` directly, which
  is what puts the two tag temporaries in the order this game's code has them.
