from oRatio.item import Atom
from oRatio.rational import Rational
from typing import Sequence


class ExecutorListener:

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
