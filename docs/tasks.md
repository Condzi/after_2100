# **Current Tasks**
* copy the Bitset implementaiton from `gol`
* change the `temporary allocator` initialize function
* spend time on refactor and bugfixing!!!
* improve path rendering. fix the bug.

* add `con_fatal_assert` when `con_assert` is not suitable and causes recursion. Just prompt "fatal crash! see "log-*time*.txt""
    * current `con_assert` is bad because it continues execution after failing. This is bad since we might not be able to get to the `shutdown` stage of the engine. It's double bad because of way we're logging stuff. When we crash, we're losing all the log data! At least we're storing it in the `Logger` so we can dump it upon crashing, I guess?
* rename `CString::size` to `length` 
* remove `Stack_Allocator`?

----
## **other**
* default allocator unit test
    * probably will look like bitset UT
