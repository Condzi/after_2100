# **Dependencies**
* catch2 -- unit tests
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
* **Startup + Application class**
    * Startup
        * Memory
        * Context
        * Logger
        * Window
        * Splash screen stuff? 
        * Resource loading (+config file)
    * Running
    * Shutdown (just like startup but in reverse order)

    * Other stuff:
        * global timer started right before memory allocation?
        * fixed / independent timestep
---
* **Engine configuration** -- `macro_config.hpp`
    * Memory block size
    * temporary storage size
    * assets paths
    * graphics debug / global debug flags
----
* **Typedefs**
    * s32, u64, byte?....
    * global / file_local etc. macros? (use unnamed namesapces)
    * `#define constant auto const`?
    * `#define compile_constant inline static constexpr auto`?
    * Jai's `defer`? **`// Note: do we need it?`**
    * Use own string literal `_cs` that converts null-terminated c-string to our own data + size structure (see `CString` class)
----
* **Memory Allocation**
    * `Temporary Storage`
    * Basic global memory pool
----
* **Context** structure that contains:
    * *?Entity manager?*
    * *?Resource manager?*
    * basic allocator
    * temporary storage allocator
    * hotloaded variables?
    * game running time (useful for animations / random stuff)
    * some global flags? like "exit game" 
----
* **Logger**, just one.
    * (is it a good idea tho?) log in AppData folder, see `std::getenv( "appdata" )` for specific path
        * Try the logging to opened `notepad.exe` instance thing from [here](http://kylehalladay.com/blog/2020/05/20/Rendering-With-Notepad.html) (but probably it's too hacky-whacky to use)
    * custom indentation looks cool 
----
* **Hotloading** variables from a file (use Windows ability to notify us when the file changes)
    * Hotloading of resources
        * Texture and shader ( do we want shaders? ) is just OpenGL id?
        * Figure out how to handle TTF 
        * Figure out how to handle sounds (low priority)
    * We can just check every 100 frames for the last file change using `std::filesystem::last_write_time`
----
* **Entities**
    * Use Hold / Cold data approach. Store everything in arrays. See `hot_cold_entities.cpp` for better reference.
    * Don't use inheritance! And ECS. Just include the `Entity` using pointers and references.
    * `Entity Manager` holds arrays of entities + *one instance of player Entity*

----
* **Rendering**
    * `Renderer` loops over `Cold` data in `Entity`: the `Render_Info` field. **`// Maybe store pointer to that in the Cold, but the array be in the Renderer structure?`**
    * `Render_Info` contains:
        * vao, vbo, ebo **`// have one EBO for every sprite/quad?`**
        * amount of vertices
        * vertex size (we may use 3D positioning system for ships, but usually we just want 2D. We may also want to use texture without tint/coloring.)
        * drawing layer
        * drawing group? (GUI, Debug Console, Game, Menu...?) depending on that we order the draw calls
        * texture id (not the OpenGL one)
        * compiled shader id (not the OpenGL one, we may want to hotload it)
    * Camera stuff
        * following a certain path
        * shakes
    * Instantiated rendering / render N vertices at one time thing?
        * True Type Font Rendering
        * Bitmap Font rendering (for dev console; maybe just use TTF^?)

----
* Built-in **developer console**
    * basic developer commands like `draw_hitboxes`
        * this commands should affect hotloaded variables? should they overwrite them?
        * Use instanciated rendering for stuff like `draw_hitboxes`
----
* **Configuration file**
    * located in `data/launcher.vars`
    * settings like:
        * window size
        * vsync
        * key bindings **`// low priority`**
    * debugging stuff
        * startup level
        * allow developer commands 
        * and more!...
---
* **Random variables**
    * Maybe in Context?
    * Use some simply function, not the slow standard one. Maybe we can depend on current system time and that's it. We don't need any hardcore accurate stuff, really
----
**Bother with this later:**
* Resource Manager
* GUI
* Serialization
* Signals? To notify others about stuff like collision
* Performance Profiler
* Internationalization
* Editor

# **Game**
Use it only as entry point. All code is in the `engine` project.