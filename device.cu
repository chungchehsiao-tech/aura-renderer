#include <owl/owl.h>

using namespace owl;

// CPU/GPU shared struct
struct LaunchParams {
    uint32_t* colorBuffer;
    vec2i fbSize;
    OptixTraversableHandle world;
};

extern "C" __constant__ LaunchParams optixLaunchParams;

// ray HITS Program (triangle)
OPTIX_CLOSEST_HIT_PROGRAM(TriangleCH)() {
    // Return Black as silhouette now
    optixSetPayload_0(0x00000000);
}

// ray MISS Program
OPTIX_MISS_PROGRAM(Miss)() {
    // Return Dark Gray
    optixSetPayload_0(0x00777777);
}

// Generates rays for every pixel on the screen
OPTIX_RAYGEN_PROGRAM(aura_renderer_raygen)()
{
    vec2i pixel = getLaunchIndex();
    vec2i size = optixLaunchParams.fbSize;

    // Simple orthographic camera pointing down the Z-axis for testing
    vec2f uv = (vec2f(pixel) + 0.5f) / vec2f(size);
    vec3f rayDir(0.f, 0.f, 1.f);
    vec3f rayOrg(uv.x, uv.y, -1.f); 

    uint32_t output_color = 0;
    optixTrace(optixLaunchParams.world, rayOrg, rayDir, 0.f, 1e5f, 0.0f,
        255, OPTIX_RAY_FLAG_DISABLE_ANYHIT, 0, 1, 0, output_color);

    optixLaunchParams.colorBuffer[pixel.x + pixel.y * size.x] = output_color;
}
