# **Current Tasks**

* add font loading and add font stuff to `Prepared_Resources`.
    Maybe we should think about font class again. Should it be a resource which is hold
    in the `Prepared_Resources`, or it's a static resource which is loaded once, at the app startup?
    I think this would be easier approach since fonts are not that heavy (it's just a bunch of textures after all)
    and loading it everytime we want to get to the main menu seems kinda tidious... right?
    Therefore fonts should be loaded only once (their size should be recalculated, and thus the textures recreated
    on every window/screen resolution change).

    Should `Font` object be even used in game programming? All we want to really do is to say "Look, here is a
    string that I want you to render. Here is the size, here is the font name. Here is the size and color.". Maybe
    small font handles (ids which are just hashed `CString`s) would be OK? The kerning and line spaceing stuff
    will happen in the `construct_text` sort of function.

    Speaking of which, we should also return the created text bounds. Maybe just as additional paramater accept `v2& bounds` or
    even a pointer, to indicate that this is an 'In' type of parameter (Microsoft style it is, I think). Or try multiple return 
    values with `auto` structured-bindings and unnamed struct or something. Like that:
    `auto[render_info, text_bounds] = construct_text(...)`.

    However, the whole idea of font that has two "cases", as I would name it, is and example given by Mike Acton about cache efficiency.
    We should *know* when we need singe-sized font, I think... Buut it's only font... Buut it may be more elegant to have `Mono_Sized_Font` 
    and `Dynamic_Font`!
    
* add `construct_text`
* remove `Font_Test` entity code and all it's belongings (in `Entity_Manager` and `Player`)

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
**I don't think we would benefit from this approach. One color for entire text is more fiesable.**

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
