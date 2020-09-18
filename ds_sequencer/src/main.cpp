
#include <renderer/renderer.h>
#include <base/components/basic.h>
#include <ui/ui.h>

#include "bgfx_utils.h"
#include <bx/uint32_t.h>
#include "common.h"
#include <entt/entt.hpp>
#include <glm/mat4x4.hpp>
#include "imgui/imgui.h"
#include "entry/input.h"

#include <iostream>
#include <string>
#include <variant>


#define IMGUI_INCLUDE_IMGUI_USER_H

namespace
{

class DSSequencer : public entry::AppI
{
public:
	DSSequencer(const char* _name, const char* _description, const char* _url)
		: entry::AppI(_name, _description, _url)
	{
	}

	void init(int32_t argc, const char* const* argv, uint32_t width, uint32_t height) override
	{
		Args args(argc, argv);

		m_width = width;
		m_height = height;
		m_debug = renderer::DEBUG;
		m_reset = renderer::RESET;
		
		renderer::init(args.m_type, args.m_pciId, width, height);
		ui::init();

		registry.emplace<component::Transform>(registry.create(), glm::mat4(1.0f));
	}

	virtual int shutdown() override
	{
		imguiDestroy();

		// Shutdown bgfx.
		bgfx::shutdown();

		return 0;
	}

	bool update() override
	{
		if (!entry::processEvents(m_width, m_height, m_debug, m_reset, &m_mouseState) )
		{
			auto const* ch_ptr = inputGetChar();
			

			ui::render(registry, m_width, m_height, m_mouseState, ch_ptr ? *ch_ptr : -1);
			renderer::render(registry, m_width, m_height);
;

			return true;
		}

		return false;
	}

	entry::MouseState m_mouseState;

	uint32_t m_width;
	uint32_t m_height;
	uint32_t m_debug;
	uint32_t m_reset;

	entt::registry registry;
};

} // namespace

ENTRY_IMPLEMENT_MAIN(
	  DSSequencer
	, "ds_sequencer"
	, ""
	, ""
	);
