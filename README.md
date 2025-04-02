# DX11 Sample project

Creating a small renderer with DX11.

## TODO:

### Done recently

- [X] - Make RHI a singleton and remove interface from window.h
- [X] - Create basic object classes -> Mesh
- [X] - Upload buffer from RHI in Mesh

### WIP

- [~] - Finish RHIShader + renderer implementation (default tasks)
- [~] - RHI types and interface types
- [~] - Make RHI buffer upload logic
- [~] - Create scene with renderable objects

### Upcoming (ranked by priority)

- [ ] - Create an InputLayout class to handle `stride` and `offset`
- [ ] - Add CommandList system
- [ ] - Use PSOs and create a wrapper for pipeline state
- [ ] - Create Transform structure
- [ ] - Add RenderState enum to Mesh to track when to upload buffer/render/...
- [ ] - Add a submesh abstraction

### Secondary tasks

- [ ] - Distinguish Window & Frame
- [ ] - Handle multiple viewport (1 scene -> 1 viewport)
- [ ] - Create forward .h for common STL includes (vector, string, ...)

### Not a feature

- [ ] - Core should be D3D11Core
- [ ] - Rename D3D11 classes from engine to differentiate from windows api classes
- [ ] - Add Debug logs to keep track of what's happening, when, and how many times
- [ ] - Create a xgo namespace