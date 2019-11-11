/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/
#pragma once

namespace con
{
class Area;
}

namespace con::priv
{
// Observer pattern. Areas are registrating and unregistraing in their ctors/dtors.
// Checks if areas are overlaping in O(n^2) complexicity; calls s_area_entered.
// @Performance: maybe it could be faster by finding min and max collision_layer? I think it
// still may be O(n^2).
class Area_Overleaping_Checker final
{
public:
	void register_area( Area* const area_to_register );
	void unregister_area( Area* const area_to_unregister );

	void update();

	static Area_Overleaping_Checker& get_instance();

private:
	std::vector<Area*> areas;
};
}

#define G_Area_Overleaping_Checker con::priv::Area_Overleaping_Checker::get_instance()