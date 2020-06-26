# **Important**
* **make bitset use allocator instead of stack**
* do a basic sprite renderer
* Config_File::parse should return a bool instead of asserting.
    * we need better info about how parse of the cfg file went -- did it failed because file didint open or because of some human error?
* move some unit tests from `con_engine` and write few new -- we probably want to check if we're correctly allocating using `Default_Allocator` 
* set allocator type in bitset; consider adding it as a template? because sometimes we want to use stack, so if we pass special class as a parameter, let's say like that: `Bitset<16, Stack_Allocator>` we can use stack. But this way we're breaking the schemantics of using `bitset.allocator = my_allocator` so IDK.
    * Idea about stack allocator: 
```cpp
#define CON_STACK_ALLOCATOR_SIZE 1024 // be careful -- we don't want stack overflow
struct Stack_Allocator
{
    // works just like the default allocator?
    byte stack[CON_STACK_ALLOCATOR_SIZE];
};
```
* add growing array like a vector and use it in config file parser and scene resources info file parser. Now it's rather clumsy and overcomplicated because we have to count the stuff first, and then create an array and again read the same stuff. It's almost double the work!
    * add `Allocator::realloc`
* figure out ttf fonts

# **Quality of life**
* add support for const char* const in sprint because right now we have to do `CString{ CON_SOME_CSTRING_MACRO }`'
* do a debug console (without editing for now, just one color messages + scrolling)
    * maybe use cascadia monospace font?
    * we'll need ascii text input handling later
* better indentation handling in the logger. Consider adding a "current_indent" variable that we can change that is the default one for logging. This way we could have less cluttered looking calls and less indentation problems. However, this is a rahter low priority thing.
    * Also, I see that I most often use indentation of 1.
* log the hardware info (RAM, max texture size, this kind of stuff)
* use WinMain for release mode
    * and for debug too, but we don't have dev console yet.
* add `load_entire_file_binary( path ) -> Array<char>` ( or `<byte>`? )

# **For later**
* don't forget to **add licenses**!
* Gamepad support
* add random number generator (see comment in plan.md file. we may want to use a simple approach instead of using C++'s)