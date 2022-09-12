from oRatio.item import Atom, ComplexItem
from oRatio.timeline import *


class SVValue(Value):

    def __init__(self, _from: InfRational, _to: InfRational, atoms: set[Atom]):
        super(SVValue, self).__init__(_from, _to)
        self.atoms = atoms

    def __str__(self) -> str:
        if len(self.atoms) == 0:
            return super(SVValue, self).__str__() + ' none'
        elif len(self.atoms) == 1:
            atm = next(iter(self.atoms))
            return super(SVValue, self).__str__() + ' ' + atm.type.name
        else:
            return super(SVValue, self).__str__() + ' [' + ', '.join(atm.type.name for atm in self.atoms) + ']'

    def __repr__(self) -> str:
        return self.__str__()


class StateVariable(Timeline[SVValue]):

    def __init__(self, name: str, origin: InfRational, horizon: InfRational):
        super(StateVariable, self).__init__(name, origin, horizon)


class StateVariableExtractor(TimelineExtractor):

    def extract(self, itm: ComplexItem, json_tl) -> StateVariable:
        sv = StateVariable(
            itm.name, itm.solver.exprs["origin"].val, itm.solver.exprs["horizon"].val)

        for val in json_tl['values']:
            atoms: list[Atom] = []
            for atm_id in val['atoms']:
                atoms.append(itm.solver.atoms[str(atm_id)])
            sv.values.append(
                SVValue(rational_from_json(val['from']),
                        rational_from_json(val['to']),
                        atoms))

        return sv
