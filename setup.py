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
                      '/usr/local/include/utils',
                      '/usr/local/include/json',
                      '/usr/local/include/SeMiTONE',
                      '/usr/local/include/RiDDLe',
                      '/usr/local/include/oRatioSolver'],
        libraries=['utils',
                   'json',
                   'SeMiTONE',
                   'RiDDLe',
                   'oRatioSolver'],
        library_dirs=['/usr/local/lib'],
        define_macros=[
            ('INTEGER_TYPE_LONG', '1'),
            ('BUILD_LISTENERS', '1'),
            ('COMPUTE_NAMES', '1'),
            ('LA_TN', '1'),
            ('H_MAX', '1'),
            ('DEFERRABLE_FLAWS', '1'),
            ('GRAPH_PRUNING', '1')
        ]
    ),
    Pybind11Extension(
        'oRatioExecutorNative',
        sources=['src/py_executor_listener.cpp',
                 'src/py_executor.cpp'],
        include_dirs=['include',
                      '/usr/local/include/utils',
                      '/usr/local/include/json',
                      '/usr/local/include/SeMiTONE',
                      '/usr/local/include/RiDDLe',
                      '/usr/local/include/oRatioSolver',
                      '/usr/local/include/PlExA'],
        libraries=['utils',
                   'json',
                   'SeMiTONE',
                   'RiDDLe',
                   'oRatioSolver',
                   'PlExA'],
        library_dirs=['/usr/local/lib'],
        define_macros=[
            ('INTEGER_TYPE_LONG', '1'),
            ('BUILD_LISTENERS', '1'),
            ('COMPUTE_NAMES', '1'),
            ('LA_TN', '1'),
            ('H_MAX', '1'),
            ('DEFERRABLE_FLAWS', '1'),
            ('GRAPH_PRUNING', '1')
        ]
    )
]

setup(
    ext_modules=ext_modules,
    packages=['oRatio', 'oRatio.timelines'],
    package_dir={'oRatio': 'src/oRatio',
                 'oRatio.timelines': 'src/oRatio/timelines'}
)
