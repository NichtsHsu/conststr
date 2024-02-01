# Conststr

Operate strings in constant context.

For example:

```cpp
#include "conststr.hpp"
 
using conststr::cstr;
using namespace conststr::literal;

constexpr auto hello = "hello"_cs;
constexpr auto world = "world"_cs;
constexpr auto msg = hello + ' ' + world + '!';

static_assert(msg == "hello world!"_cs);

constexpr auto newmsg = msg.uppercase()    // to "HELLO WORLD!"
                            .pop()         // to "HELLO WORLD"
                            .erase(4)      // to "HELL WORLD"
                            .erase(8)      // to "HELL WORD"
                            .append('?');  // to "HELL WORD?"

static_assert(newmsg == "HELL WORD?"_cs);
```

Check the [document](https://conststr.docs.nihil.cc/) or [tests](https://github.com/NichtsHsu/conststr/tree/master/tests) to learn more.
