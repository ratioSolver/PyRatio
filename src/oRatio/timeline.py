from oRatio.rational import Rational


class Value:

    def __init__(self):
        self._from = Rational(-1, 0)
        self._to = Rational(1, 0)


class Timeline:

    def __init__(self):
        self.values: list[Value] = []
