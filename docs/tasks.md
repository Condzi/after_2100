# Renderer notes
* Just loop on entities in entity manager, get visible ones. Then sort by their layer, then render. Note that we also need its type data to know what matrices set how etc.

```cpp
for(s32 i = 0; i < stuff_to_render.size(); ++i ) {
    if(stuff_to_render[i].drawing_group == SOME_UNUSUAL_SPRITES) {
        auto some_data_to_shader = entity_manager.enemies_type_b.get(stuff_to_render[i].unique_parent_id );

        set_shader_data("field", some_data_to_shader);
    }

    render(stuff_to_render[i].elements_count, stuff_to_render[i].vao)
}
```

# **Current Tasks**
* do a basic sprite renderer
