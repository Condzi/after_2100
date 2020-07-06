# **Important**
* add text rendering
    * add ttf font
        * lookup SFML (and other) implementation
* add developer console (text printing and scrolling only for now)
* move some unit tests from `con_engine` and write few new -- we probably want to check if we're correctly allocating using `Default_Allocator` 
* figure out ttf fonts

# **Quality of life**
* add support for const char* const in sprint because right now we have to do `CString{ CON_SOME_CSTRING_MACRO }`
    * well, actually if we don't do it like that we lose compile time knowledge of the size. We can only do this: `FOO ""_cs` 
* do a debug console (without editing for now, just one color messages + scrolling)
    * maybe use cascadia monospace font?
    * we'll need ascii text input handling later
* better indentation handling in the logger. Consider adding a "current_indent" variable that we can change that is the default one for logging. This way we could have less cluttered looking calls and less indentation problems. However, this is a rahter low priority thing.
    * Also, I see that I most often use indentation of 1.
* log the hardware info (RAM, max texture size, this kind of stuff)
* use WinMain for release mode
    * and for debug too, but we don't have dev console yet.
* **ADD** `.reload()` which reloads resources and then searches for entities that use them. Then assigning the new resource in place of the old one. (Just like in the example above)

# **For later**
* don't forget to **add licenses**!
* Gamepad support
* add random number generator (see comment in plan.md file. we may want to use a simple approach instead of using C++'s)