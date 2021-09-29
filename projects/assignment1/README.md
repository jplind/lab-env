Simple program for testing your math library.

Add your class files (vec3.h, vec4.h, mat4.h) to the root directory and add them to the list of files located in CMakeLists.txt

Build and run test by executing the run_test.bat file, or by using CMake GUI.

Make sure to implement according to the following class descriptions, or modify the test so that it uses your own naming convention and coding style.

# Syntax
* `name: type` = variable or parameter/argument
* `f32` = 32 bit floating point number
* `const&` = const reference
* `+` = public variable or method
* `-` = private variable or method
* `name(args):return_type` = function or method

# class vec3
#### Members
    +x: f32
    +y: f32
    +z: f32
#### Constructors
    +vec3() // zero length vector
    +vec3(x: f32 const, y: f32 const, z: f32 const)
    +vec3(v: vec3 const&)
#### Methods and operators
    +operator=(rhs: vec3 const&):vec3
    +operator-():vec3
    +operator+(rhs: vec3 const&):vec3
    +operator+=(rhs: vec3 const&):void
    +operator-(rhs: vec3 const&):vec3
    +operator-=(rhs: vec3 const&):void
    +operator*=(scalar: f32 const):void
    +operator*(scalar: f32 const):vec3
    +operator==(rhs: vec3 const&):bool
    +operator!=(rhs: vec3 const&):bool
    +operator[](i: ui32 const):f32& // range [0,2]
#### Functions
    dot(a: vec3 const&, b: vec3 const&):f32
    length(v: vec3 const&):f32
    cross(a: vec3 const&, b: vec3 const&):vec3
    normalize(v: vec3 const&):vec3

# class vec4
#### Members
    +x: f32
    +y: f32
    +z: f32
    +w: f32
#### Constructors
    +vec4() // zero length vector
    +vec4(x: f32 const, y: f32 const, z: f32 const, w: f32 const)
    +vec4(v: vec4 const&)
#### Methods and operators
    +operator=(rhs: vec4 const&):vec4
    +operator-():vec4
    +operator+(rhs: vec4 const&):vec4
    +operator+=(rhs: vec4 const&):void
    +operator-(rhs: vec4 const&):vec4
    +operator-=(rhs: vec4 const&):void
    +operator*=(scalar: f32 const):void
    +operator*(scalar: f32 const):vec4
    +operator==(rhs: vec4 const&):bool
    +operator!=(rhs: vec4 const&):bool
    +operator[](i: ui32 const):f32 // range [0,3]
#### Functions
    dot(a: vec4 const&, b: vec4 const&):f32
    length(v: vec4 const&):f32
    normalize(v: vec4 const&):vec4

# class mat4
#### Members
    +m: vec4[4]
#### Constructors
    +mat4() // identity matrix
    +mat4(r0: vec4 const&, r1: vec4 const&, r2: vec4 const&, r3: vec4 const&)
    +mat4(m: mat4 const&)
#### Methods and operators
    +operator=(rhs: mat4 const&):mat4
    +operator*(rhs: mat4 const&):mat4
    +operator==(rhs: mat4 const&):bool
    +operator!=(rhs: mat4 const&):bool
    +operator[](i: ui32 const):vec4& // range [0,3]
#### Functions
    determinant(m: mat4 const&):f32
    inverse(m: mat4 const&):mat4 // returns identity upon failure
    transpose(m: mat4 const&):mat4
    rotationx(rad: f32 const):mat4 // radians
    rotationy(rad: f32 const):mat4
    rotationz(rad: f32 const):mat4
    rotationaxis(v: vec3 const&, rad: f32 const):mat4
#### Optional functions
    perspective(fovy: f32 const, aspect: f32 const, near: f32 const, far: f32 const): mat4
    lookat(eye: vec3 const&, at: vec3 const&, up: vec3 const&): mat4

Add `#define TEST_VIEW_PERSPECTIVE` to top of main.cc to test perspective and view matrix functions.

Add `#define USE_LH` to configure perspective and view to be left-handed instead of right-handed
