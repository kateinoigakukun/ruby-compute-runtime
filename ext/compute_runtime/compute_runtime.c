#include "compute_runtime.h"

VALUE rb_mComputeRuntime;

void
Init_compute_runtime(void)
{
  rb_mComputeRuntime = rb_define_module("ComputeRuntime");
}
