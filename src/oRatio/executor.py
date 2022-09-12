from oRatioExecutorNative import new_instance, delete_instance, exec_tick, dont_start_tasks_yet, dont_end_tasks_yet, failed_tasks
from typing import Sequence
from oRatio.item import Atom
from oRatio.solver import Solver
from oRatio.executor_listener import ExecutorListener, Rational


class Executor:

    def __init__(self, solver: Solver, units_per_tick: Rational = Rational(1)):
        self.solver = solver
        self.executor_listeners: list[ExecutorListener] = []
        new_instance(self, units_per_tick)

    def dispose(self):
        delete_instance(self)

    def tick(self):
        exec_tick(self)

    def dont_start_yet(self, atoms: Atom | Sequence[Atom] | tuple[Atom, Rational] | Sequence[tuple[Atom, Rational]]) -> None:
        atms_delays: list[tuple[int, Rational]] = []
        if isinstance(atoms, Atom):
            atms_delays.append((atoms.id, Rational(1)))
        elif isinstance(atoms, Sequence[Atom]):
            for atm in atoms:
                atms_delays.append((atm.id, Rational(1)))
        elif isinstance(atoms, tuple[Atom, Rational]):
            atms_delays.append((atoms[0].id, atoms[1]))
        else:
            for atm in atoms:
                atms_delays.append((atm[0].id, atm[1]))
        dont_start_tasks_yet(self, atms_delays)

    def dont_end_yet(self, atoms: Atom | Sequence[Atom] | tuple[Atom, Rational] | Sequence[tuple[Atom, Rational]]) -> None:
        atms_delays: list[tuple[int, Rational]] = []
        if isinstance(atoms, Atom):
            atms_delays.append((atoms.id, Rational(1)))
        elif isinstance(atoms, Sequence[Atom]):
            for atm in atoms:
                atms_delays.append((atm.id, Rational(1)))
        elif isinstance(atoms, tuple[Atom, Rational]):
            atms_delays.append((atoms[0].id, atoms[1]))
        else:
            for atm in atoms:
                atms_delays.append((atm[0].id, atm[1]))
        dont_end_tasks_yet(self, atms_delays)

    def failure(self, atoms: Atom | Sequence[Atom]) -> None:
        atms_ids: list[int] = []
        if isinstance(atoms, Atom):
            atms_ids.append(atoms.id)
        else:
            for atm in atoms:
                atms_ids.append(atm.id)
        failed_tasks(self, atms_ids)

    def fire_tick(self, current_time: Rational) -> None:
        for l in self.executor_listeners:
            l.tick(current_time)

    def fire_starting_atoms(self, atoms: Sequence[int]) -> None:
        c_atoms: list[Atom] = []
        for atm_id in atoms:
            c_atoms.append(self.solver.atoms[str(atm_id)])
        for l in self.executor_listeners:
            l.starting_atoms(c_atoms)

    def fire_start_atoms(self, atoms: Sequence[int]) -> None:
        c_atoms: list[Atom] = []
        for atm_id in atoms:
            c_atoms.append(self.solver.atoms[str(atm_id)])
        for l in self.executor_listeners:
            l.start_atoms(c_atoms)

    def fire_ending_atoms(self, atoms: Sequence[int]) -> None:
        c_atoms: list[Atom] = []
        for atm_id in atoms:
            c_atoms.append(self.solver.atoms[str(atm_id)])
        for l in self.executor_listeners:
            l.ending_atoms(c_atoms)

    def fire_end_atoms(self, atoms: Sequence[int]) -> None:
        c_atoms: list[Atom] = []
        for atm_id in atoms:
            c_atoms.append(self.solver.atoms[str(atm_id)])
        for l in self.executor_listeners:
            l.end_atoms(c_atoms)

    def add_executor_listener(self, listener: ExecutorListener) -> None:
        self.executor_listeners.append(listener)
