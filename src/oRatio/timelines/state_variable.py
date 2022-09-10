from oRatio.item import Atom
from oRatio.timeline import *


class SVValue(Value):

    def __init__(self, _from: InfRational, _to: InfRational, atoms: set[Atom]):
        super(SVValue, self).__init__(_from, _to)
        self.atoms = atoms


class StateVariable(Timeline[SVValue]):

    def __init__(self, name: str, origin: InfRational, horizon: InfRational):
        super(StateVariable, self).__init__(name, origin, horizon)


class StateVariableExtractor(TimelineExtractor):

    def extract(self, itm: Item, atoms: set[Atom]) -> StateVariable:
        sv = StateVariable(
            itm.name, itm.solver.exprs["origin"].val, itm.solver.exprs["horizon"].val)
        starting_values: dict[InfRational, list[Atom]] = {}
        ending_values: dict[InfRational, list[Atom]] = {}
        pulses: set[InfRational] = []

        for atm in atoms:
            start = atm.exprs["start"].val
            end = atm.exprs["end"].val
            pulses.insert(start)
            pulses.insert(end)
            if start not in starting_values:
                starting_values[start] = []
            starting_values[start].append(atm)
            if end not in ending_values:
                ending_values[end] = []
            ending_values[end].append(atm)

        sorted_pulses = sorted(pulses)
        overlapping_atoms: list[Atom] = []

        if sorted_pulses[0] in starting_values:
            overlapping_atoms.extend(starting_values[sorted_pulses[0]])
        if sorted_pulses[0] in ending_values:
            for atm in ending_values[sorted_pulses[0]]:
                overlapping_atoms.remove(atm)

        i = 1
        while i < len(sorted_pulses):
            sv.values.append(
                SVValue(sorted_pulses[i-1], sorted_pulses[i], overlapping_atoms.copy()))
            if sorted_pulses[i] in starting_values:
                overlapping_atoms.extend(starting_values[sorted_pulses[i]])
            if sorted_pulses[i] in ending_values:
                for atm in ending_values[sorted_pulses[i]]:
                    overlapping_atoms.remove(atm)

        return sv
