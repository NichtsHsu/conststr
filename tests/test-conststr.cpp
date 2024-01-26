#include <functional>

#include "conststr.hpp"

using conststr::cstr;
namespace charutils = conststr::charutils;
using namespace conststr::literal;

int main() {
    constexpr auto hello = "hello"_cs;

    static_assert(hello == "hello"_cs);
    static_assert(hello != "HELLO"_cs);
    static_assert(hello > "hell"_cs);
    static_assert(hello < "helloo"_cs);

    constexpr auto hello_world = hello + ' ' + "world"_cs + '!';

    static_assert(hello_world == "hello world!"_cs);
    static_assert(hello_world.front() == 'h');
    static_assert(hello_world.back() == '!');
    static_assert(hello_world[5] == ' ');
    static_assert(hello_world ==
                  hello.append(' ').append("world"_cs).append('!'));
    static_assert(hello_world == hello.flatten(" "_cs, "world"_cs, "!"_cs));
    static_assert(hello_world ==
                  conststr::flatten(hello, " "_cs, "world"_cs, "!"_cs));
    static_assert(hello_world.pop() == "hello world"_cs);
    static_assert(hello_world.cut<5>() == "hello"_cs);
    static_assert(hello_world.insert(5, 'e') == "helloe world!"_cs);
    static_assert(hello_world.substr<3, 0>() == ""_cs);
    static_assert(hello_world.uppercase() == "HELLO WORLD!"_cs);
    static_assert(hello_world.uppercase().lowercase() == hello_world);
    static_assert(hello_world.replace<'o', '0'>() == "hell0 w0rld!"_cs);
    static_assert(hello_world.replace(hello_world.cbegin(),
                                      hello_world.cbegin() + 5,
                                      '?') == "????? world!"_cs);
    static_assert(hello_world.replace(0, "happy"_cs) == "happy world!"_cs);
    static_assert(hello_world.erase(4).erase(8) == "hell word!"_cs);
    static_assert(hello_world.find('l') == 2);
    static_assert(hello_world.rfind('l') == 9);
    static_assert(hello_world.find('f') == hello_world.npos);
    static_assert(hello_world.find("lo"_cs) == 3);
    static_assert(hello_world.find("loo"_cs) == hello_world.npos);
    static_assert(hello_world.find_if(charutils::isspace<char>) == 5);
    static_assert(hello_world.find_if(std::not_fn(charutils::isprint<char>)) ==
                  hello_world.npos);
    static_assert(hello_world.contains('w'));
    static_assert(!hello_world.contains('f'));
    static_assert(hello_world.start_with('h'));
    static_assert(!hello_world.start_with('H'));
    static_assert(hello_world.start_with("hell"_cs));
    static_assert(!hello_world.start_with("helo"_cs));
    static_assert(hello_world.end_with('!'));
    static_assert(!hello_world.end_with('d'));
    static_assert(hello_world.end_with("d!"_cs));
    static_assert(!hello_world.end_with("rd!"_cs));

    std::cout << __FILE__ ": all tests passed." << std::endl;
    return 0;
}