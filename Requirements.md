* I want an application that display a pretty picture with explosions and stuff. Making movies here.
* Display or output?
  * Which format?
    * Industry standard.
      * h264 video
* Do we want editing or not? Interactive?
  * Yes all of that
* Define "pretty"
  * I want it to look like this (example from movie)
    * -> if it looks right, it is right
    * Given a scene, simulate light transport (realistically) to an error of < 1%
* What's in the scene?
  * Everything
    * Import
* Animation

====
* MVP for hackathon
* Given a scene, and a camera, simulate light transport (realistically) to an error of < 1%
* How do we get the scene?
  * Hardcode
* What is in the scene?
  * Sphere at first
  * Support for others - plane, cube, cylinder
* Materials?
  * Light emitter
  * Lambertian diffuse
  * Support for - mirror, refractive
  * Support for textures
* What kind of camera?
  * Pinhole
* Where and how to output?
  * Screen
  * RGB -> sRGB
  * Resolution
    * max 1920x1080
* GUI
  * SPP
  * Time per frame
  * Exposure control
  * Enable-disable features
* Interactive?
  * No - for MVP
  * Support for future
  * Camera and object properties
    * Live movement through GUI
  * No rigid-body physics

# Non-functional
* Performance
  * Let's use less than 1 GB of RAM and VRAM
  * CPU usage < 100%
  * Possibility use accelerators
  * Don't kill GUI thread
* Timing
  * We want to see something in at least 1 second
  * Final result (< 1% error) in less than 5 minutes
* Environment
  * Linux, portable
  * My laptop
* Testability
  * Debug display modes
* Scalability
  * Distributed?
    * No, not planned
  * Not scalable to new features

# System and component requirements
* Features
  * Pure monte-carlo path tracing
    * Support for next event estimation
  * Multi-threading
  * GUI
  * Scene manager
  * Camera controller
  * Material library
  * Modular pipeline
* Data models
  * Polymorphic Object
  * Linear algebra data/objects
  * Access patterns
    * Access one pixel depth first
    * Access the whole scene for each pixel for each interaction
    * Calculation in material
  * Camera
  * Global configuration block
  * Psedorandom generator
    * Thread local
* Technologies/frameworks
  * C++
  * CUDA
  * SFML
  * ImGUI
  * Eigen