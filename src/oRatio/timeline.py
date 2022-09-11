from oRatio.item import ComplexItem
from oRatio.rational import *
from typing import TypeVar, SupportsAbs, Generic


class Value:

    def __init__(self, _from: InfRational, _to: InfRational):
        self._from = _from
        self._to = _to

    def __str__(self) -> str:
        if self._from.rat < self._to.rat or self._from.inf < self._to.inf:
            return '[' + str(self._from) + ', ' + str(self._to) + ']'
        else:
            return str(self._from)

    def __repr__(self) -> str:
        return self.__str__()


T = TypeVar('T', bound=SupportsAbs[Value])


class Timeline(Generic[T]):

    def __init__(self, name: str, origin: InfRational, horizon: InfRational):
        self.name = name
        self.origin = origin
        self.horizon = horizon
        self.values: list[T] = []

    def __str__(self) -> str:
        return self.__class__.__name__ + ' ' + self.name

    def __repr__(self) -> str:
        return self.__str__()


class TimelineExtractor:

    def extract(self, itm: ComplexItem, json_tl) -> Timeline:
        pass
