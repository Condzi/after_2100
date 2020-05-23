# ToDo
* implement `Default_Allocator` stuff (allocate one block and manage it)
* data structures should have allocator field that default to `Default_Allocator`
```cpp
struct Array
{
    Allocator* allocator = Context.default_allocator;
};
```