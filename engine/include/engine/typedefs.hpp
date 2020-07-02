#pragma once

#include <cstdint>

#include <engine/macro_config.hpp>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>

#include <glad/glad.h>

namespace con
{
//
// GLM stuff
//
using v2   = glm::vec2;
using v3   = glm::vec3;
using v4   = glm::vec4;
using mat3 = glm::mat3;
using mat4 = glm::mat4;


//
//	Typedefs for C++ types.
//
using u8  = std::uint8_t;
using s8  = std::int8_t;
using u16 = std::uint16_t;
using s16 = std::int16_t;
using u32 = std::uint32_t;
using s32 = std::int32_t;
using u64 = std::uint64_t;
using s64 = std::int64_t;

using f32 = float;
using f64 = double;

using byte = u8;
using gl_id = GLuint;

//
// Handy macros
//

#define unused( x )			( ( void )( x ) )
#define constant			auto const
#define compile_constant	static constexpr auto
#define file_scope			namespace
#define returning			[[nodiscard]] auto 

// 
//	Defer's macro stuff here
//


namespace priv
{
template <typename T>
struct Exit_Scope final
{
	T const lambda;
	Exit_Scope( T l ) : lambda( l ) {}
	~Exit_Scope() { lambda(); }
	Exit_Scope( Exit_Scope const& ) = delete;
	Exit_Scope& operator=( Exit_Scope const& ) = delete;
};

struct Exit_Scope_Help final
{
	template <typename T>
	Exit_Scope<T> operator+( T t ) const { return Exit_Scope{ t }; }
};

}
#define defer  constant CON_CONCAT( defer__, __LINE__ ) = con::priv::Exit_Scope_Help{} + [&]
//
// End of defer's macro stuff.
//

}