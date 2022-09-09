#pragma once
#include "solver_listener.h"
#include <pybind11/pybind11.h>

namespace ratio::python
{
  class py_solver_listener : public ratio::solver::solver_listener
  {
  public:
    py_solver_listener(ratio::solver::solver &slv, pybind11::object &py_slv);
    ~py_solver_listener();

  private:
    void flaw_created(const ratio::solver::flaw &f) override;
    void flaw_state_changed(const ratio::solver::flaw &f) override;
    void flaw_cost_changed(const ratio::solver::flaw &f) override;
    void flaw_position_changed(const ratio::solver::flaw &f) override;
    void current_flaw(const ratio::solver::flaw &f) override;

    void resolver_created(const ratio::solver::resolver &r) override;
    void resolver_state_changed(const ratio::solver::resolver &r) override;
    void current_resolver(const ratio::solver::resolver &r) override;

    void causal_link_added(const ratio::solver::flaw &f, const ratio::solver::resolver &r) override;

  private:
    pybind11::object py_slv;
  };
} // namespace ratio::python
