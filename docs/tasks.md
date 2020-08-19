# **Current Tasks**
* adapt the code to use `con_check` with handing of failures if `con_assert` is too harsh.
    * all "core" utilities (`sprint`, allocators) should use `con_assert`, because we're using them in the `con_check`.
* copy the Bitset implementaiton from `gol`
* change the `temporary allocator` initialize function
* spend time on refactor and bugfixing!!!
* improve path rendering. fix the bug.

* rename `CString::size` to `length` 
* remove `Stack_Allocator`?

----
## **other**
* default allocator unit test
    * probably will look like bitset UT
