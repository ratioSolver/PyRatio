from oRatio import Type


class Item:

    def __init__(self, type: Type):
        self.type = type


class BoolItem(Item):
    pass


class ArithItem(Item):
    pass


class EnumItem(Item):
    pass


class Atom(Item):
    pass
