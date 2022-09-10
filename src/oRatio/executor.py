from oRatioExecutorNative import new_instance, delete_instance, exec_tick
from typing import Sequence
from oRatio.solver import Solver
from oRatio.executor_listener import ExecutorListener, Rational


class Executor:

    def __init__(self, solver: Solver):
        self.solver = solver
        self.executor_listeners: list[ExecutorListener] = []
        new_instance(self)

    def dispose(self):
        delete_instance(self)

    def tick(self):
        exec_tick(self)

    def fire_tick(self, current_time: Rational) -> None:
        for l in self.executor_listeners:
            l.tick(current_time)

    def fire_starting_atoms(self, atoms: Sequence[int]) -> None:
        for l in self.executor_listeners:
            l.starting_atoms(atoms)

    def fire_start_atoms(self, atoms: Sequence[int]) -> None:
        for l in self.executor_listeners:
            l.start_atoms(atoms)

    def fire_ending_atoms(self, atoms: Sequence[int]) -> None:
        for l in self.executor_listeners:
            l.ending_atoms(atoms)

    def fire_end_atoms(self, atoms: Sequence[int]) -> None:
        for l in self.executor_listeners:
            l.end_atoms(atoms)

    def add_executor_listener(self, listener: ExecutorListener) -> None:
        self.executor_listeners.append(listener)
