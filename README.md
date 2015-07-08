This project is a clone of QGears tailored towards getting FF7 100% working.

Current idea/plan is as follows:

1. Get all builds and packaging working on OSX, Linux and Windows.
2. Still using C++ to share QGears code
3. Implement in terms of Kernel, Menu, Field, Battle and World map modules.
4. Using SDL2 for Window/Input management, bgfx for rendering and NanoVG for the Menu module. ImGui for debug UI.
5. Use native formats with a thin abstraction layer.
6. Build in mod support/file swapping at the file system abstraction layer
