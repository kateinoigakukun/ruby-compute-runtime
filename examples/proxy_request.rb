# Example:
# $ ./exe/ruby-compute-runtime ./examples/proxy_request.rb -o tmp/sample.wasm
# $ viceroy -v -C ./fastly.toml ./tmp/sample.wasm
#
# [IMPORTANT] Make sure your fastly.toml file has necessary configuration like below;
# ```
# [local_server]
#   [local_server.backends]
#     [local_server.backends.origin_0]
#       url = "https://www.ruby-lang.org"
# ```

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
