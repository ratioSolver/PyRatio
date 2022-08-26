from xmlrpc.client import Boolean
from oRatioNative import new_instance, delete_instance, read_riddle, solve_problem


class Solver:

    def __init__(self):
        new_instance(self)

    def dispose(self):
        delete_instance(self)

    def read(self, riddle) -> Boolean:
        read_riddle(self, riddle)

    def solve(self) -> Boolean:
        solve_problem(self)
