from oRatioExecutorNative import new_instance, delete_instance, exec_tick
from oRatio.solver import Solver


class Executor:

    def __init__(self, solver: Solver):
        self.solver = solver
        new_instance(self)

    def dispose(self):
        delete_instance(self)

    def tick(self):
        exec_tick(self)
