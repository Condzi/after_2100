# **Important**

* move some unit tests from `con_engine` and write few new -- we probably want to check if we're correctly allocating using `Default_Allocator` 
* check where we have to call constructors and see if we can remove ctors.

# **Quality of life**
* add `con_fatal_assert` when `con_assert` is not suitable and causes recursion. Just prompt "fatal crash! see "log-*time*.txt""
    * current `con_assert` is bad because it continues execution after failing. This is bad since we might not be able to get to the `shutdown` stage of the engine. It's double bad because of way we're logging stuff. When we crash, we're losing all the log data! At least we're storing it in the `Logger` so we can dump it upon crashing, I guess?
* rename `CString::size` to `length` 
* remove `Stack_Allocator`?
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
* dev_console prints blanks lines from the bottom instead from the top when scrolling and we're at the end of the written lines
    * just follow how many lines we have saved and don't go past that number. If it's bigger than buffer size - ignore it.
* dev_console causes crash when scrolling too fast using touchpad?
* sometimes we may need to call ctors. either use new and delete OR inplace new OR initialize everything to 0. If we do that, we can simply memset(0) everytime we allocate new memory.
* if orbit is out of screen bounds the game crash (investigate when exactly and why)
* we may introduce bugs because of copying the `Array` somewhere. investigate that.
