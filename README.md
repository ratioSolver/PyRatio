# PyRatio

Python API for the oRatio solver.

## Installation

Recursively clone this repository through `git clone --recurse-submodules https://github.com/ratioSolver/PyRatio`.

Compile and install [PlExA](https://github.com/ratioSolver/PlExA).

```shell
cd extern/plexa
mkdir build
cd build
cmake -DCOMPUTE_NAMES=ON ..
sudo make install
```

Go back at PyRatio level and compile the wheel using `python -m build`.

Install the wheel using `pip install` with the generated `.whl` files.

## Solving problems

The following example shows how to use the Python API for solving a problem instance.

```python
from oRatio import Solver

s = Solver()
if s.read(["example.rddl"]) and s.solve():
    tls = s.extract_timelines()

s.dispose()
```

In order to use the Python API, specifically, just import the `Solver` from the `oRatio` package.
Create a new instance of the solver. Read a RiDDLe script or a list of files and call the `solve` method.
Once solved, it is possible to extract the timelines to visualize the solution.
Remember, once done with the solver, to call the `dispose` method to release the resources.

## Programmatic access to the solution

The `read` and the `solve` methods populate the internal data structures of the solver.

```python
from oRatio import Solver

s = Solver()
if s.read('real a; a >= 5.0;') and s.solve():
    print(s.exprs['a'])

s.dispose()
```

As an example, the above code reads a simple RiDDLe snippet which creates a real variable `a` and constraints it to be greater or equal to `5.0`.
Once solved, it is possible to access to the `a` variable through the `exprs` attribute of the solver instance.
The above code, in particular, prints `5 [5, +inf]` on the console, indicating the value assigned to the `a` variable by the solver (i.e., `5`) and an estimate of the allowed values bounds (i.e., `[5, +inf]`).