# Example:
# $ ./exe/ruby-compute-runtime ./examples/lobster.rb -o tmp/lobster.wasm --stdlib
# $ viceroy ./tmp/lobster.wasm &
# $ curl localhost:7676/?flip=right

require "/bundle/setup"
require "compute_runtime"
require "rackup/lobster"

include ComputeRuntime::RackHelper

begin
  req = ComputeRuntime::Request.new(ComputeRuntime::Request.body_downstream_get[0])
  run Rackup::Lobster.new.call(rack_env(req.method, req.uri))
rescue Exception => e
  body = ComputeRuntime::Body.new
  resp = ComputeRuntime::Response.new
  body.write "#{e}"
  resp.send_downstream body
end