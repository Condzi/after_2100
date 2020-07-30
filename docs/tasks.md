# **Current Tasks**
## **Text rendering**

**DONT FORGET THE KERNING PAIRS**

Font class should be just a container for textures of various sized characters.
Select the sizes depending on the current resolution. The sizes should be dynamic and not explcit. I mean -- we should use some kind of `get_size_for(Text_Size::Developer_Console)` or `Text_Size::Menu_Button` etc. Don't forgeet to set a minium size too! (8, 16?)

When constructing a text render info structure we shall pass the size enum, font and utf-8 string. 

UTF_8_String class should mimic the CString class, but with wchar_t values (or u16 values?).
It should be part of the internationalization infrastructure. 
We will also need some `returning cstring_to_utf(CString) -> UTF_8_String` function for
creating text in the code, for example for developer console.

Does every vertex should have own text color? Or maybe we should have one color per text object?
If we would do the individual approach we would probably waste some space but we could do some cool
message coloring in the developer console (to highlight warning / errors for example). However, this
would require rework of the logging system.
I don't think we would benefit from this approach. One color for entire text is more fiesable.

We should think about line breaks and measuring the text. Sometimes we may not be able to fit the entire
line on the screen, therefore we need to add (automatic?) adding of newline. Maybe just add additional
parameter `s32 chars_per_line = 128` which will add `\n` every 128 chars?
Also, knowing the final width and height will be usefull for creating buttons or text boxes etc. 

```cpp
returning construct_text("Chrząszcz!"_utf8, menu_font, Text_Size::Menu_Button) -> Render_Info;
```

* cascadia code mono is for dev console, oxanium or other will be for gameplay
* set text drawing group correctly to GUI
----
## **other**
* default allocator unit test
    * probably will look like bitset UT
