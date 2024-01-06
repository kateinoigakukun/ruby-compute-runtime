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
require "uri"

req = ComputeRuntime::Request.new
resp = ComputeRuntime::Response.new
begin
    original_request = ComputeRuntime::Request.new(ComputeRuntime::Request.body_downstream_get[0])
    req.method = original_request.method
    req.uri = "https://www.ruby-lang.org/" + URI(original_request.uri).path
    result = req.send ComputeRuntime::Body.new.handle, "origin_0"
    resp.send_downstream ComputeRuntime::Body.new(result[1])
rescue Exception => e
    body.write "#{e}"
    resp.send_downstream body
end
