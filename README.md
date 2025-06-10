
# Small engine with RHI (Render Hardware Interface) abstraction
Currently working on RHI and Dx11 specification of the interface.

## TODO:

### Done recently

- [X] \- Make RHI a singleton and remove interface from window.h
- [X] \- Create basic object classes -> Mesh
- [X] \- Upload buffer from RHI in Mesh
- [X] \- Finish RHIShader for Vertex and Pixel shader + renderer implementation (default tasks)
- [X] \- Make RHI buffer upload logic
- [X] \- Create scene with renderable objects
- [X] \- Separate Device and Swapchain creation
	- [X] \- Create DXGI Factory to create Swapchain
- [~] \- RHI types and interface types
	- [X] \- Create a RHIViewport class holding width-height
- [X] \- Update MVP in CBO using Camera position
- [X] \- Handle input for movement
- [X] \- Add lookAt in camera class
- [X] \- Handle input for rotation
- [X] \- Create events before window creation
- [X] \- Create events after window creation
- [X] \- Fixed crash because of late window events registration
- [X] \- Track when mouse leaves the window
- [X] \- Fix error when minimizing the window
- [X] \- Output texture in render target instead of backbuffer

### WIP

- [~] \- Add simple pbr light pass

### Upcoming (ranked by priority)

- [ ] \- Add delta time
- [~] \- RHI types and interface types
	- [ ] \- Create an InputLayout class to handle `stride` and `offset`
	- [ ] \- Add CommandList system
	- [ ] \- Use PSOs and create a wrapper for pipeline state
- [ ] \- Handle materials
- [ ] \- Make Buffer upload logic generic
- [ ] \- Create Transform structure
  - [ ] \- Add relative rotation
- [ ] \- Add RenderState enum to Mesh to track when to upload buffer/render/...
- [ ] \- Add a submesh abstraction
- [ ] \- Rework inputs system to use dinput

### Secondary tasks

- [ ] \- Update window center (in screen)
- [ ] \- Update camera position while button pressed and use delta time
- [ ] \- Distinguish Window & Frame
- [ ] \- Handle multiple viewport (1 scene -> 1 viewport)
- [ ] \- Create forward .h for common STL includes (vector, string, ...)
- [ ] \- Isolate CPU and GPU tasks in their own thread

### Not a feature

- [ ] \- Make indentation homogenous (only tabs)
- [ ] \- Move HWND to RHIViewport
- [ ] \- Add constness to methods when possible
- [ ] \- Get rid of `friend class` with API Interface in RHI classes
- [ ] \- Get rid of virtual keyword when override
- [ ] \- Make code cppcheck compliant
- [ ] \- Core should be D3D11Core
- [ ] \- Rename D3D11 classes from engine to differentiate from windows api classes
- [ ] \- Add Debug logs to keep track of what's happening, when, and how many times
- [ ] \- Create a xgo namespace
- [ ] \- Clean includes
- [ ] \- Create a Math fwd.h with DirectXMath include and typedefs
- [ ] \- Create a Math fwd.h with DirectXMath include and typedefs