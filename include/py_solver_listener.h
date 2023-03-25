#pragma once
#include "solver_listener.h"
#include <pybind11/pybind11.h>

namespace ratio::python
{
  class py_solver_listener : public ratio::solver_listener
  {
  public:
    py_solver_listener(ratio::solver &slv, pybind11::object &py_slv);
    ~py_solver_listener();

  private:
    void flaw_created(const ratio::flaw &f) override;
    void flaw_state_changed(const ratio::flaw &f) override;
    void flaw_cost_changed(const ratio::flaw &f) override;
    void flaw_position_changed(const ratio::flaw &f) override;
    void current_flaw(const ratio::flaw &f) override;

    void resolver_created(const ratio::resolver &r) override;
    void resolver_state_changed(const ratio::resolver &r) override;
    void current_resolver(const ratio::resolver &r) override;

    void causal_link_added(const ratio::flaw &f, const ratio::resolver &r) override;

  private:
    pybind11::object py_slv;
  };
} // namespace ratio::python
