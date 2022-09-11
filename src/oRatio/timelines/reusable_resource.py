from oRatio.item import Atom, ComplexItem
from oRatio.timeline import *


class RRValue(Value):

    def __init__(self, _from: InfRational, _to: InfRational, amount: InfRational, atoms: set[Atom]):
        super(RRValue, self).__init__(_from, _to)
        self.amount = amount
        self.atoms = atoms


class ReusableResource(Timeline[RRValue]):

    def __init__(self, name: str, origin: InfRational, horizon: InfRational, capacity: InfRational):
        super(ReusableResource, self).__init__(name, origin, horizon)
        self.capacity = capacity


class ReusableResourceExtractor(TimelineExtractor):

    def extract(self, itm: ComplexItem, json_tl) -> ReusableResource:
        sv = ReusableResource(itm.name,
                              itm.solver.exprs["origin"].val,
                              itm.solver.exprs["horizon"].val,
                              itm.exprs['capacity'])

        for val in json_tl['values']:
            atoms: list[Atom] = []
            for atm_id in val['atoms']:
                atoms.append(itm.solver.atoms[str(atm_id)])
            sv.values.append(RRValue(rational_from_json(val['from']),
                                     rational_from_json(val['to']),
                                     rational_from_json(val['usage']),
                                     atoms))

        return sv
