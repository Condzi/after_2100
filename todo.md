# ToDo
* think about how to allocate a Game State. Should we save the individual positions of everything? Or maybe only of dynamic entities? Does camera count?
    * What **is** a game state?
* set allocator type in bitset; consider adding it as a template? because sometimes we want to use stack, so if we pass special class as a parameter, let's say like that: `Bitset<16, Stack_Allocator>` we can use stack. But this way we're breaking the schemantics of using `bitset.allocator = my_allocator` so IDK.
* move some unit tests from `con_engine` and write few new -- we probably want to check if we're correctly allocating using `Default_Allocator` 
* data structures should have allocator field that default to `Default_Allocator`
```cpp
struct Array
{
    Allocator* allocator = Context.default_allocator;
};
```