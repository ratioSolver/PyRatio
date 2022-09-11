from typing import Sequence
from oRatio.field import Field
from oRatio.constructor import Constructor
from oRatio.method import Method


class Type:

    def __init__(self, name: str, primitive: bool = False):
        self.name = name
        self.primitive = primitive
        self.supertypes: list[Type] = []
        self.constructors: list[Constructor] = []
        self.fields: dict[str, Field] = {}
        self.methods: dict[str, Sequence[Method]] = {}
        self.types: dict[str, Type] = {}
        self.predicates: dict[str, Predicate] = {}
        self.instances = set()

    def add_instance(self, itm):
        self.instances.add(itm)
        for tp in self.supertypes:
            tp.add_instance(itm)

    def __str__(self) -> str:
        if len(self.fields) == 0:
            return self.name
        else:
            return self.name + '(' + ', '.join(str(f) for f in self.fields.values()) + ')'

    def __repr__(self) -> str:
        return self.__str__()


class Predicate(Type):

    def __init__(self, name: str):
        super(Predicate, self).__init__(name, False)
