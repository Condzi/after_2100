# **Current Tasks**
* rethink logger.
* add developer console (text printing and scrolling only for now)
    * just simply display text using the new `construct_text` function.
    * adapt solutions from the legacy code. it worked quite nicely. 

# **Dev Console**
We initialize it in two steps. 
* First, set up the buffer to save the messages. Do this as early as possible to not miss the initialization logs. (probably still miss some?)
* Second, initialize the visuals. Text rendering, backgrounds because we need OpenGL context for that.

We could skip that by flushing after initialization of the `Window`, but that would be not so robust. We would have higher chance of missing logs if a sudden crash occurs. That's why we're flushing as frequent as possible.

We need a simple rectangle shapes to create console background.
* Just add a colored vertex structure. Then `create_rectangle_shape`. Just remember that we're dealing with origin point in the center of the rectangle. Remember to resize the size accordingly to the window's.

The `Dev_Console` won't use entities at all. The `Renderer` will render its `Render_Info`s at the end of the frame. It's just a special case. There is no need for creating entities in the `Entity_Manager` for that. We need one `Render_Info` for the text, one for the background, for now. We'll need at least two more for the input field.

----
## **other**
* default allocator unit test
    * probably will look like bitset UT
