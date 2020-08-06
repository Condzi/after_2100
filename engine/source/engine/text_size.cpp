#include <engine/text_size.hpp>

#include <engine/context.hpp>
#include <engine/window.hpp>

namespace con
{
void update_text_sizes()
{
	constant H = static_cast<f32>( Context.window->height() );

	// It's a linear function.
	// 768  =>  12
	// 1080 =>  16
	Context.text_sizes[Text_Size::Developer_Console] =( 1/72.0f ) * H + ( 28 / 13.0f );
	Context.text_sizes[Text_Size::Menu_Button] = 72;
}
}