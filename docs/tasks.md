# **Current Tasks**

* Debugging in fullscreen causes the window to hang when error occurs. We can partially deal with it by closing the window before doing DebugBreak call.
* add `Find_Result::not_found()` and use it instead `found()` because it's more commonly used.
* log the hardware info (RAM, max texture size, this kind of stuff)
* default allocator unit test
    * probably will look like bitset UT
