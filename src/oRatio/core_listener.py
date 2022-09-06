from typing import Sequence


class CoreListener:
    def log(self, log: str) -> None:
        pass

    def read(self, script: str) -> None:
        pass

    def read(self, files: Sequence[str]) -> None:
        pass

    def stateChanged(self) -> None:
        pass

    def startedSolving(self) -> None:
        pass

    def solutionFound(self) -> None:
        pass

    def inconsistentProblem(self) -> None:
        pass
