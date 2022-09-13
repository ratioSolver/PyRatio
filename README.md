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
Both the `read` and the `solve` methods return a boolean indicating whether a trivial inconsistency has been found in the problem and whether a solution has been found.
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
Once solved, it is possible to access to the `a` variable through the `exprs` attribute of the solver's instance.
The above code, in particular, prints `5 [5, +inf]` on the console, indicating the value assigned to the `a` variable by the solver (i.e., `5`) and an estimate of the allowed values bounds (i.e., `[5, +inf]`).

## Executing the solutions

Once a problem has been solved, it is possible to execute the generated solution and, possibly, to dynamically update it along the execution.
To this end, an `Executor` has to be created and a listener must be registered to it.
Similar to a callback, the listener allows to receive information from the executor about the tasks to be executed.
The following listener simply prints the starting/ending tasks on the console.

```python
from oRatio import Executor, ExecutorListener

class ExecListener(ExecutorListener):

    def __init__(self, executor: Executor):
        self.executor = executor

    def tick(self, current_time: Rational) -> None:
        print(current_time)

    def starting_atoms(self, atoms: Sequence[Atom]) -> None:
        print(atoms)

    def start_atoms(self, atoms: Sequence[Atom]) -> None:
        print(atoms)

    def ending_atoms(self, atoms: Sequence[Atom]) -> None:
        print(atoms)

    def end_atoms(self, atoms: Sequence[Atom]) -> None:
        print(atoms)
```

Note that while the `start_atoms` (`end_atoms`) methods are meant to start (stop) the execution of tasks, the `starting_atoms` (`ending_atoms`) methods are meant to ask for the ability to start (end) the tasks.
In particular, within these methods, it is possible to delay the start (termination) of tasks by invoking the executor's `dont_start_yet` (`dont_end_yet`) method, passing the atoms whose start (termination) is to be delayed as a parameter.

```python
def starting_atoms(self, atoms: Sequence[Atom]) -> None:
    for atm in atoms:
        if not_ready(atm):
            self.executor.dont_start_yet(atm)
```

Finally, the following code shows how to register the listener to the executor.

```python
s = Solver()
e = Executor(s)
e.add_executor_listener(ExecListener(e))

if s.read(['example.rddl']) and s.solve():
    tls = s.extract_timelines()
```

The last aspect concerns the passage of time.
This is handled by invoking the executor's tick method.
This method can be invoked, for example, every second, by means of a timer.

```python
import time

starttime = time.time()
while True:
    e.tick()
    time.sleep(1.0 - ((time.time() - starttime) % 1.0))
```
