# DX11 Sample project

Creating a small renderer with DX11.

## TODO:

[ ] - Create Transform structure
[ ] - Create basic object classes
[ ] - Handle objects rotation
[ ] - Distinguish Window & Frame
[X] - Make RHI a singleton and remove interface from window.h
[ ] - Add RenderState enum to Mesh to track when to upload buffer/render/...
[ ] - Create sscene with number tris to draw
[ ] - Make RHI buffer upload logic
[ ] - Upload buffer from RHI in Mesh
[ ] - Rename D3D11 classes from engine to differentiate from windows api classes
[ ] - Handle multiple viewport (1 scene -> 1 viewport)
[ ] - RHI types and interface types
[ ] - Core should be D3D11Core
[ ] - Create forward .h for common STL includes (vector, string, ...)
[ ] - Finish RHIShader + renderer implementation
[ ] - Add CommandList system