#ifdef VERTEX_SHADER
layout (location=0) in vec2 l_position;
layout (location=1) in vec2 l_texture_coords;
layout (location=2) in vec4 l_color;

out vec2 texture_coords;
out vec4 color_base;

uniform mat4 u_mvp_mat; // projection * view * model

void main()
{
	gl_Position    = u_mvp_mat * vec4( l_position.xy, 0.1f, 1.0f );
    texture_coords = l_texture_coords;
    color_base     = l_color;
}
#endif

#ifdef FRAGMENT_SHADER
in vec2 texture_coords;
in vec4 color_base;

out vec4 color;

uniform sampler2D text_texture;

void main()
{
    float alpha = 1 - texture(text_texture, texture_coords).r;
    color = color_base;
    color.alpha *= alpha;
}
#endif