from oRatio.item import Atom, ComplexItem
from oRatio.timeline import *


class CRValue(Value):

    def __init__(self, _from: InfRational, _to: InfRational, start: InfRational, end: InfRational, atoms: set[Atom]):
        super(CRValue, self).__init__(_from, _to)
        self.start = start
        self.end = end
        self.atoms = atoms

    def __str__(self) -> str:
        return super(CRValue, self).__str__() + ' ' + str(self.start) + ' -> ' + str(self.end)

    def __repr__(self) -> str:
        return self.__str__()


class ConsumableResource(Timeline[CRValue]):

    def __init__(self, name: str, origin: InfRational, horizon: InfRational, initial_amount: InfRational, capacity: InfRational):
        super(ConsumableResource, self).__init__(name, origin, horizon)
        self.initial_amount = initial_amount
        self.capacity = capacity


class ConsumableResourceExtractor(TimelineExtractor):

    def extract(self, itm: ComplexItem, json_tl) -> ConsumableResource:
        sv = ConsumableResource(itm.name,
                                itm.solver.exprs["origin"].val,
                                itm.solver.exprs["horizon"].val,
                                itm.exprs['initial_amount'],
                                itm.exprs['capacity'])

        for val in json_tl['values']:
            atoms: list[Atom] = []
            for atm_id in val['atoms']:
                atoms.append(itm.solver.atoms[str(atm_id)])
            sv.values.append(CRValue(rational_from_json(val['from']),
                                     rational_from_json(val['to']),
                                     rational_from_json(val['start']),
                                     rational_from_json(val['end']),
                                     atoms))

        return sv
