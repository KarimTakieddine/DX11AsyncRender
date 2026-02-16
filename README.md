# DX11AsyncRender - Windows

A multithreaded, Windows console application demonstrating asynchronous pipelining of updates and GPU command submission by the CPU, coupled with waiting on the main (CPU) thread for an arbitrary number of queued frames to be swapped by the renderer.

This is in an effort to maximise performance of both updates to the game / physics state and frame presentation, while smoothing out visual glitches, tearing and/or jitter resulting from inconsistencies in naive frame time sampling.

## **_Note_**

Oh, and while we have experienced great success along the way, at the moment it's only rendering a single triangle and applying some very basic transformations every frame, so you'll have to use your imagination to think what game to build on top of this!

### But look how smooth! (minus some ScreenToGif recording lag :D)

https://github.com/user-attachments/assets/a37dbbd3-0604-495c-bf08-07ff31f26a2a

# Building

- Clone the repository
- Build using CMake on Windows

## Example using Microsoft Visual Studio Community 2026 (18.3.0):

- File -> Open -> CMake Project...
- Open this repository's CMakeLists.txt file
- Edit local / shared CMakePresets using the build target menu -> "Manage configurations"
- Wait for configuration to complete (**note this is a Windows-only program**)
- Build away! 

# Inspirations

[Unity Article by TAUTVYDAS ŽILYS / UNITY TECHNOLOGIES (2026)](https://unity.com/blog/engine-platform/fixing-time-deltatime-in-unity-2020-2-for-smoother-gameplay)

# Libraries / Tools Used

- STL
- D3D11
- CMake
- My own (e.g. allocators, platform layer...)

# LICENSE

MIT (see LICENSE.txt)
