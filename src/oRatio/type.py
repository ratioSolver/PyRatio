from typing import Sequence
from oRatio.field import Field
from oRatio.constructor import Constructor
from oRatio.method import Method


class Type:

    def __init__(self, name: str, primitive: bool = False):
        self.name = name
        self.primitive = primitive
        self.constructors: list[Constructor] = []
        self.fields: dict[str, Field] = {}
        self.methods: dict[str, Sequence[Method]] = {}
        self.supertypes: list[Type] = []


class Predicate(Type):

    def __init__(self, name: str):
        super(Predicate, self).__init__(name, False)
