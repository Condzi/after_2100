# **Dependencies**
* catch2 -- unit tests
---
We'll use a fasade pattern to hide these?
* GLFW -- window creation
* GLAD -- OpenGL functions loader
* GLM  -- Vector and matrix math
* stb_image -- textures loading (**define own malloc/free/realloc! also, `#define STBI_ONLY_PNG`**)
* ???  -- sound
* ???  -- collision detection (Box2D? tinyHeaders?)

# **Engine**
* We don't need RTTI and exceptions. However, we may want to use some stuff from standard C++ library (probably mostly from `<algorithm>`)
    * however, we may want to put `<algorithm>` stuff in `dependencies` because we want more usable return values, not just iterators.
----
* We're not going to use threads heavly. Probably only in few loops? But must possibly - none at all.
* Don't forget to call `std::ios_base::sync_with_stdio(false)` thing
----
* Configure engine using macros -- `macro_config.hpp`?
    * Memory block size
    * temporary storage size
    * assets paths
    * graphics debug / global debug flags
----
* Typedefs
    * s32, u64, byte?....
    * global / file_local etc. macros? (use unnamed namesapces)
    * `#define constant auto const`?
    * `#define compile_constant inline static constexpr auto`?
    * Jai's `defer`?
    * Use own string literal `_cs` that converts null-terminated c-string to our own data + size structure (see `CString` class)
----
* Memory Allocation
    * `Temporary Storage`
    * Basic global memory pool
----
* Global context structure that contains:
    * *?Entity manager?*
    * *?Resource manager?*
    * basic allocator
    * temporary storage allocator
    * logger
    * hotloaded variables?
----
* Basic 2D Renderer
    * We don't need to load fancy shaders, do we?
        * But if we do, treat them as resources and reload them on file change.
    * Camera stuff
        * following a certain path
        * shakes
    * Textured Quad
    * Colored & Textured Quad
    * Instantiated rendering / render N vertices at one time thing
        * True Type Font Rendering
        * Bitmap Font rendering (for dev console; maybe just use TTF^?)
----
* Logger, just one.
    * (is it a good idea tho?) log in AppData folder, see `std::getenv( "appdata" )` for specific path
        * Try the logging to opened `notepad.exe` instance thing from [here](http://kylehalladay.com/blog/2020/05/20/Rendering-With-Notepad.html) (but probably it's too hacky-whacky to use)
    * Built-in developer console
    * basic developer commands like `draw_hitboxes`
        * this commands should affect hotloaded variables? should they overwrite them?
        * Use instanciated rendering for stuff like `draw_hitboxes`
    * custom indentation looks cool (see `The Wintess` logs -- more indent, deeper the log?)
----
* Hotloading variables from a file (use Windows ability to notify us when the file changes)
    * Hotloading of resources
        * Texture and shader ( do we want shaders? ) is just OpenGL id?
        * Figure out how to handle TTF 
        * Figure out how to handle sounds (low priority)

----
**Bother with this later:**
* Components as SoA
* Entities as containers of handles to Components?
* GUI
* Signals? To notify others about stuff like collision
* Performance Profiler
* Internationalization
* Editor

# **Game**
* Has defined entry point
    * This is the place where we code our Application class that handles initialization of all that stuff that we just listed in Engine (allocation, resource loading...)
* Consist of scenes... (todo)