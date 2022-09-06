from oRatio.type import Type


class Item:

    def __init__(self, type: Type):
        self.type = type


class BoolItem(Item):
    pass


class ArithItem(Item):
    pass


class EnumItem(Item):
    pass


class ComplexItem(Item):

    def __init__(self, type: Type):
        super(ComplexItem, self).__init__(type)


class Atom(ComplexItem):
    def __init__(self, type: Type):
        super(Atom, self).__init__(type)
