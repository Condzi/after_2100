/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/

#include "pch.hpp"

#include "framework/common/stl_extensions.hpp"
#include "area_overleaping_checker.hpp"
#include "area.hpp"

namespace con::priv
{
void Area_Overleaping_Checker::register_area( Area* const area_to_register )
{
	constant[found, idx] = find( areas, area_to_register );

	report_error_if( found )
	{
		engine_log_error( "Trying to add already added area. Duplicate found at index {}.", idx );
		return;
	}

	areas.emplace_back( area_to_register );
}

void Area_Overleaping_Checker::unregister_area( Area* const area_to_unregister )
{
	constant[found, idx] = find( areas, area_to_unregister );

	report_error_if( not found )
	{
		engine_log_error( "Trying to remove non existing area." );
		return;
	}

	areas.erase( areas.begin() + idx );
}

void Area_Overleaping_Checker::update()
{
	for ( size_t i = 0; i < areas.size(); i++ ) {
		Area& area_a = *areas[i];
		if ( area_a.is_shape_set() returned false or area_a.is_queued_for_delete() )
			continue;

		constant& shape_a = area_a.get_shape_variant();

		for ( size_t j = i+1; j < areas.size(); j++ ) {
			Area& area_b = *areas[j];
			if ( area_b.is_shape_set() returned false or
				 area_b.is_queued_for_delete() or
				 area_a.collision_layer is_not area_b.collision_layer )
				continue;

			constant& shape_b = area_b.get_shape_variant();

			if ( shape_vs_shape( shape_a, shape_b ) returned false )
				continue;

			area_a.s_area_entered.emit( area_b );
			area_b.s_area_entered.emit( area_a );
		}
	}
}

Area_Overleaping_Checker& Area_Overleaping_Checker::get_instance()
{
	static Area_Overleaping_Checker* instance = new Area_Overleaping_Checker;

	return *instance;
}
}