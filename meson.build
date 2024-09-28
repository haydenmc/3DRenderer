project('renderer', 'c',
    default_options: [
        'warning_level=3',          # All warnings enabled.
        'werror=true',              # Warnings as errors
        'c_std=c17',                # C99 standard
    ])

# Subprojects
sdl2_wrap = subproject('sdl2',
    default_options: [
        'test=false', # Don't build SDL tests
        'default_library=static',
        'werror=false',
        'warning_level=1',
    ])

# Other dependencies
# cc = meson.get_compiler('c')
# math_dep = cc.find_library('m', required : true)

# Executable
executable('renderer',
    dependencies: [
        dependency('sdl2'),
    ],
    c_pch: 'src/pch.h',
    include_directories: include_directories('vendor'),
    sources: [
        'src/Display.c',
        'src/Lighting.c',
        'src/Main.c',
        'src/Matrix.c',
        'src/Mesh.c',
        'src/Swap.c',
        'src/Texture.c',
        'src/Triangle.c',
        'src/Vector.c',
        'vendor/array/array.c',
        'vendor/upng/upng.c',
    ])