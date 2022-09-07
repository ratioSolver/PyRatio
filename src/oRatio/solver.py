from oRatioNative import new_instance, delete_instance, read_script, read_files, solve_problem
from typing import Sequence
from oRatio.type import *
from oRatio.item import *
from oRatio.core_listener import CoreListener
from oRatio.solver_listener import SolverListener


class Solver:

    def __init__(self):
        new_instance(self)
        self.fields: dict[str, Field] = {}
        self.methods: dict[str, list[Method]] = {}
        self.exprs: dict[str, Item] = {}
        self.core_listeners: list[CoreListener] = []
        self.solver_listeners: list[SolverListener] = []

    def dispose(self):
        delete_instance(self)

    def read(self, riddle: str | Sequence[str]) -> bool:
        if isinstance(riddle, str):
            return read_script(self, riddle)
        else:
            return read_files(self, riddle)

    def solve(self) -> bool:
        return solve_problem(self)

    def fire_log(self, msg: str) -> None:
        for l in self.core_listeners:
            l.log(msg)

    def fire_read(self, riddle: str | Sequence[str]) -> bool:
        for l in self.core_listeners:
            l.read(riddle)

    def fire_state_changed(self) -> None:
        for l in self.core_listeners:
            l.state_changed()

    def fire_started_solving(self) -> None:
        for l in self.core_listeners:
            l.started_solving()

    def fire_solution_found(self) -> None:
        for l in self.core_listeners:
            l.solution_found()

    def fire_inconsistent_problem(self) -> None:
        for l in self.core_listeners:
            l.inconsistent_problem()

    def add_core_listener(self, listener: CoreListener) -> None:
        self.core_listeners.append(listener)

    def add_solver_listener(self, listener: SolverListener) -> None:
        self.solver_listeners.append(listener)
