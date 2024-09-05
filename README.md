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

## Transformation Matrices

### Rotation

#### Applying a rotation to a 2D vector using trigonometry functions

Goal: Rotate a 2D vector $(x, y)$ around the origin by angle $\beta$ to get
$(x', y')$.

Let $\alpha$ represent the angle from the origin to $(x, y)$.

Let $r$ represent the hypotenuse of the triangle, or radius of the rotation.

![Illustration of the triangle formed between the origin and a given 2D vector](/images/2d-vector-angle.png)

Since $\cos(\alpha) = \frac{x}{r}$, $x = r \times \cos(\alpha)$

Since $\sin(\alpha) = \frac{y}{r}$, $y = r \times \sin(\alpha)$

After applying the angle $\beta$, the new coordinates can be calculated:

![Illustration of the rotated vector's new triangle](/images/2d-vector-rotated.png)

$\cos(\alpha + \beta) = \frac{x'}{r}$, so $x' = r \times \cos(\alpha + \beta)$

$\sin(\alpha + \beta) = \frac{y'}{r}$, so $y' = r \times \sin(\alpha + \beta)$

Trig functions that add two values can be expanded using the
_angle addition formula_:

$$
x' = r\cos(\alpha + \beta) \\
x' = r(\cos{\alpha} \cos{\beta} - \sin{\alpha} \sin{\beta}) \\
x' = r \cos{\alpha} \cos{\beta} - r \sin{\alpha} \sin{\beta}
$$

You can substitute $r \cos{\alpha}$ with $x$, and $r \sin{\alpha}$ with $y$:

$$
x' = x \cos{\beta} - y \sin{\beta}
$$

Similarly,

$$
y' = r \sin(\alpha + \beta) \\
y' = r(\sin{\alpha} \cos{\beta} + \cos{\alpha} \sin{\beta}) \\
y' = r \sin{\alpha} \cos{\beta} + r \cos{\alpha} \sin{\beta} \\
y' = y \cos{\beta} + x \sin{\beta}
$$

These are the formulas that are used by a rotation transformation matrix.

The same principle applies to 3 dimensions, but with one dimension at a time:

```c
vec3_t Vec3RotateX(vec3_t v, float angle)
{
    vec3_t rotated_vector = {
        .x = v.x,
        .y = v.y * cosf(angle) - v.z * sinf(angle),
        .z = v.y * sinf(angle) + v.z * cosf(angle),
    };
    return rotated_vector;
}

vec3_t Vec3RotateY(vec3_t v, float angle)
{
    vec3_t rotated_vector = {
        .x = v.x * cosf(angle) - v.z * sinf(angle),
        .y = v.y,
        .z = v.x * sinf(angle) + v.z * cosf(angle),
    };
    return rotated_vector;
}

vec3_t Vec3RotateZ(vec3_t v, float angle)
{
    vec3_t rotated_vector = {
        .x = v.x * cosf(angle) - v.y * sinf(angle),
        .y = v.x * sinf(angle) + v.y * cosf(angle),
        .z = v.z,
    };
    return rotated_vector;
}
```

# Progress

Here's where I take videos of significant milestones to look back on the
progress I've made.

## 01. Simple Perspective Cube Points

A cube made up of a cloud of points with simple perspective projection applied.

https://github.com/user-attachments/assets/7b7784bc-4cf1-41ff-a7f4-16496ee17e95

_[01.Simple-Perspective-Cube-Points.mp4](/videos/01.Simple-Perspective-Cube-Points.mp4)_

## 02. Simple Rotation Transformation

Rotating the cube with simple rotation transformations.

https://github.com/user-attachments/assets/d292fcb0-5ef9-456a-81c1-ded31994804e

_[02.Simple-Vector-Rotation-Transformation.mp4](/videos/02.Simple-Vector-Rotation-Transformation.mp4)_

## 03. Wireframe Cube

The cube is now expressed as a collection of triangle faces and is rendered as
a wireframe using a simple line rasterization algorithm.

https://github.com/user-attachments/assets/2110c5b9-4b13-4d52-b9e0-2d0182134f01

_[03.Wireframe-Cube.mp4](/videos/03.Wireframe-Cube.mp4)_