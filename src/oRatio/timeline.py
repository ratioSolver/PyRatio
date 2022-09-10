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


class TimelinesList(list[Timeline]):

    def __init__(self, solver):
        self.solver = solver
        self.timelines_extractors: dict[Type, TimelineExtractor] = {}

    def add_timelines_extractor(self, type: Type, timelines_extractor: TimelineExtractor) -> None:
        self.timelines_extractors[type] = timelines_extractor

    def state_changed(self):
        self.clear()
        atoms: dict[Item, list[Atom]] = {}

        for atm in self.solver.atoms.values():
            tau = atm.exprs.get('tau')
            if isinstance(tau, EnumItem):
                for itm in tau.vals:
                    if itm not in atoms:
                        atoms[itm] = []
                    atoms[itm].append(atm)
            elif tau:
                if tau not in atoms:
                    atoms[tau] = []
                atoms[tau].append(atm)

        for itm, atms in atoms.items():
            extractor = self.timelines_extractors[itm.type]
            if extractor:
                self.append(extractor.extract(itm, atms))
