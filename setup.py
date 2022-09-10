from glob import glob
from setuptools import setup
from pybind11.setup_helpers import Pybind11Extension

ext_modules = [
    Pybind11Extension(
        'oRatioSolverNative',
        sources=['src/py_core_listener.cpp',
                 'src/py_solver_listener.cpp',
                 'src/py_solver.cpp'],
        include_dirs=['include',
                      '/usr/local/include/json',
                      '/usr/local/include/SeMiTONE',
                      '/usr/local/include/RiDDLe',
                      '/usr/local/include/ratioCore',
                      '/usr/local/include/oRatio'],
        libraries=['json',
                   'SeMiTONE',
                   'RiDDLe',
                   'ratioCore',
                   'oRatio'],
        library_dirs=['/usr/local/lib'],
        define_macros=[
            ('BUILD_LISTENERS', '1'),
            ('COMPUTE_NAMES', '1'),
            ('TEMPORAL_NETWORK_TYPE', 'LA'),
            ('HEURISTIC_TYPES', 'h_max'),
            ('DEFERRABLE_FLAWS', '1'),
            ('GRAPH_PRUNING', '1')
        ]
    ),
    Pybind11Extension(
        'oRatioExecutorNative',
        sources=['src/py_executor_listener.cpp',
                 'src/py_executor.cpp'],
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
            ('BUILD_LISTENERS', '1'),
            ('COMPUTE_NAMES', '1'),
            ('TEMPORAL_NETWORK_TYPE', 'LA'),
            ('HEURISTIC_TYPES', 'h_max'),
            ('DEFERRABLE_FLAWS', '1'),
            ('GRAPH_PRUNING', '1')
        ]
    )
]

setup(
    ext_modules=ext_modules,
    packages=['oRatio'],
    package_dir={'oRatio': 'src/oRatio'}
)
