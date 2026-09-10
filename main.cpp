#include <owl/owl.h>
#include <iostream>
#include <fstream>

struct vec2i { int x, y; };
struct vec3f { float x, y, z; };
struct vec3i { int x, y, z; };

// This array is generated automatically by embed_ptx in CMake
extern "C" char device_ptx[]; 

// CPU/GPU shared struct
struct LaunchParams {
    uint32_t* colorBuffer;
    vec2i fbSize;
    OptixTraversableHandle world;
};

int main() {

    std::cout << "Initializing OWL Context..." << std::endl;
    
    // Create context with 1 GPU
    OWLContext context = owlContextCreate(nullptr, 1);
    
    // Load the PTX compiled from device.cu
    OWLModule module = owlModuleCreate(context, device_ptx);


    // Create the  1 Triangle for testing
    vec3f vertices[3] = {{0.5f, 0.8f, 0.f}, {0.2f, 0.2f, 0.f}, {0.8f, 0.2f, 0.f}};
    int3 indices[1]   = {{0, 1, 2}};

    OWLBuffer vertexBuffer = owlDeviceBufferCreate(context, OWL_FLOAT3, 3, vertices);
    OWLBuffer indexBuffer = owlDeviceBufferCreate(context, OWL_INT3, 1, indices);

    OWLGeomType type = owlGeomTypeCreate(context, OWL_GEOMETRY_TRIANGLES, 0, nullptr, -1);
    owlGeomTypeSetClosestHit(type, 0, module, "TriangleCH");

    OWLGeom geom = owlGeomCreate(context, type);
    owlTrianglesSetVertices(geom, vertexBuffer, 3, sizeof(vec3f), 0);
    owlTrianglesSetIndices(geom, indexBuffer, 1, sizeof(int3), 0);

    // Build the BVH
    OWLGroup triGroup = owlTrianglesGeomGroupCreate(context, 1, &geom);
    owlGroupBuildAccel(triGroup);
    OWLGroup world = owlInstanceGroupCreate(context, 1, &triGroup);
    owlGroupBuildAccel(world);
    
    // Create the Programs
    OWLRayGen rayGen = owlRayGenCreate(context, module, "aura_renderer_raygen", 0, nullptr, -1);
    owlMissProgCreate(context, module, "Miss", 0, nullptr, -1);


    // Configure Launch Parameters
        OWLVarDecl lpVars[] = {
            {"colorBuffer", OWL_BUFPTR, offsetof(LaunchParams, colorBuffer)},
            {"fbSize", OWL_INT2, offsetof(LaunchParams, fbSize)},
            {"world", OWL_GROUP, offsetof(LaunchParams, world)},
            {nullptr}
    };
    OWLLaunchParams lp = owlParamsCreate(context, sizeof(LaunchParams), lpVars, -1);

    vec2i fbSize = { 800, 600 }; 
    OWLBuffer fb = owlHostPinnedBufferCreate(context, OWL_INT, fbSize.x * fbSize.y);

    owlParamsSetBuffer(lp, "colorBuffer", fb);
    owlParamsSet2i(lp, "fbSize", fbSize.x, fbSize.y);
    owlParamsSetGroup(lp, "world", world);

    // Build pipeline and run
    owlBuildPrograms(context);
    owlBuildPipeline(context);
    owlBuildSBT(context);
    owlLaunch2D(rayGen, fbSize.x, fbSize.y, lp);

    // 6. Save the output to a PPM image
    std::ofstream out("render.ppm");
    out << "P3\n" << fbSize.x << " " << fbSize.y << "\n255\n";

    const uint32_t* pixels = (const uint32_t*)owlBufferGetPointer(fb, 0);
    for (int y = fbSize.y - 1; y >= 0; --y) {
        for (int x = 0; x < fbSize.x; ++x) {
            uint32_t c = pixels[x + y * fbSize.x];
            out << (c & 255) << " " << ((c >> 8) & 255) << " " << ((c >> 16) & 255) << "\n";
        }
    }

    std::cout << "Success! Saved render.ppm to the build folder." << std::endl;
    
    owlContextDestroy(context);
    return 0;
}
