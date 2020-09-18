#pragma once
#include "base/utils.h"
#include "bgfx_utils.h"
#include <entt/entt.hpp>
#include "entry/entry.h"

#include <string_view>
#include <variant>


namespace renderer
{

	static constexpr auto DEBUG = BGFX_DEBUG_TEXT;
	static constexpr auto RESET = BGFX_RESET_VSYNC;

	void init(const bgfx::RendererType::Enum type, const uint16_t pciId, const uint32_t& width, const uint32_t& height);

	void render(entt::registry&, const uint32_t& width, const uint32_t& height);

	namespace act
	{
		struct init
		{
			inline void exec(const bgfx::RendererType::Enum type, const uint16_t pciId, const uint32_t width, const uint32_t height)
			{
				renderer::init(type, pciId, width, height);
			}
		};

		struct render
		{
			inline void exec(const uint32_t width, const uint32_t height)
			{
				//renderer::render(width, height);
			}
		};
	}


	//using ActsTypeList = TypeList<act::init, act::render>;

	//static constexpr ActsTypeList actsTypeList;

}
