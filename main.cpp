#include <owl/owl.h>
#include <iostream>

// This array is generated automatically by embed_ptx in CMake
extern "C" char device_ptx[]; 

int main() {
    std::cout << "Initializing OWL Context..." << std::endl;
    
    // Create context with 1 GPU
    OWLContext context = owlContextCreate(nullptr, 1);
    
    // Load the PTX compiled from device.cu
    OWLModule module = owlModuleCreate(context, device_ptx);
    
    std::cout << "SUCCESS! OWL Context and Module created." << std::endl;
    
    owlContextDestroy(context);
    return 0;
}
