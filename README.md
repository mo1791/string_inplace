===================================\
string with inplace storage. [Test](https://godbolt.org/z/MvTbrxqo7)

Compatibility
-------------
inplace_string<N, CharT, Traits> implements C++17's std::string interface, plus:
  * `max_size()` and `capacity()` are `constexpr`
  * `inplace_string` can be constructed from `const CharT(&)[M])`, allowing a compile-time error if the input exceeds the maximum capacity

