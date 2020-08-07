

#include <bx/uint32_t.h>
#include "common.h"
#include "bgfx_utils.h"
#include "imgui/imgui.h"

#include <renderer/renderer.h>
#include <ui/ui.h>

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

		renderer::init(args, width, height);
		ui::init();
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
			ui::render(m_width, m_height, m_mouseState);
			renderer::render(m_width, m_height);

			return true;
		}

		return false;
	}

	entry::MouseState m_mouseState;

	uint32_t m_width;
	uint32_t m_height;
	uint32_t m_debug;
	uint32_t m_reset;
};

} // namespace

ENTRY_IMPLEMENT_MAIN(
	  DSSequencer
	, "ds_sequencer"
	, ""
	, ""
	);
