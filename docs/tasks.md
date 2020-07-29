# **Current Tasks**
## **Text rendering**

**DONT FORGET THE KERNING PAIRS**

Font class should be just a container for textures of various sized characters.
Select the sizes depending on the current resolution. There should be at lest 3 main
sizes: small, medium and large. Calculate them depending on the screen height.

When constructing a text render info structure we shall pass the size enum, font and 
utf-8 string. 

UTF_8_String class should mimic the CSring class, but with wchar_t values (or u16 values?).
It should be part of the internationalization infrastructure. 
We will also need some `returning ascii_to_utf(CString) -> UTF_8_String` function for
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
returning construct_text("Chrząszcz!"_utf8, cascadia, Text_Size::Small) -> Render_Info;
```


* try libschrift. If not fiesable, do bitmap fonts.

* parse bitmap fonts. that's all. format: https://www.angelcode.com/products/bmfont/doc/file_format.html#bin
    * we probably can skip few blocks there
* cascadia code mono is for dev console, oxanium or other will be for gameplay
* set text drawing group correctly to GUI
----
## **other**
* default allocator unit test
    * probably will look like bitset UT
