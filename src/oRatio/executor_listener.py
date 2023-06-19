from enum import Enum
from oRatio.item import Atom
from oRatio.rational import Rational
from typing import Sequence

class ExecutorState(Enum):
    Reasoning = 0
    Idle = 1
    Adapting = 2
    Executing = 3
    Finished = 4
    Failed = 5

class ExecutorListener:

    def executor_state_changed(self, state: ExecutorState) -> None:
        pass

    def tick(self, current_time: Rational) -> None:
        pass

    def starting_atoms(self, atoms: Sequence[Atom]) -> None:
        pass

    def start_atoms(self, atoms: Sequence[Atom]) -> None:
        pass

    def ending_atoms(self, atoms: Sequence[Atom]) -> None:
        pass

    def end_atoms(self, atoms: Sequence[Atom]) -> None:
        pass
