#include "renderer/renderer.h"

#include <base/components/basic.h>
#include <glm/gtc/type_ptr.hpp>
#include "camera.h"

#include "bgfx_utils.h"
#include "debugdraw/debugdraw.h"
#include "imgui/imgui.h"

namespace
{
	
}

namespace renderer
{

	void init(const bgfx::RendererType::Enum type, const uint16_t pciId, const uint32_t& width, const uint32_t& height)
	{	
		bgfx::Init init;
		init.type     = type;
		init.vendorId = pciId;
		init.resolution.width  = width;
		init.resolution.height = height;
		init.resolution.reset  = BGFX_RESET_VSYNC;
		bgfx::init(init);

		// Enable debug text.
		bgfx::setDebug(BGFX_DEBUG_TEXT);

		// Set view 0 clear state.
		bgfx::setViewClear(0
			, BGFX_CLEAR_COLOR|BGFX_CLEAR_DEPTH
			, 0x303030ff
			, 1.0f
			, 0
			);
		cameraCreate();
		cameraSetPosition({ 0.0f, 0.0f, 1.0f });

		//cameraSetVerticalAngle(0.0f);

		ddInit();
	}

	void render(entt::registry& reg, const uint32_t& width, const uint32_t& height)
	{
		// Set view 0 default viewport.
		bgfx::setViewRect(0, 0, 0, uint16_t(width), uint16_t(height));

		float view[16];
		cameraGetViewMtx(view);

		float proj[16];
		// Set view and projection matrix for view 0.
		{
			bx::mtxProj(proj, 60.0f, float(width) / float(height), 0.1f, 100.0f, bgfx::getCaps()->homogeneousDepth);

			bgfx::setViewTransform(0, view, proj);
			bgfx::setViewRect(0, 0, 0, uint16_t(width), uint16_t(height));

		}


		// This dummy draw call is here to make sure that view 0 is cleared
		// if no other draw calls are submitted to view 0.
		
		bgfx::touch(0);

		//// Use debug font to print information about this example.
		//bgfx::dbgTextClear();

		//bgfx::dbgTextPrintf(0, 10, 0x0f, "Color can be changed with ANSI \x1b[9;me\x1b[10;ms\x1b[11;mc\x1b[12;ma\x1b[13;mp\x1b[14;me\x1b[0m code too.");

		//bgfx::dbgTextPrintf(80, 10, 0x0f, "\x1b[;0m    \x1b[;1m    \x1b[; 2m    \x1b[; 3m    \x1b[; 4m    \x1b[; 5m    \x1b[; 6m    \x1b[; 7m    \x1b[0m");
		//bgfx::dbgTextPrintf(80, 11, 0x0f, "\x1b[;8m    \x1b[;9m    \x1b[;10m    \x1b[;11m    \x1b[;12m    \x1b[;13m    \x1b[;14m    \x1b[;15m    \x1b[0m");

		//const bgfx::Stats* stats = bgfx::getStats();
		//bgfx::dbgTextPrintf(0, 11, 0x0f, "Backbuffer %dW x %dH in pixels, debug text %dW x %dH in characters."
		//	, stats->width
		//	, stats->height
		//	, stats->textWidth
		//	, stats->textHeight
		//);
		DebugDrawEncoder dde;
		dde.begin(0);
		reg.view<component::Transform>().each([&](const auto& transform) {
			
			//bgfx::deb
			dde.pushTransform(glm::value_ptr(transform));
			
			dde.drawCircle({ 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 0.0f }, 0.1f);
			dde.popTransform();
			});
		dde.end();

		// Advance to next frame. Rendering thread will be kicked to
		// process submitted rendering primitives.
		bgfx::frame();
	}

}
