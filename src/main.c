#include <stdio.h>

#include "sokol/sokol_gfx.h"
#include "sokol/sokol_app.h"
#include "sokol/sokol_glue.h"
#include "sokol/sokol_time.h"
#include "sokol/sokol_log.h"

#include "nanovg/nanovg.h"

#define NANOVG_SOKOL_IMPLEMENTATION 1
#include "nanovg_sokol/nanovg_sokol.h"

#include "nanovg_demo/demo.h"

static int blowup = 0;
static int screenshot = 0;
static int premult = 0;
static float mouseX;
static float mouseY;
static sg_pass_action pass_action;
static DemoData data;
static NVGcontext* vg = NULL;

void sokol_init(void) {
    stm_setup();
    sg_desc ctx = (sg_desc) {
        .logger.func = slog_func,
        .environment = sglue_environment(),
        .pipeline_pool_size = 512,
    };
    sg_setup(&ctx);

    // clear action
    pass_action = (sg_pass_action) {
        .colors[0] = {
            .load_action = SG_LOADACTION_CLEAR,
            .clear_value =  { 1.f, 1.0f, 1.0f, 1.0f }
        }
    };

    vg = nvgCreateSokol(NVG_ANTIALIAS | NVG_STENCIL_STROKES);
    if (vg == NULL) {
        printf("Could not init NanoVG.\n");
        return;
    }

    if (loadDemoData(vg, &data) == -1) {
        printf("Could not loadDemoData.\n");
        return;
    }
}

void sokol_frame(void) {
    int winWidth = sapp_width();
    int winHeight = sapp_height();
    int fbWidth = sapp_width();
    int fbHeight = sapp_height();
    float pxRatio = (float)fbWidth / (float)winWidth;
    float t = stm_sec(stm_now());

    // Calculate pixel ration for hi-dpi devices.

    if (premult) {
        pass_action.colors[0].clear_value = (sg_color) { 0.0f, 0.0f, 0.0f, 0.0f };
    } else {
        pass_action.colors[0].clear_value = (sg_color) { 0.3f, 0.3f, 0.32f, 1.0f };
    }
    sg_begin_pass(&(sg_pass){ .action = pass_action, .swapchain = sglue_swapchain() });

    nvgBeginFrame(vg, winWidth, winHeight, pxRatio);
    renderDemo(vg, mouseX, mouseY, winWidth, winHeight, t, blowup, &data);
    nvgEndFrame(vg);

    if (screenshot) {
        screenshot = 0;
        saveScreenShot(fbWidth, fbHeight, premult, "dump.png");
    }

    sg_end_pass();
    sg_commit();
}

void sokol_cleanup(void) {
    freeDemoData(vg, &data);
    nvgDeleteSokol(vg);
    sg_shutdown();
}

void sokol_event(const sapp_event* event) {
    if (event->type == SAPP_EVENTTYPE_MOUSE_MOVE) {
        mouseX = event->mouse_x;
        mouseY = event->mouse_y;
    }
    if (event->type != SAPP_EVENTTYPE_KEY_DOWN) {
        return;
    }
    if (event->key_code == SAPP_KEYCODE_ESCAPE) {
        // TODO
    }
    if (event->key_code == SAPP_KEYCODE_SPACE) {
        blowup = !blowup;
    }
    if (event->key_code == SAPP_KEYCODE_S) {
        screenshot = 1;
    }
    if (event->key_code == SAPP_KEYCODE_P) {
        premult = !premult;
    }
}

sapp_desc sokol_main(int argc, char* argv[]) {
    return (sapp_desc) {
        .init_cb = sokol_init,
        .frame_cb = sokol_frame,
        .cleanup_cb = sokol_cleanup,
        .event_cb = sokol_event,
        .width = 1024,
        .height = 768,
        .window_title = "nanovg_sokol demo"
    };
}