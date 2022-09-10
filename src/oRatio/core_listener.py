from typing import Sequence


class CoreListener:

    def log(self, log: str) -> None:
        pass

    def read(self, script: str | Sequence[str]) -> None:
        pass

    def state_changed(self) -> None:
        pass

    def started_solving(self) -> None:
        pass

    def solution_found(self) -> None:
        pass

    def inconsistent_problem(self) -> None:
        pass