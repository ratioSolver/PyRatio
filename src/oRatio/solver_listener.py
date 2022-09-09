from enum import Enum
from typing import Sequence
from oRatio.rational import Rational


class State(Enum):
    Forbidden = 0
    Active = 1
    Inactive = 2


class Bound:
    def __init__(self, min: int = None, max: int = None):
        self.min = min
        self.max = max


class SolverListener:

    def flaw_created(self, id: int, causes: Sequence[int], label: str, state: State, position: Bound) -> None:
        pass

    def flaw_state_changed(self, id: int, state: State) -> None:
        pass

    def flaw_cost_changed(self, id: int, cost: Rational) -> None:
        pass

    def flaw_position_changed(self, id: int, position: Bound) -> None:
        pass

    def current_flaw(self, id: int) -> None:
        pass

    def resolver_created(self, id: int, effect: int, cost: Rational, label: str, state: State) -> None:
        pass

    def resolver_state_changed(self, id: int, state: State) -> None:
        pass

    def current_resolver(self, id: int) -> None:
        pass

    def causal_link_added(self, flaw: int, resolver: int) -> None:
        pass
