# PyRatio

Python API for the oRatio solver.

Recursively clone this repository through `git clone --recurse-submodules https://github.com/ratioSolver/PyRatio`.

Compile and install [PlExA](https://github.com/ratioSolver/PlExA).

```
cd extern/plexa
mkdir build
cd build
cmake -DCOMPUTE_NAMES=ON ..
sudo make install
```

Go back at PyRatio level and compile the wheel using `python -m build`.

Install the wheel using `pip install` with the generated `.whl` files.