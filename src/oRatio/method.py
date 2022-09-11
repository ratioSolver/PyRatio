from typing import Sequence
from oRatio.type import Field


class Method:

    def __init__(self, name: str, parameters: Sequence[Field] = [], return_type=None):
        self.name = name
        self.pars = parameters
        self.return_type = return_type
        self.fields: dict[str, Field] = {}
        for par in parameters:
            self.fields[par.name] = par

    def __str__(self) -> str:
        return self.name + '(' + ', '.join(str(f) for f in self.fields.values()) + ')'

    def __repr__(self) -> str:
        return self.__str__()
