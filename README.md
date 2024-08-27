# 3D Rendering in Software

This is my work to build a software 3D renderer as guided by the
["Learn Computer Graphics Programming" course by Gustavo Pezzi](https://pikuma.com/courses/learn-3d-computer-graphics-programming).

# Math Notes

## Perspective Projection

The two triangles that make up the viewer's angle to the screen-space projected
point and the 3D point are similar triangles, which share a constant ratio
$\frac{BC}{DE} = \frac{AB}{AD}$ as illustrated in the top-down view of the point's X axis
below.

![Illustration of perspective projection similar triangles](/images/perspective-projection-triangle.png)

This can be simplified:

$$\frac{P'_x}{P_x}=\frac{1}{P_z}$$

$$P'_x=\frac{P_x}{P_z}$$

Note $P_z$ in the denominator, indicating that the projected X is scaled by the
Z distance to the point. This is called the **perspective divide**.

Similarly, the Y perspective projection:

$$P'_y=\frac{P_y}{P_z}$$

## Left vs Right-handed Coordinate Systems

Mainly determines whether Z values grow "into" the screen away from the viewer,
or "out" of the display toward the viewer.

Left-handed coordinate systems refer to Z values that grow into the screen, as
used by DirectX.

Right-handed coordinate systems refer to Z values that grow out of the screen,
as used by OpenGL.

![Illustration of human hands mnemonic](/images/coordinate-system-hands.png)

_Illustration from https://www.oreilly.com/library/view/learn-arcore/9781788830409_

# Progress

Here's where I take videos of significant milestones to look back on the
progress I've made.

## 01. Simple Perspective Cube Points

A cube made up of a cloud of points with simple perspective projection applied.

https://github.com/user-attachments/assets/7b7784bc-4cf1-41ff-a7f4-16496ee17e95

_[01.Simple-Perspective-Cube-Points.mp4](/videos/01.Simple-Perspective-Cube-Points.mp4)_
