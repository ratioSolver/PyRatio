from oRatio.item import Atom, ComplexItem
from oRatio.timeline import *


class SVValue(Value):

    def __init__(self, _from: InfRational, _to: InfRational, atoms: set[Atom]):
        super(SVValue, self).__init__(_from, _to)
        self.atoms = atoms


class StateVariable(Timeline[SVValue]):

    def __init__(self, name: str, origin: InfRational, horizon: InfRational):
        super(StateVariable, self).__init__(name, origin, horizon)


class StateVariableExtractor(TimelineExtractor):

    def extract(self, itm: ComplexItem, json_tl) -> StateVariable:
        sv = StateVariable(
            itm.name, itm.solver.exprs["origin"].val, itm.solver.exprs["horizon"].val)

        for val in json_tl['values']:
            atoms: list[Atom] = []
            for atm_id in val.atoms:
                atoms.append(itm.solver.atoms[str(atm_id)])
            sv.values.append(
                SVValue(inf_rational_from_json(val['from']), inf_rational_from_json(val['to']), inf_rational_from_json(val['usage']), atoms))

        return sv
