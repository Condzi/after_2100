# ToDo
* set allocator type in bitset; consider adding it as a template? because sometimes we want to use stack, so if we pass special class as a parameter, let's say like that: `Bitset<16, Stack_Allocator>` we can use stack. But this way we're breaking the schemantics of using `bitset.allocator = my_allocator` so IDK.
* move some unit tests from `con_engine` and write few new -- we probably want to check if we're correctly allocating using `Default_Allocator` 
* data structures should have allocator field that default to `Default_Allocator`
```cpp
struct Array
{
    Allocator* allocator = Context.default_allocator;
};
```
* Logger class:
    * Global, doesn't send the log data anywhere, just holds it in Temporary Buffer
    * so you have to manually send the data to console or file

```cpp
// to use macro or to not use a macro?
#define con_log( ... ) G_Logger.log( __VA_ARGS__ )
// call before Temporary Buffer clear!!! maybe at the end of game frame for example
void flush_logger()
{
    // Ptr + size so it's convinient to use CString
    CString data = G_Logger.data();

    log_to_file(data);
    // We can probably add some ifdefs here -- we don't want to log to console when we're in
    // the release mode. We maybe don't want to log to dev terminal either.
    log_to_console();
    log_to_dev_terminal();
    // do other stuff with it?

    // logger data is automatically cleared because it's stored in temporary buffer
}
```