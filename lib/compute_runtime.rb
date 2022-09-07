# frozen_string_literal: true

require "compute_runtime.so"
require_relative "compute_runtime/version"

module ComputeRuntime
  class Error < StandardError; end

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
