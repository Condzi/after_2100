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
            * check if all needed paths exist (data folder, logs folder...)
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
    * Jai's `defer`
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
    * (is it a good idea tho?) log in AppData folder, see `std::getenv( "appdata" )` for specific path **`// IT SOMETIMES FAILS`**
        * Try the logging to opened `notepad.exe` instance thing from [here](http://kylehalladay.com/blog/2020/05/20/Rendering-With-Notepad.html) (but probably it's too hacky-whacky to use)
    * custom indentation looks cool 
----
# No hotloading - we can just call reload command
* **Hotloading** variables from a file (we can use Windows ability to notify us when the file changes)
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
    * Scene has additional `.scene_resources` file associated with it that holds resources that it needs

----
* **Resource Manager**
    * ToDo: We gonna need need handling of texture atlases.
    * ToDo: Color values? That we can set in config file. Different enemies could have different colors but same texture for example.
    * Resources are:
        * Animations (Sprite sheet + frame size + play speed)
        * Textures
        * Fonts
        * Music
        * Sound
        * Localized Strings
    * Shaders are actually pretty common so let's have them for entire lifetime? (add them to `default.scene_resources`)
    * Because we specify every texture size, we may take advantage of that and generate correctly sized "null" textures for them (if we fail to load them for some reason)
        * if we don't event have the texture mentioned in the config file, we should return an invalid texture of some kind... a checkboard?
    * Every scene has associated .scene_resources file in which it specifies what resources it needs
    * we have `default.scene_resources` wchich specifies most frequent data that should be loaded all the time (something like most common enemy texture, player texture, ui textures, default font)
    * Note how many resources are currently loaded
    * have a list of all resources that we can request (as a hash table)
    * have a way to reload all resources
    * we want to free unused resources somehow
    * how we can provide fast lookup + reload
        * when we're rendering we want to get texture ids quickly. 
        * maybe we can return a watcher-like object that updates its content every frame? Then we would call at the end of the frame to check if we have to update our value, or maybe even just do `resource.gl_resource_id = manager.get_resource<Texture_ID>(resource.id_in_resource_manager)`. That structure would look like this:
```cpp
struct GL_Resource
{ 
    // sounds in SFML implementation use ids like that too?
    // We can't use this approach only for localized strings i think. But we're setting them very seldom so
    // we don't actually need a very quick lookup times?
    GLUint gl_resource_id; // texture, shader. maybe something else if we want to reuse it
    s32 id_in_resource_manager;
}
```
    
----
* **Rendering** 
    * `Renderer` loops over `Cold` data in `Entity`: the `Render_Info` field. **`// Maybe add Render_Info as separete field alongside with Cold and Hot?`**
    * Since we'll be loopng over every entity type individually, we may use some ifs when rendering stuff and setting up matrices depending on the entity type (according to the field stored in `Hot`)
    * `Render_Info` contains:
        * vao, vbo, ebo **`// have one EBO for every sprite/quad`**
        * amount of elements
        * vertex size (we may use 3D positioning system for ships, but usually we just want 2D. We may also want to use texture without tint/coloring.)
        * drawing layer
        * drawing group? (GUI, Debug Console, Game, Menu...?) depending on that we order the draw calls
            * Also, the GUI, Menu and Debug Console shouldn't use the camera matrix, so it's usefull to have it in the different group.
        * texture id (not the OpenGL one)
        * compiled shader id (not the OpenGL one, we may want to hotload it)
        * drawing type -- is it element draw call, or array one?
        * drawing arrays info (vertices count and type)
    * Camera stuff
        * following a certain path
        * shakes
    * Instantiated rendering / render N vertices at one time thing?
        * True Type Font Rendering
        * Bitmap Font rendering (for dev console; maybe just use TTF^?)
    * Vertex and fragment shaders in one file (I've seen a macro trick to achieve this in GLSL)

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
----
* **Random variables**
    * Maybe in Context?
    * Use some simply function, not the slow standard one. Maybe we can depend on current system time and that's it. We don't need any hardcore accurate stuff, really

----
* **Performance profiler**
    * Add plotting to graph at runtime (like jblow's Sokoban)
        * render using `GL_LINES` and update every 1ms, with a scanline sortof thing
    * Just profile few important functions to not affect performance
    * Dump data at the end of every frame, like the logger does. This way we don't do file output all the time.
----
* **Collisions**
    * store collision info (about what collided with what) in global array which will be updated every frame. The collision detection should also happen somewhere at the end of the upated of the entities, so we won't be making weird situations. (*frame start **->** update entities **->** delete old collision info **->** check collisions **->** frame end **->** frame start **->** ...*)
----
* **Planets**
    * should have a sphere of influence (path conics theory)
    * we should also define it's mass in not multiplied value. For example if we define it as `2.24`, in game it is `2.24e15` or something like that.
    * are described by a `planets.variables` kind of file
    * we can compose levels/scenes using them like this:
    ```
    # other stuff related to scene...
    :/Planets
    # meta-info of planet where player appears
    starting_planet planet_a
    planet_a pos_x pos_y
    ```
    * we're not using any real math for orbital velocity
    * we have to specify it's texture, maybe animation or other effects
    ```
    :/planet_a
    texture planet_a
    # debatable
    particles particle_a 100
    ```
----
* **Gameplay**
    * One, giant map with several different sized planets (in opposite to the level based approach where we had 2-3 planets in each level)
        * This approach also gives us more possibilities for complicated behavior.
        * **However**, distances between planets in the solar system would've been smaller. The initial approach of condensed celestial bodies seems to be better?
    * Enemies should have recorded movement? Since it'd be hard to program. The planet movement won't change, so if we just record me playing as the enemy and then playing it back in the game, it'd be nice! 
        * Serialize initial position, starting time and velocity. Then we just have to save firing time and direction. This is all the data we really need to do AI behavior.
        * **BUT** what about fighting? How should they approach the player? Maybe the puzzle of getting to the right place is sufficient enough, we don't have to fight with them? This seem to be a story problem which we need to solve.

----
**Bother with this later:**
* Art
* Story
* GUI
* Serialization
* Signals? To notify others about stuff like collision
        * Internationalization
* Editor

# **Game**
Use it only as entry point. All code is in the `engine` project.