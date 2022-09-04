# Example:
# $ ./exe/ruby-compute-runtime ./examples/hello.rb -o tmp/sample.wasm
# $ viceroy ./tmp/sample.wasm

require "compute_runtime.so"

module Fastly
  module ComputeEdge
    class Error < StandardError; end
  end

  class Body
    attr_reader :handle
    def initialize
      @handle = ::ComputeRuntime::ABI.fastly_http_body_new
    end

    def write(data, body_write_end = 0)
      ::ComputeRuntime::ABI.fastly_http_body_write(@handle, data, body_write_end)
    end
  end

  class Response
    def initialize
      @handle = ::ComputeRuntime::ABI.fastly_http_resp_new
    end

    def send_downstream(body, streaming = 0)
      ::ComputeRuntime::ABI.fastly_http_resp_send_downstream(@handle, body.handle, streaming)
    end
  end
end

# require "json"
# require "rbconfig"

body = Fastly::Body.new
resp = Fastly::Response.new

# body.write JSON.pretty_generate(RbConfig::CONFIG)
body.write "hello"
resp.send_downstream body
