# GAT350

<img width="416" alt="SSPhong" src="https://github.com/user-attachments/assets/f2a9d718-29aa-4439-a7ab-da23292892f1">

<img width="663" alt="SSRayTracerFinalScene" src="https://github.com/user-attachments/assets/3637d020-b818-4089-870f-6cec6d03c9e9">


Welcome to a study in C++ Computer Graphics. This repository contains two key projects that showcase advanced rendering techniques:

1. **Software Renderer** - A CPU-based rendering engine.
2. **Ray Tracing Engine** - A high-quality ray tracer.

---

## Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Getting Started](#getting-started)
  - [Prerequisites](#prerequisites)
  - [Installation](#installation)
- [Projects](#projects)
  - [Software Renderer](#software-renderer)
  - [Ray Tracing Engine](#ray-tracing-engine)
- [Technical Details](#technical-details)
  - [Software Renderer](#software-renderer-technical-details)
  - [Ray Tracing Engine](#ray-tracing-engine-technical-details)
- [Third-Party Assets](#third-party-assets)
- [Usage](#usage)
- [Roadmap](#roadmap)
- [Contributing](#contributing)
- [License](#license)
- [Contact](#contact)

---

## Overview

This repository demonstrates two complementary rendering techniques:

- The **Software Renderer** focuses on real-time, rasterization-based rendering on the CPU.
- The **Ray Tracing Engine** emphasizes photorealism, simulating light and shadow behavior with high fidelity.

Both projects are designed to provide educational value and serve as a foundation for building more complex rendering systems.

---

## Features

### Software Renderer
- Real-time rendering on CPU
- Support for basic shapes and models
- Perspective projection and transformations
- **Shading Models:**
  - Flat Shading
  - Gouraud Shading
  - Phong Shading
- **Shader Support:**
  - Vertex shader emulation
  - Fragment shader emulation
- **Material Handling:**
  - Diffuse and specular reflection
  - Adjustable material properties (e.g., shininess, color)
- **Post-Processing:**
  - Simple bloom effect
  - Grayscale filter
  - Etc.

### Ray Tracing Engine
- Accurate simulation of light physics
- Support for reflection, refraction, and shadows
- Global illumination via path tracing
- **Material Handling:**
  - Dielectrics (e.g., glass, water)
  - Metals (e.g., gold, aluminum)
  - Diffuse materials (e.g., matte surfaces)
  - Procedural textures
- **Anti-Aliasing:**
  - Supersampling for smoother edges
  - Configurable sampling rates
- **Post-Processing:**
  - Tone mapping
  - Depth-of-field effects
- **Shading Models:**
  - Blinn-Phong
  - Fresnel effects for realistic light interaction

---

## Technical Details

### Software Renderer Technical Details

#### Gouraud Shading
Gouraud shading interpolates vertex colors across a polygon’s surface:
1. Vertex normals are calculated or pre-defined.
2. Lighting calculations are applied per vertex using the **Phong Reflection Model**.
3. Color values are linearly interpolated across the polygon.

#### Phong Shading
Phong shading performs per-pixel lighting for more accurate results:
1. Vertex normals are interpolated across the polygon.
2. At each pixel, the interpolated normal is used in the **Phong Reflection Model** for lighting.
3. Results in smoother highlights and better lighting transitions.

#### Shader Emulation
- **Vertex Shader:** Simulates transformations and lighting calculations for vertices.
- **Fragment Shader:** Applies per-pixel operations like interpolation and lighting.

---

### Ray Tracing Engine Technical Details

#### Material Types
- **Diffuse:** Lambertian reflection for matte surfaces. 
  - Light scatters uniformly in all directions.
- **Metals:** Reflective surfaces modeled using Fresnel equations. 
  - Includes adjustable roughness for blurred reflections.
- **Dielectrics:** Transparent materials with refraction (e.g., glass, water).
  - Supports total internal reflection and customizable indices of refraction.
- **Procedural Materials:** Uses mathematical functions to generate textures like stripes, noise, or gradients.

#### Anti-Aliasing
To reduce jagged edges, the ray tracer uses **Supersampling**:
1. Multiple rays are cast per pixel with jittered offsets.
2. Color values are averaged to produce a smoother result.
3. Configurable sampling rate (e.g., 4x, 16x) for performance trade-offs.

---

## Third-Party Assets

This project uses the following third-party library:

- **GLM (OpenGL Mathematics):**
  - A C++ mathematics library for 3D graphics.
  - Used for vector, matrix, and transformation operations.
  - Licensed under the MIT License.
  - Repository: [https://github.com/g-truc/glm](https://github.com/g-truc/glm)

Ensure that you adhere to the terms of the GLM license if you modify or redistribute this project.

---

## Usage

To run the projects:
1. Navigate to the respective project's build directory.
2. Execute the application:
   ```bash
   ./2D   # For the Software Renderer
   ./RayTracer  # For the Ray Tracing Engine
