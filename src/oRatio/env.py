from oRatio.item import Item

class Env:

    def __init__(self, env):
        self.env = env
        self.exprs = dict[str, Item]
