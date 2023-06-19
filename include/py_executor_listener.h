#pragma once
#include "executor_listener.h"
#include <pybind11/pybind11.h>

namespace ratio::python
{
  class py_executor_listener : public ratio::executor::executor_listener
  {
  public:
    py_executor_listener(ratio::executor::executor &exec, pybind11::object &py_exec);
    ~py_executor_listener();

    void executor_state_changed(ratio::executor::executor_state state) override;

    void tick(const utils::rational &time) override;

    void starting(const std::unordered_set<ratio::atom *> &atoms) override;
    void start(const std::unordered_set<ratio::atom *> &atoms) override;

    void ending(const std::unordered_set<ratio::atom *> &atoms) override;
    void end(const std::unordered_set<ratio::atom *> &atoms) override;

  private:
    pybind11::object py_exec;
  };
} // namespace ratio::python