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


class InfRational:

    def __init__(self, rat: int, inf: int):
        self.rat = rat
        self.inf = inf
