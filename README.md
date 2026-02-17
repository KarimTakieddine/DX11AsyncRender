# DX11AsyncRender - Windows

A multithreaded, Windows console application demonstrating asynchronous pipelining of updates and GPU command submission by the CPU, coupled with waiting on the main (CPU) thread for an arbitrary number of queued frames to be swapped by the renderer.

This is in an effort to maximise performance of both updates to the game / physics state and frame presentation, while smoothing out visual glitches, tearing and/or jitter resulting from inconsistencies in naive frame time sampling.

### **_Note_**

Evidently, this is a very basic code sample and for obvious reasons is probably a far cry from what Unity Technologies actually employ as an algorithm. That being said, the general problem of "judder" when making games and graphics simulations has been living rent-free in my head for almost the past decade, whether when using commercial engines like Unity, Unreal or while developing my own custom engine(s). The article referenced below provided a great source of inspiration and desire to make more performant and polished products as an independent developer.

At the time of writing, Unity likely is leveraging the latest features of DirectX 12 to employ a more sophisticated and hardware-scalable deferred GPU command buffer submission architecture, offering opportunities for greater pipelining by splitting rendering logic into further stages.

## Technical Overview

### CPU throttling controlled by queued frame buffer depth

At the start of each loop iteration, the main UI / Windowing thread (after draining the Windows message queue and events within) waits for frame n - maxQueuedFrames (left as a locally-scoped constant for simplicity) to be swapped by the asynchronous rendering logic. This allows pacing of updates such that they do not run too far ahead of the rendering thread and risk of overruning the shared frame buffer's data while it is still being rendered, as well as ensure consistent updates and prevent accumulation of updates prior to rendering a given frame, leading to "jerky" linear-time transformations.

### Wait-free ring buffer of shared frame data

In this SPSC (single-producer-single-consumer) scenario, predictable time access into a shared frame buffer allows for maximum CPU throughput (in addition to being separated from asynchronous rendering logic) and and avoiding locking overhead / resource contention around access to a given frame's data. This also allows signalling to occur across thread boundaries without significantly slowing down the renderer.

### Frame time smoothing

Frame delta time is set atomically prior to each signal from the rendering thread that "work is done". Coupled with precise frame swap timestamps obtained by API-specific call (the equivalent in OpenGL would be using something like a `GLQuery` timer query or a `GLFence` object to obtain the specific swap time), this allows micro variations in frame-to-frame delta time to be absorbed, even in cases of a spike in execution time of CPU-side logic (provided they don't become the norm).

### **_Note_**

Oh, and while we have experienced great success along the way, at the moment it's only rendering a single triangle and applying some very basic transformations every frame, so you'll have to use your imagination to think what game to build on top of this!

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
