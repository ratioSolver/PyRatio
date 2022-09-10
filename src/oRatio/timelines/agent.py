from oRatio.item import Atom, ComplexItem
from oRatio.timeline import *


class Action(Value):

    def __init__(self, _from: InfRational, _to: InfRational, atom: Atom):
        super(Action, self).__init__(_from, _to)
        self.atom = atom


class Agent(Timeline[Action]):

    def __init__(self, name: str, origin: InfRational, horizon: InfRational):
        super(Agent, self).__init__(name, origin, horizon)


class AgentExtractor(TimelineExtractor):

    def extract(self, itm: ComplexItem, json_tl) -> Agent:
        agnt = Agent(
            itm.name, itm.solver.exprs['origin'].val, itm.solver.exprs['horizon'].val)

        for atm_id in json_tl['values']:
            atm = itm.solver.atoms[atm_id]
            agnt.values.append(Action(atm.exprs['start'].val, atm.exprs['end'].val, atm) if 'start' in atm.exprs else Action(
                atm.exprs['at'].val, atm.exprs['at'].val, atm))

        return agnt
