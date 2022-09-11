class Field:

    def __init__(self, type, name: str):
        self.type = type
        self.name = name

    def __str__(self) -> str:
        return self.type.name + ' ' + self.name

    def __repr__(self) -> str:
        return self.__str__()
