# Aura Renderer 🚧 (Work in Progress)

> **Note:** This project is currently in active development. Features and documentation are being updated regularly.

## 📖 Overview
**Aura Renderer** is a volumetric renderer built to use hardware-accelerated ray tracing. The primary goal of this project is to explore and implement raymarching using the Nvidia ecosystem.

## 🛠️ Tech Stack
* **Core Ray Tracing API:** [Nvidia OptiX](https://developer.nvidia.com/rtx/ray-tracing/optix)
* **High-Level Framework:** [Nvidia OWL (OptiX Wrapper Library)](https://github.com/owl-project/owl)
* **Languages:** C++ / CUDA
* **Build System:** CMake

## 🚀 Current Status & Roadmap
I am currently actively developing the core architecture and focusing on setting the Beer's Law in the raymarching.

**To-Do List:**
- [x] Initial project setup and environment configuration
- [x] Basic OWL context and module initialization
- [x] Implement basic geometry intersection / model silhouette
- [ ] Add raymarching fog simulation
- [ ] Finalize build instructions for Windows/Linux
- [ ] Add visual showcases/screenshots of the renders

## ⚙️ Build Instructions (Coming Soon)
Detailed instructions on how to clone, build (via CMake), and run the project will be added once the core foundation is stabilized. 

*Note: Requires an Nvidia RTX-compatible GPU and the latest CUDA Toolkit.*


