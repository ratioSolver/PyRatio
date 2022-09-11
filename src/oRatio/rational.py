class Rational:

    def __init__(self, num: int, den: int = 1):
        self.numerator = num
        self.denominator = den

    def is_positive(self) -> bool:
        return self.numerator > 0

    def is_negative(self) -> bool:
        return self.numerator < 0

    def is_infinite(self) -> bool:
        return self.denominator == 0

    def is_positive_infinite(self) -> bool:
        return self.is_positive() and self.is_infinite()

    def is_negative_infinite(self) -> bool:
        return self.is_negative() and self.is_infinite()

    def __eq__(self, other):
        if isinstance(other, Rational):
            return self.numerator == other.numerator and self.denominator == other.denominator
        elif isinstance(other, InfRational):
            return self.numerator == other.numerator and self.denominator == other.denominator and other.inf.numerator == 0
        return False

    def __lt__(self, other):
        if isinstance(other, Rational):
            if self.denominator == other.denominator:
                return self.numerator < other.numerator
            else:
                return self.numerator * other.denominator < self.denominator * other.numerator
        elif isinstance(other, InfRational):
            if self.denominator == other.denominator:
                return self.numerator < other.numerator or (self.numerator == other.numerator and other.inf.numerator < 0)
            else:
                return self.numerator * other.denominator < self.denominator * other.numerator or (self.numerator * other.denominator or self.denominator * other.numerator and other.inf.numerator < 0)
        return False

    def __str__(self) -> str:
        if self.denominator == 0:
            return '+inf' if self.numerator > 0 else '-inf'
        elif self.denominator == 1:
            return str(self.numerator)
        else:
            return str(self.numerator) + '/' + str(self.denominator)

    def __repr__(self) -> str:
        return self.__str__()


class InfRational(Rational):

    def __init__(self, rat: Rational, inf: Rational = Rational(0)):
        super(InfRational, self).__init__(rat.numerator, rat.denominator)
        if inf.numerator != 0:
            self.inf = inf

    def __lt__(self, other):
        if isinstance(other, Rational):
            return super(InfRational, self).__lt__(other) or (super(InfRational, self).__eq__(other) and self.inf.numerator < 0)
        elif isinstance(other, InfRational):
            return super(InfRational, self).__lt__(other) or (super(InfRational, self).__eq__(other) and self.inf < other.inf)
        return False

    def __str__(self) -> str:
        if self.inf.numerator == 0:
            return str(self.rat)
        else:
            return str(self.rat) + ' + ε' + str(self.inf)

    def __repr__(self) -> str:
        return self.__str__()


def rational_from_json(rational) -> Rational:
    if 'inf' in rational:
        return InfRational(rational_from_json(rational['rat'], rational['inf']))
    else:
        return Rational(rational['num'], rational['den'])
