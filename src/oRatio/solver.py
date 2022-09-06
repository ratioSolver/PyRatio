from oRatioNative import new_instance, delete_instance, read_riddle, solve_problem
from typing import Sequence
from oRatio.item import *
from oRatio.type import *


class Solver:

    def __init__(self):
        new_instance(self)
        self.fields = dict[str, Field]
        self.methods = dict[str, Sequence[Method]]
        self.exprs = dict[str, Item]

    def dispose(self):
        delete_instance(self)

    def read(self, riddle: str | Sequence[str]) -> bool:
        return read_riddle(self, riddle)

    def solve(self) -> bool:
        return solve_problem(self)
