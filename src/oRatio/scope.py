from typing import Sequence
from oRatio.field import Field


class Scope:

    def __init__(self, scope, fields: Sequence[Field] = []):
        self.scope = scope
        self.fields = dict[str, Field]
        for field in fields:
            self.fields[field.name] = field
