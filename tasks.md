* think about resource manager and plan it
* actually do a decent resource manager


```cpp
// Every scene should have its own config file which specifies which exactly resources it needs. This way we won't have to touch code if we find a typo in name, just reload the resources from game level or relaunch it.

// we can make it more generic: (sprint("%.varibles", scene_name))
// where scene_name is a CString. Hence, we can even move this function
// out of the scene user code to the Scene Manager and make it happen in the
// manager.
resource_manager.prepare_from_file("scene1.variables"); 

// still in scene initialization function.
// We don't need ability to load/free stuff, only the actual resources
// are needed for creation of the entities.
// Maybe put it to the .prepare_from_file(..)?
// And set it to nullptr if the resource_manager frees stuff?
Context.prepared_resources = &resource_manager.prepared_resources;
```
# Updating textures after change
This feature will be useful only in developement -- we should disable
it in release build, I think. It should not be perfomed automatically since it would be very ineffective - it's a corner case, most of the time (99.999%) we won't be performing this action.
```cpp
// Just update every entity in the array that has texture!!!
// this thing applies to all other resources. We could even do this
// in separate thread at the beginning of the frame -- we would have plenty
// of time to update these values.
for(auto& cold : entities) {
if(ids_to_reload.contains( cold.render_info.texture.resource_id ))
{
    cold.render_info.texture.gl_id = resource_manager.get_texture(resource_id);
}
```

# Reloading
We don't want to free stuff we gonna use in the next scene (UI textures, fonts, sounds, player texture?). Therefore, we probably want to compare what scene needs vs what we have already loaded. This leads us to not having `manager.free_resources(...)` function at all - this behavior shall be handled by `.prepare_from_file` which should do the comparasion mentioned above.