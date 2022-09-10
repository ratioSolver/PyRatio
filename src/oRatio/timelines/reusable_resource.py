from oRatio.item import Atom
from oRatio.timeline import *
from fractions import Fraction


class RRValue(Value):

    def __init__(self, _from: InfRational, _to: InfRational, amount: InfRational, atoms: set[Atom]):
        super(RRValue, self).__init__(_from, _to)
        self.amount = amount
        self.atoms = atoms


class ReusableResource(Timeline[RRValue]):

    def __init__(self, name: str, origin: InfRational, horizon: InfRational):
        super(ReusableResource, self).__init__(name, origin, horizon)


class ReusableResourceExtractor(TimelineExtractor):

    def extract(self, itm: Item, atoms: set[Atom]) -> ReusableResource:
        sv = ReusableResource(
            itm.name, itm.solver.exprs["origin"].val, itm.solver.exprs["horizon"].val)
        starting_values: dict[InfRational, list[Atom]] = {}
        ending_values: dict[InfRational, list[Atom]] = {}
        pulses: set[InfRational] = []

        for atm in atoms:
            start = atm.exprs["start"].val
            end = atm.exprs["end"].val
            pulses.add(start)
            pulses.add(end)
            if start not in starting_values:
                starting_values[start] = []
            starting_values[start].append(atm)
            if end not in ending_values:
                ending_values[end] = []
            ending_values[end].append(atm)

        sorted_pulses = sorted(pulses)
        overlapping_atoms: list[Atom] = []

        usage = Fraction()
        if sorted_pulses[0] in starting_values:
            for atm in starting_values[sorted_pulses[0]]:
                overlapping_atoms.add(atm)
                usage.__add__(
                    Fraction(atm.exprs['usage'].numerator, atm.exprs['usage'].denominator))
        if sorted_pulses[0] in ending_values:
            for atm in ending_values[sorted_pulses[0]]:
                overlapping_atoms.remove(atm)
                usage.__sub__(
                    Fraction(atm.exprs['usage'].numerator, atm.exprs['usage'].denominator))

        i = 1
        while i < len(sorted_pulses):
            sv.values.append(
                RRValue(sorted_pulses[i-1], sorted_pulses[i], usage, overlapping_atoms.copy()))
            if sorted_pulses[i] in starting_values:
                for atm in starting_values[sorted_pulses[i]]:
                    overlapping_atoms.add(atm)
                    usage.__add__(
                        Fraction(atm.exprs['usage'].numerator, atm.exprs['usage'].denominator))
            if sorted_pulses[i] in ending_values:
                for atm in ending_values[sorted_pulses[i]]:
                    overlapping_atoms.remove(atm)
                    usage.__sub__(
                        Fraction(atm.exprs['usage'].numerator, atm.exprs['usage'].denominator))

        return sv
