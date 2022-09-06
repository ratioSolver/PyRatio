from oRatioNative import new_instance, delete_instance, read_riddle, solve_problem
from typing import Sequence
from oRatio.scope import Scope
from oRatio.env import Env


class Solver(Scope, Env):

    def __init__(self):
        super(Solver, self).__init__(None)
        new_instance(self)

    def dispose(self):
        delete_instance(self)

    def read(self, riddle: str | Sequence[str]) -> bool:
        return read_riddle(self, riddle)

    def solve(self) -> bool:
        return solve_problem(self)
