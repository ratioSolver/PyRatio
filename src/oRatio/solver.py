from oRatioSolverNative import new_instance, delete_instance, read_script, read_files, solve_problem, extract_solver_timelines
from typing import Sequence
import json
from oRatio.timeline import Timeline, TimelineExtractor
from oRatio.type import *
from oRatio.item import *
from oRatio.core_listener import CoreListener
from oRatio.solver_listener import SolverListener, Rational, Bound
from oRatio.timelines.agent import AgentExtractor
from oRatio.timelines.state_variable import StateVariableExtractor
from oRatio.timelines.reusable_resource import ReusableResourceExtractor
from oRatio.timelines.consumbale_resource import ConsumableResourceExtractor


class Solver:

    def __init__(self, build_timelines_at_state_change: bool = False, build_timelines_at_solution_found: bool = True):
        self.build_timelines_at_state_change = build_timelines_at_state_change
        self.build_timelines_at_solution_found = build_timelines_at_solution_found
        self.fields: dict[str, Field] = {}
        self.methods: dict[str, list[Method]] = {}
        self.types: dict[str, Type] = {}
        self.predicates: dict[str, Predicate] = {}
        self.exprs: dict[str, Item] = {}
        self.items: dict[str, Atom] = {}
        self.atoms: dict[str, Atom] = {}
        self.core_listeners: list[CoreListener] = []
        self.solver_listeners: list[SolverListener] = []

        new_instance(self)

        self.timelines_extractors: dict[Type, TimelineExtractor] = {}
        self.timelines_extractors[self.types["Agent"]
                                  ] = AgentExtractor()
        self.timelines_extractors[self.types["StateVariable"]
                                  ] = StateVariableExtractor()
        self.timelines_extractors[self.types["ReusableResource"]
                                  ] = ReusableResourceExtractor()
        self.timelines_extractors[self.types["ConsumableResource"]
                                  ] = ConsumableResourceExtractor()

    def dispose(self):
        delete_instance(self)

    def read(self, riddle: str | Sequence[str]) -> bool:
        if isinstance(riddle, str):
            return read_script(self, riddle)
        else:
            return read_files(self, riddle)

    def solve(self) -> bool:
        return solve_problem(self)

    def extract_timelines(self) -> list[Timeline]:
        j_tls = json.loads(extract_solver_timelines(self))

        timelines: list[Timeline] = []
        for j_tl in j_tls:
            extractor = self.timelines_extractors[j_tl['type']]
            if extractor:
                timelines.append(extractor.extract(
                    self.items[j_tl['id']], j_tl))

        return timelines

    def fire_log(self, msg: str) -> None:
        for l in self.core_listeners:
            l.log(msg)

    def fire_read(self, riddle: str | Sequence[str]) -> bool:
        for l in self.core_listeners:
            l.read(riddle)

    def fire_state_changed(self) -> None:
        if self.build_timelines_at_state_change:
            self.timelines.state_changed()
        for l in self.core_listeners:
            l.state_changed()

    def fire_started_solving(self) -> None:
        for l in self.core_listeners:
            l.started_solving()

    def fire_solution_found(self) -> None:
        if self.build_timelines_at_solution_found:
            self.timelines.state_changed()
        for l in self.core_listeners:
            l.solution_found()

    def fire_inconsistent_problem(self) -> None:
        for l in self.core_listeners:
            l.inconsistent_problem()

    def fire_flaw_created(self, id: int, causes: Sequence[int], label: str, state: State, position: Bound) -> None:
        for l in self.solver_listeners:
            l.flaw_created(id, causes, label, state, position)

    def fire_flaw_state_changed(self, id: int, state: State) -> None:
        for l in self.solver_listeners:
            l.flaw_state_changed(id, state)

    def fire_flaw_cost_changed(self, id: int, cost: Rational) -> None:
        for l in self.solver_listeners:
            l.flaw_cost_changed(id, cost)

    def fire_flaw_position_changed(self, id: int, position: Bound) -> None:
        for l in self.solver_listeners:
            l.flaw_position_changed(id, position)

    def fire_current_flaw(self, id: int) -> None:
        for l in self.solver_listeners:
            l.current_flaw(id)

    def fire_resolver_created(self, id: int, effect: int, cost: Rational, label: str, state: State) -> None:
        for l in self.solver_listeners:
            l.resolver_created(id, effect, cost, label, state)

    def fire_resolver_state_changed(self, id: int, state: State) -> None:
        for l in self.solver_listeners:
            l.resolver_state_changed(id, state)

    def fire_current_resolver(self, id: int) -> None:
        for l in self.solver_listeners:
            l.current_resolver(id)

    def fire_causal_link_added(self, flaw: int, resolver: int) -> None:
        for l in self.solver_listeners:
            l.causal_link_added(flaw, resolver)

    def add_core_listener(self, listener: CoreListener) -> None:
        self.core_listeners.append(listener)

    def add_solver_listener(self, listener: SolverListener) -> None:
        self.solver_listeners.append(listener)
