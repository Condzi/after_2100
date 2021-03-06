# **Important**

* move some unit tests from `con_engine` and write few new -- we probably want to check if we're correctly allocating using `Default_Allocator` 
* check where we have to call constructors and see if we can remove ctors.

# **Quality of life**

* add fps counter and frame time in the corner
* add performance profiler with diagrams and such
* don't use default parameters in functions like `initialize` (see: `Bitset::initialize` and `Temporary_Allocator`)
* log the hardware info (RAM, max texture size, this kind of stuff)
* add support for `const char*` const in sprint because right now we have to do `CString{ CON_SOME_CSTRING_MACRO }`
    * well, actually if we don't do it like that we lose compile time knowledge of the size. We can only do this: `FOO ""_cs` 
* use WinMain for release mode
    * and for debug too, but we don't have dev console yet.
* **ADD** `.reload()` which reloads resources and then searches for entities that use them. Then assigning the new resource in place of the old one. (Just like in the example above)

# **For later**
* for localized text we'll need a special text class. Just don't do the `Localized_String` approach from the legacy version. When needed, just loop over `Text` (or something like that) entities and update them.
* don't forget to **add licenses**!
* Gamepad support
* add random number generator (see comment in plan.md file. we may want to use a simple approach instead of using C++'s)

# **BUGS**
* we may introduce bugs because of copying the `Array` somewhere. investigate that.