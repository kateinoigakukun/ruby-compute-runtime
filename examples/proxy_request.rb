# Example:
# $ ./exe/ruby-compute-runtime ./examples/proxy_request.rb -o tmp/sample.wasm
# $ viceroy ./tmp/sample.wasm

require "compute_runtime"

req = ComputeRuntime::Request.new
resp = ComputeRuntime::Response.new
begin
    req.method = "GET"
    req.uri = "https://www.ruby-lang.org/en/"
    result = req.send ComputeRuntime::Body.new.handle, "origin_0"
    resp.send_downstream ComputeRuntime::Body.new(result[1])
rescue Exception => e
    body.write "#{e}"
    resp.send_downstream body
end
