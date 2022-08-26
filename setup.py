from setuptools import Extension, setup

setup(
    ext_modules=[
        Extension(
            name="oRatioNative",
            sources=['src/py_solver.cpp',
                     'src/py_core_listener.cpp'],
            include_dirs=['include',
                          '/usr/local/include/json',
                          '/usr/local/include/SeMiTONE',
                          '/usr/local/include/RiDDLe',
                          '/usr/local/include/ratioCore',
                          '/usr/local/include/oRatio',
                          '/usr/local/include/PlExA'],
            libraries=['json',
                       'SeMiTONE',
                       'RiDDLe',
                       'ratioCore',
                       'oRatio',
                       'PlExA'],
            library_dirs=['/usr/local/lib'],
            define_macros=[
                ("BUILD_LISTENERS", "1"),
                ("TEMPORAL_NETWORK_TYPE", "LA"),
                ("HEURISTIC_TYPES", "h_max"),
                ("DEFERRABLE_FLAWS", "1"),
                ("GRAPH_PRUNING", "1")
            ]
        )
    ],
    packages=['oRatio'],
    package_dir={"oRatio": "src/oRatio"},
    requires=['fractions']
)
