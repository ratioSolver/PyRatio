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

    void tick(const semitone::rational &time);

    void starting(const std::unordered_set<ratio::core::atom *> &atoms);
    void start(const std::unordered_set<ratio::core::atom *> &atoms);

    void ending(const std::unordered_set<ratio::core::atom *> &atoms);
    void end(const std::unordered_set<ratio::core::atom *> &atoms);

  private:
    pybind11::object py_exec;
  };
} // namespace ratio::python