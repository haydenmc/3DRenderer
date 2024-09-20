# 3D Rendering in Software

This is my work to build a software 3D renderer as guided by the
["Learn Computer Graphics Programming" course by Gustavo Pezzi](https://pikuma.com/courses/learn-3d-computer-graphics-programming).

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

## 04. Rendering OBJ Files

Instead of a static cube, the renderer can now read in arbitrary OBJ files to
render.

https://github.com/user-attachments/assets/89b2c095-3da6-4f46-abf7-81b7079f193d

_[04.Render-OBJ-File.mp4](/videos/04.Render-OBJ-File.mp4)_

## 05. Back Face Culling

Implemented a bunch of vector functions and used them to implement back-face
culling; mesh faces that aren't visible by the camera are no longer rendered.

https://github.com/user-attachments/assets/6244fd86-ecb3-4da7-bc90-2546d74a739c

_[05.Back-Face-Culling.mp4](/videos/05.Back-Face-Culling.mp4)_

## 06. Triangle Rasterization

Triangles are now filled in with a set of static colors. Render modes can be
changed at runtime between wireframe and rasterized, plus an option for
showing dots on vertices and enabling/disabling back-face culling.

https://github.com/user-attachments/assets/c22b3a23-cfb9-4d59-ba3d-ba4bcd3f1792

_[06.Triangle-Rasterization.mp4](/videos/06.Triangle-Rasterization.mp4)_

## 07. Face Depth Sorting

With a naive sorting algorithm, faces are rasterized in the correct depth order,
preventing back faces from "bleeding through."

https://github.com/user-attachments/assets/aabe4a79-ba2c-4486-93bc-711db1823edf

_[07.Face-Depth-Sorting.mp4](/videos/07.Face-Depth-Sorting.mp4)_

## 08. Transformation Matrices

Use rotation, scaling, and translation matrices to apply transformations to mesh
vertices.

https://github.com/user-attachments/assets/1e44e171-277d-4cdf-bf4a-76d6296baa15

_[08.Transformation-Matrices.mp4](/videos/08.Transformation-Matrices.mp4)_

# Notes

## Perspective Projection

The two triangles that make up the viewer's angle to the screen-space projected
point and the 3D point are similar triangles, which share a constant ratio
$\frac{BC}{DE} = \frac{AB}{AD}$ as illustrated in the top-down view of the point's X axis
below.

![Illustration of perspective projection similar triangles](/images/perspective-projection-triangle.png)

This can be simplified:

$$
\frac{P'_x}{P_x}=\frac{1}{P_z}
$$

$$
P'_x=\frac{P_x}{P_z}
$$

Note $P_z$ in the denominator, indicating that the projected X is scaled by the
Z distance to the point. This is called the **perspective divide**.

Similarly, the Y perspective projection:

$$
P'_y=\frac{P_y}{P_z}
$$

## Left vs Right-handed Coordinate Systems

Mainly determines whether Z values grow "into" the screen away from the viewer,
or "out" of the display toward the viewer.

Left-handed coordinate systems refer to Z values that grow into the screen, as
used by DirectX.

Right-handed coordinate systems refer to Z values that grow out of the screen,
as used by OpenGL.

![Illustration of human hands mnemonic](/images/coordinate-system-hands.png)

_Illustration from https://www.oreilly.com/library/view/learn-arcore/9781788830409_

## Transformation

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
x' = r\cos(\alpha + \beta)
$$

$$
x' = r(\cos{\alpha} \cos{\beta} - \sin{\alpha} \sin{\beta})
$$

$$
x' = r \cos{\alpha} \cos{\beta} - r \sin{\alpha} \sin{\beta}
$$

You can substitute $r \cos{\alpha}$ with $x$, and $r \sin{\alpha}$ with $y$:

$$
x' = x \cos{\beta} - y \sin{\beta}
$$

Similarly,

$$
y' = r \sin(\alpha + \beta)
$$

$$
y' = r(\sin{\alpha} \cos{\beta} + \cos{\alpha} \sin{\beta})
$$

$$
y' = r \sin{\alpha} \cos{\beta} + r \cos{\alpha} \sin{\beta}
$$

$$
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

## Vector Math Review

### Vector Magnitude

**Magnitude** refers to the length of the vector:

$$
|\vec{v}|
$$

$$
|\vec{v}| = \sqrt{{v_x}^2 + {v_y}^2}
$$

### Vector Addition

Adding vectors is basically starting one vector from the end of the other:

$$
\vec{a} + \vec{b} = (a_x + b_x, a_y + b_y)
$$

![Illustration of two vectors being added](/images/vector-adding.png)

### Vector Subtraction

Subtraction is the same as addition, but invert/negate the second vector:

$$
\vec{a} - \vec{b} = (a_x - b_x, a_y - b_y)
$$

![First illustration of vector subtraction, showing original vectors](/images/vector-subtraction-01.png)

![Second illustration of vector subtraction, showing second vector negated](/images/vector-subtraction-02.png)

![Third illustration of vector subtraction, showing resulting vector](/images/vector-subtraction-03.png)

### Cross Product

The cross product helps to calculate the normal vector of a plane.

The cross product of two vectors $\vec{x} \times \vec{y}$ yields a vector that is
perpendicular to both of those vectors.

![Illustration showing two vectors and their cross product](/images/vector-cross-product.png)

To calculate the cross product:

$$
\vec{N} = \vec{a} \times \vec{b}
$$

$$
N_x = a_y b_z - a_z b_y
$$

$$
N_y = a_z b_x - a_x b_z
$$

$$
N_z = a_x b_y - a_y b_x
$$

There are two possible perpendicular vectors for any given pair of vectors.
The order of operands will determine which direction is calculated.

![Illustration showing the different operand order of cross product](/images/vector-cross-product-order.png)

The **magnitude** of the cross product is related to the angle between the two
input vectors:

$$
|\vec{a} \times \vec{b}| = |a| |b| \sin{\theta}
$$

[Resource for additional information on how to derive the cross product](https://youtu.be/-1nle1mGZSQ)

### Dot Product

The dot product of two vectors produces a scalar value of the sum of the
components of each given vector multiplied together.

$$
\vec{a} \cdot \vec{b} = {a_x}{b_x} + {a_y}{b_y}
$$

When used with unit vectors, the dot product can be used to produce a
"projection" of one vector onto the other.

![Illustration of dot product projection](/images/vector-dot-product-projection.png)

The more "aligned" the vectors are, the larger the dot product is. If they are
exactly the same, the dot product is $1$.

At a 90 degree offset, the dot product is $0$.

If the two vectors are complete opposites, the dot product is $-1$.

### Normalizing Vectors

A normalized vector is a vector with a magnitude of 1.

$$
\hat{a} = \frac{\vec{a}}{|\vec{a}|}
$$

If you don't care about the length of a vector, it's often better to express it
as a normalized vector.

## Back Face Culling

If we'd like to avoid rendering faces that are facing away from the camera, we
can simply compare their normal vector to the vector of the camera.

![Illustration of the the camera's vector relative to cube face normals](/images/back-face-culling-diagram.png)

Here's how we can get the normal vector of a triangle face:

![Illustration of calculating normal vector given 3 vertices](/images/getting-normal-of-triangle-face.png)

Note that we take our vertices in clockwise order, consistent with our chosen
coordinate system.

Once we have the normal vector, we can compare it to the camera ray vector using
the dot product to determine if the face is facing toward the camera or away
from it.

To find the camera ray vector, we simply subtract the camera position from the
point we are observing.

## Matrices

Just a way of expressing and manipulating a set of values in rows and columns.

$$
M = 
\begin{bmatrix}
-2 & 5 & 6 \\
 5 & 2 & 7
\end{bmatrix}
$$

Matrix $M$ has 2 rows and 3 columns; the *dimensions* are $2 \times 3$.

A matrix has a set of elements that can be referenced as followed:

$$
M_{3 \times 2} =
\begin{bmatrix}
m_{11} & m_{12} \\
m_{21} & m_{22} \\
m_{31} & m_{32}
\end{bmatrix}
$$

Matrices are useful for solving systems of equations:

$$
\left \lbrace
\begin{alignedat}{3}
    x + 2y -4z = 5 \\
    2x + y - 6z = 8 \\
    4x - y - 12z = 13
\end{alignedat}
\right.
$$

$$
\begin{bmatrix}
1 &  2 &  -4 & 5 \\
2 &  1 &  -6 & 8 \\
4 & -1 & -12 & 13
\end{bmatrix}
$$

In computer graphics, matrices are useful in converting sets of geometric data
into different coordinate systems. They can be used to apply translation,
rotation, projection, and many other transformations.

### Matrix Operations

#### Matrix Addition

Simply add each element together.

$$
\begin{bmatrix}
2 &  3 \\
1 & -5
\end{bmatrix} +
\begin{bmatrix}
3 & 1 \\
1 & 2
\end{bmatrix} =
\begin{bmatrix}
5 &  4 \\
2 & -3
\end{bmatrix}
$$

#### Matrix Subtraction

Simply subtract each element from each other.

$$
\begin{bmatrix}
2 &  3 \\
1 & -5
\end{bmatrix} -
\begin{bmatrix}
3 & 1 \\
1 & 2
\end{bmatrix} =
\begin{bmatrix}
-1 &  2 \\
 0 & -7
\end{bmatrix}
$$

#### Matrix Multiplication

Matrix multiplication is more complex. For each combination of row and column
you must multiply the row elements with the column elements and sum the results:

$$
\begin{bmatrix}
1 & 2 \\
3 & 4
\end{bmatrix} *
\begin{bmatrix}
5 & 6 \\
7 & 8
\end{bmatrix} =
\begin{bmatrix}
(1 \ast 5) + (2 \ast 7) & (1 \ast 6) + (2 \ast 8) \\
(3 \ast 5) + (4 \ast 7) & (3 \ast 6) + (4 \ast 8)
\end{bmatrix} =
\begin{bmatrix}
19 & 22 \\
43 & 50
\end{bmatrix}
$$

Multiplication is only possible when the number of columns on the left matrix
is equal to the number of rows on the right matrix.

The dimension of the resulting matrix will have the number of rows of the left
matrix and the number of columns of the right matrix.

$$
M_{N \times M} * M_{M \times P} = M_{N \times P}
$$

Matrix multiplication is not commutative:

$$
A * B \neq B * A
$$

### Identity Matrix

A square matrix with 1's in the diagonal and 0's everywhere else.

Any matrix multiplied by the identity matrix will return an unchanged result.

$$
\begin{bmatrix}
1 & 0 & 0 & 0 \\
0 & 1 & 0 & 0 \\
0 & 0 & 1 & 0 \\
0 & 0 & 0 & 1
\end{bmatrix}
$$

### Simple Rotation Matrix

Earlier, we determined that you can calculate the new $x$ and $y$ positions for
a given rotation $\alpha$ using simple trigonometry functions:

$$
x' = x \cos{\alpha} - y \sin{\alpha}
$$

$$
y' = y \cos{\alpha} + x \sin{\alpha}
$$

This can be represented in matrix form:

$$
\begin{bmatrix}
x' \\
y'
\end{bmatrix} =
\begin{bmatrix}
\cos{\alpha} & -\sin{\alpha} \\
\sin{\alpha} & \cos{\alpha}
\end{bmatrix} *
\begin{bmatrix}
x \\
y
\end{bmatrix} =
\begin{bmatrix}
x\cos{\alpha} - y\sin{\alpha} \\
x\sin{\alpha} + y\cos{\alpha}
\end{bmatrix}
$$

This matrix is called a 2D
[rotation matrix](https://en.wikipedia.org/wiki/Rotation_matrix):

$$
R =
\begin{bmatrix}
\cos{\theta} & -\sin{\theta} \\
\sin{\theta} & \cos{\theta}
\end{bmatrix}
$$

When it is multiplied against a set of coordinates, it produces a set of
transformed coordinates rotated by $\theta$.

### Transformation Matrices

In linear algebra, linear transformations can be represented by matrices.

$$
\begin{bmatrix}
m & m & m & m \\
m & m & m & m \\
m & m & m & m \\
m & m & m & m
\end{bmatrix} *
\begin{bmatrix}
x \\
y \\
z \\
w
\end{bmatrix}
$$

4x4 matrices are usually used to represent 3D transformations (scale,
translation, rotation, etc.)

We use 4x4 instead of 3x3 because some transformations (ex. translation) require
an extra row/column.

To enable multiplication, an extra component $w$ is added to our original
vector.

#### Scale Matrix

$$
\begin{bmatrix}
sx & 0  & 0  & 0 \\
0  & sy & 0  & 0 \\
0  & 0  & sz & 0 \\
0  & 0  & 0  & 1 \\
\end{bmatrix} *
\begin{bmatrix}
x \\
y \\
z \\
1
\end{bmatrix}
$$

Performing this multiplication yields the following:

$$
\begin{bmatrix}
(sx * x) + 0        + 0        + 0 \\
0        + (sy * y) + 0        + 0 \\
0        + 0        + (sz * z) + 0 \\
0        + 0        + 0        + (1 * 1) \\
\end{bmatrix} = 
\begin{bmatrix}
sx * x \\
sy * y \\
sz * z \\
1
\end{bmatrix}
$$

#### Translation Matrix

$$
\begin{bmatrix}
1 & 0 & 0 & tx \\
0 & 1 & 0 & ty \\
0 & 0 & 1 & tz \\
0 & 0 & 0 & 1
\end{bmatrix} *
\begin{bmatrix}
x \\
y \\
z \\
1
\end{bmatrix}
$$

Performing the multiplication yields the following:

$$
\begin{bmatrix}
x + 0 + 0 + tx \\
0 + y + 0 + ty \\
0 + 0 + z + tz \\
0 + 0 + 0 + 1
\end{bmatrix} =
\begin{bmatrix}
x + tx \\
y + ty \\
z + tz \\
1
\end{bmatrix}
$$

#### Rotation Matrix

*These are defined in a left-handed coordinate system, such that each axis is
rotated counter-clockwise around its axis. See
[direction](https://en.wikipedia.org/wiki/Rotation_matrix#Direction).*

The rotation matrix for the **X axis** looks like this:

$$
\begin{bmatrix}
1 & 0 & 0 & 0 \\
0 &  \cos{\alpha} & \sin{\alpha} & 0 \\
0 & -\sin{\alpha} & \cos{\alpha} & 0 \\
0 & 0 & 0 & 1
\end{bmatrix} *
\begin{bmatrix}
x \\
y \\
z \\
1
\end{bmatrix}
$$

The rotation matrix for the **Y axis** looks like this:

$$
\begin{bmatrix}
\cos{\alpha} & 0 & -\sin{\alpha} & 0 \\
0 & 1 & 0 & 0 \\
\sin{\alpha} & 0 &  \cos{\alpha} & 0 \\
0 & 0 & 0 & 1
\end{bmatrix} *
\begin{bmatrix}
x \\
y \\
z \\
1
\end{bmatrix}
$$

The rotation matrix for the **Z axis** looks like this:

$$
\begin{bmatrix}
 \cos{\alpha} & \sin{\alpha} & 0 & 0 \\
-\sin{\alpha} & \cos{\alpha} & 0 & 0 \\
0 & 0 & 1 & 0 \\
0 & 0 & 0 & 1
\end{bmatrix} *
\begin{bmatrix}
x \\
y \\
z \\
1
\end{bmatrix}
$$

### World Matrix

By combining translation, rotation, and scaling matrices via matrix
multiplication, we can express the location of an object in the world with a
single matrix.

$$
M_{world} = M_{translation} * M_{rotation} * M_{scale}
$$

$$
\vec{v}_{vertex} = 
M_{world} *
\begin{bmatrix}
x \\
y \\
z \\
1
\end{bmatrix}
$$

#### Order of Transformations

The order of transformations matters. The usual order is:

 1. Scale
 2. Rotate
 3. Translate

If these are performed out-of-order, it may result in unexpected values. For
example, if translation is applied before rotation such that the object has
moved away from the origin (0, 0), then rotation will be still be applied around
the origin of (0, 0), exaggerating the result of the rotation transformation.

