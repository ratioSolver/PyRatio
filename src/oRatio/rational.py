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

    def __str__(self) -> str:
        if self.denominator == 0:
            return '+inf' if self.numerator > 0 else '-inf'
        elif self.denominator == 1:
            return str(self.numerator)
        else:
            return str(self.numerator) + '/' + str(self.denominator)

    def __repr__(self) -> str:
        return self.__str__()


class InfRational:

    def __init__(self, rat: int, inf: int):
        self.rat = rat
        self.inf = inf

    def __str__(self) -> str:
        if self.inf.numerator == 0:
            return str(self.rat)
        else:
            return str(self.rat) + ' + ε' + str(self.inf)

    def __repr__(self) -> str:
        return self.__str__()
