# **Current Tasks**
* add developer console (text printing and scrolling only for now)
    * just simply display text using the new `construct_text` function.
    * adapt solutions from the legacy code. it worked quite nicely. 

# **Debug Console**
We initialize it in two steps. 
* First, set up the buffer to save the messages. Do this as early as possible to not miss the initialization logs. (probably still miss some?)
* Second, initialize the visuals. Text rendering, backgrounds.

We could skip that by flushing after initialization of the `Window`, but that would be not so robust. We would have higher chance of missing logs if a sudden crash occurs. That's why we're flushing as frequent as possible.

We need a simple rectangle shapes to create console background.
* Just add a colored vertex structure. Then `create_rectangle_shape`. Just remember that we're dealing with origin point in the center of the rectangle. Remember to resize the size accordingly to the window's.

The `Debug_Console` class will spawn entities (for background and probably input field in the future), but will not be an entity by itself. We will use a separate `Render_Info` which will be rendered only if the `dev_console` flag (in `local.variables` file) is set. This way we can initialize the class pretty early on and not miss many messages.

----
## **other**
* default allocator unit test
    * probably will look like bitset UT
