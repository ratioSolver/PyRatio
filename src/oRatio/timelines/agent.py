from oRatio.item import Atom
from oRatio.timeline import *


class Action(Value):

    def __init__(self, _from: InfRational, _to: InfRational, atom: Atom):
        super(Action, self).__init__(_from, _to)
        self.atom = atom


class Agent(Timeline[Action]):

    def __init__(self, name: str, origin: InfRational, horizon: InfRational):
        super(Agent, self).__init__(name, origin, horizon)


class AgentExtractor(TimelineExtractor):

    def extract(self, itm: Item, atoms: set[Atom]) -> Agent:
        agnt = Agent(
            itm.name, itm.solver.exprs['origin'].val, itm.solver.exprs['horizon'].val)
        starting_values: dict[InfRational, list[Atom]] = {}
        pulses: set[InfRational] = []

        for atm in atoms:
            start = atm.exprs['start'].val if 'start' in atm.exprs else atm.exprs['at'].val
            pulses.add(start)
            if start not in starting_values:
                starting_values[start] = []
            starting_values[start].append(atm)

        for pulse in sorted(pulses):
            for atm in starting_values[pulse]:
                agnt.values.append(Action(atm.exprs['start'].val, atm.exprs['end'].val, atm) if 'start' in atm.exprs else Action(
                    atm.exprs['at'].val, atm.exprs['at'].val, atm))

        return agnt
