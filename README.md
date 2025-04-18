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
	- [X] - Create DXGI Factory to create Swapchain

### WIP

- [~] \- RHI types and interface types
	- [~] \- Create a RHIViewport class holding width-height
- [~] \- Update MVP in CBO using Camera position

### Upcoming (ranked by priority)

- [ ] \- Create an InputLayout class to handle `stride` and `offset`
- [ ] \- Add CommandList system
- [ ] \- Make Buffer upload logic generic
- [ ] \- Use PSOs and create a wrapper for pipeline state
- [ ] \- Create Transform structure
- [ ] \- Add RenderState enum to Mesh to track when to upload buffer/render/...
- [ ] \- Add a submesh abstraction

### Secondary tasks

- [ ] \- Distinguish Window & Frame
- [ ] \- Handle multiple viewport (1 scene -> 1 viewport)
- [ ] \- Create forward .h for common STL includes (vector, string, ...)
- [ ] \- Isolate CPU and GPU tasks in their own thread

### Not a feature

- [ ] \- Make code cppcheck compliant
- [ ] \- Core should be D3D11Core
- [ ] \- Rename D3D11 classes from engine to differentiate from windows api classes
- [ ] \- Add Debug logs to keep track of what's happening, when, and how many times
- [ ] \- Create a xgo namespace
- [ ] \- Clean includes
- [ ] \- Create a Math fwd.h with DirectXMath include and typedefs