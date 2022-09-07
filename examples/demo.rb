# Example:
# $ ./exe/ruby-compute-runtime ./examples/hello.rb -o tmp/sample.wasm
# $ viceroy ./tmp/sample.wasm

require "compute_runtime"

body = ComputeRuntime::Body.new
resp = ComputeRuntime::Response.new

body.write "hello"
resp.send_downstream body
