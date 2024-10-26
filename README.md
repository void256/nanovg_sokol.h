# nanovg_sokol.h
nanovg sokol renderer and example

This repository contains [nanovg_sokol.h](src/nanovg_sokol/nanovg_sokol.h), a renderer implementation for [memononen/nanovg](https://github.com/memononen/nanovg) that uses [floooh/sokol](https://github.com/floooh/sokol).

Additionally, the original nanovg demo is included as example code.

[nanovg_sokol.h](src/nanovg_sokol/nanovg_sokol.h) was created by [@darkuranium](https://github.com/darkuranium) and [@zeromake](https://github.com/zeromake), see https://github.com/floooh/sokol/issues/633 for it's history. I've only contributed a tiny fix to make font rendering work.

## Files included

The only file you actually need:
- [src/nanovg_sokol.h](src/nanovg_sokol/nanovg_sokol.h) 😎

It embedds all the required shader code and supports the following shader languages: `glsl410:glsl300es:hlsl4:metal_macos:metal_ios:metal_sim:wgsl`.
  
Everything else is just required for the demo and the example setup:

- [example](example) contains the original image resources from [memononen/nanovg](https://github.com/memononen/nanovg).
- [src/nanovg](src/nanovg) contains the original, unmodified nanovg source from [memononen/nanovg](https://github.com/memononen/nanovg).
- [src/nanovg_demo](src/nanovg_demo) contains the original, unmodified nanovg demo source from [memononen/nanovg](https://github.com/memononen/nanovg).
- [src/sokol](src/sokol) contains the sokol headers.

- [src/main.c](src/main.c) shows how to setup everything and then loads and renders the demo from [src/nanovg_demo](src/nanovg_demo).

## Usage
The only file you actually need is [nanovg_sokol.h](src/nanovg_sokol/nanovg_sokol.h) and you'll use `nvgCreateSokol` to create the NVGcontext.

```c
#include "nanovg.h"

#define NANOVG_SOKOL_IMPLEMENTATION 1
#include "nanovg_sokol.h"

// in your init_cb call somewhere
NVGcontext* vg = nvgCreateSokol(NVG_ANTIALIAS | NVG_STENCIL_STROKES);
if (vg == NULL) {
    printf("Could not init NanoVG.\n");
    return;
}

// in your sokol frame_cb call:
sg_begin_pass(&(sg_pass){ .action = pass_action, .swapchain = sglue_swapchain() });

nvgBeginFrame(vg, winWidth, winHeight, pxRatio);
// use vg from now on to call nanovg functions ...
nvgEndFrame(vg);

sg_end_pass();
sg_commit();
```

## Build the demo
You'll need [ninja-build](https://ninja-build.org/) and then simply call:

```bash
ninja build/main
cd build
./main
```

The build.ninja file currently only supports macOS and uses `SOKOL_METAL` but can be easily adjusted for other operating systems.

If everything worked it should look like this:

https://github.com/user-attachments/assets/22e6e78d-337d-44e4-b4ea-701d7d174bcf

😎
