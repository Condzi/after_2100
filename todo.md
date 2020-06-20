# **todo**
* do a basic sprite renderer
* figure out ttf fonts
* do a debug console (without editing for now, just one color messages + scrolling)
* add config file hotloading (by checking every X frames (100?) if the file change date matches our and if not, reloading it)
* we need better info about how parse of the cfg file went -- did it failed because file didint open or because of some human error?
* Config_File::parse should return a bool instead of asserting.
* use WinMain for release mode
    * and for debug too, but we don't have dev console yet.
* add random number generator (see comment in plan.md file. we may want to use a simple approach instead of using C++'s)
* add `Allocator::realloc`
* think about how to allocate a Game State. Should we save the individual positions of everything? Or maybe only of dynamic entities? Does camera count?
    * What **is** a game state?
* set allocator type in bitset; consider adding it as a template? because sometimes we want to use stack, so if we pass special class as a parameter, let's say like that: `Bitset<16, Stack_Allocator>` we can use stack. But this way we're breaking the schemantics of using `bitset.allocator = my_allocator` so IDK.
    * Idea about stack allocator: 
```cpp
#define CON_STACK_ALLOCATOR_SIZE 1024 // be carefull -- we don't want stack overflow
struct Stack_Allocator
{
    // works just like the default allocator?
    byte stack[CON_STACK_ALLOCATOR_SIZE];
};
```
* move some unit tests from `con_engine` and write few new -- we probably want to check if we're correctly allocating using `Default_Allocator` 
* better indentation handling in the logger. Consider adding a "current_indent" variable that we can change that is the default one for logging. This
    way we could have less cluttered looking calls and less indentation problems. However, this is a rahter low priority thing.
* Gamepad support