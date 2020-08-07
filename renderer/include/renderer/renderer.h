#include "bgfx_utils.h"
#include "entry/entry.h"

namespace renderer
{
	static constexpr auto DEBUG = BGFX_DEBUG_TEXT;
	static constexpr auto RESET = BGFX_RESET_VSYNC;

	void init(const Args& args, const uint32_t& width, const uint32_t& height);

	void render(const uint32_t& width, const uint32_t& height);
}