#pragma once
#include "entry/entry.h"

#include <entt/entt.hpp>

namespace ui
{
	void init();

	void render(entt::registry &, const uint32_t&, const uint32_t&, const entry::MouseState&, int ch);
}