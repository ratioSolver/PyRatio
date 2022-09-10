from oRatio.item import Atom, EnumItem, Item
from oRatio.type import Type
from oRatio.rational import *
from typing import TypeVar, SupportsAbs, Generic


class Value:

    def __init__(self, _from: InfRational, _to: InfRational):
        self._from = _from
        self._to = _to


T = TypeVar('T', bound=SupportsAbs[Value])


class Timeline(Generic[T]):

    def __init__(self, name: str, origin: InfRational, horizon: InfRational):
        self.name = name
        self.origin = origin
        self.horizon = horizon
        self.values: list[T] = []


class TimelineExtractor:

    def extract(self, itm: Item, atoms: set[Atom]) -> Timeline:
        pass
