from enum import Enum
from typing import Sequence
from fractions import Fraction


class State(Enum):
    Forbidden = 0
    Active = 1
    Inactive = 2


class Bound:
    def __init__(self, min: int = None, max: int = None):
        self.min = min
        self.max = max


class SolverListener:
    def flawCreated(self, id: int, causes: Sequence[int], label: str, state: State, position: Bound) -> None:
        pass

    def flawStateChanged(self, id: int, state: State) -> None:
        pass

    def flawCostChanged(self, id: int, cost: Fraction) -> None:
        pass

    def flawPositionChanged(self, id: int, position: Bound) -> None:
        pass

    def currentFlaw(self, id: int) -> None:
        pass

    def resolverCreated(self, id: int, effect: int, cost: Fraction, label: str, state: State) -> None:
        pass

    def resolverStateChanged(self, id: int, state: State) -> None:
        pass

    def currentResolver(self, id: int) -> None:
        pass

    def causalLinkAdded(self, flaw: int, resolver: int) -> None:
        pass
