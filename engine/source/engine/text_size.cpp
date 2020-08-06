#include <engine/text_size.hpp>

#include <engine/context.hpp>

namespace con
{
void update_text_sizes()
{
	// @ToDo: update text sizes depending on the resolution.
	// ... = 24 * (1 + screen_height * 0.085 ); or something like that
	
	Context.text_sizes[Text_Size::Developer_Console] = 12;
	Context.text_sizes[Text_Size::Menu_Button] = 72;
}
}