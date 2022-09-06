from typing import Sequence
from oRatio.type import Field


class Constructor:

    def __init__(self, parameters: Sequence[Field] = []):
        self.pars = parameters
        self.fields = dict[str, Field]
        for par in parameters:
            self.fields[par.name] = par
