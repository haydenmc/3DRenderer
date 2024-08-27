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

executable('renderer',
    dependencies: [
        dependency('sdl2')
    ],
    c_pch: 'src/pch.h',
    sources: [
        'src/Main.c',
        'src/Display.c',
        'src/Vector.c',
    ])