* add config loading and default config values
```cpp
struct Default_Config_Values final
{
    static constexpr f32 ups = 1 / 60.0f;
    static constexpr bool vsync = true;
    // window dimensions'n stuff
};
```
* add main game loop