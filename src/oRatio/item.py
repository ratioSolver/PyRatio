from enum import Enum
from oRatio.type import Type, Predicate
from oRatio.rational import InfRational


class LBool(Enum):
    FALSE = 0
    TRUE = 1
    UNDEFINED = 2


class State(Enum):
    Unified = 0
    Active = 1
    Undefined = 2


class Item:

    def __init__(self, solver, type: Type, id: int):
        self.solver = solver
        self.type = type
        self.id = id


class BoolItem(Item):
    def __init__(self, solver, id: int, lit: str, val: LBool):
        super(BoolItem, self).__init__(solver, solver.types["bool"], id)
        self.lit = lit
        self.val = val


class ArithItem(Item):
    def __init__(self, solver, type: Type, id: int, lin: str, lb: InfRational, ub: InfRational, val: InfRational):
        super(ArithItem, self).__init__(solver, type, id)
        self.lin = lin
        self.lb = lb
        self.ub = ub
        self.val = val


class StringItem(Item):
    def __init__(self, solver, id: int, val: str):
        super(StringItem, self).__init__(solver, solver.types["string"], id)
        self.val = val


class EnumItem(Item):
    def __init__(self, solver, type: Type, id: int, var: str, vals: set[Item]):
        super(EnumItem, self).__init__(solver, type, id)
        self.var = var
        self.vals = vals


class ComplexItem(Item):

    def __init__(self, env, type: Type):
        self.env = env
        self.type = type
        self.exprs = dict[str, Item]


class Atom(ComplexItem):
    def __init__(self, env, type: Predicate):
        super(Atom, self).__init__(env, type)
        self.state: State = State.Undefined
