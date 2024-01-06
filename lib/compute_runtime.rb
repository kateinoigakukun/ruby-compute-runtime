# frozen_string_literal: true

require "compute_runtime.so"
require_relative "compute_runtime/version"

module ComputeRuntime
  class Error < StandardError; end

  class Request
    attr_reader :handle
    def initialize(handle = nil)
      @handle = handle || ::ComputeRuntime::ABI.fastly_http_req_new
    end

    def method
      ::ComputeRuntime::ABI.fastly_http_req_method_get(@handle)
    end

    def method=(method)
      ::ComputeRuntime::ABI.fastly_http_req_method_set(@handle, method)
    end

    def uri
      ::ComputeRuntime::ABI.fastly_http_req_uri_get(@handle)
    end

    def uri=(uri)
      ::ComputeRuntime::ABI.fastly_http_req_uri_set(@handle, uri)
    end

    def send(body, backend)
      ::ComputeRuntime::ABI.fastly_http_req_send(@handle, body, backend)
    end

    def get_header_names
      ::ComputeRuntime::ABI.fastly_http_req_header_names_get(@handle)
    end

    def get_header_values(header_name)
      ::ComputeRuntime::ABI.fastly_http_req_header_values_get(@handle, header_name)
    end

    def headers
      headers = {}
      get_header_names.each {|header_name|
        get_header_values(header_name).each {|header_value|
          headers[header_name] = header_value
        } 
      }
      headers
    end

    def set_header(name, value)
      ::ComputeRuntime::ABI.fastly_http_req_header_append(@handle, name, value)
    end

    def self.body_downstream_get
      #::ComputeRuntime::ABI.fastly_abi_init
      ::ComputeRuntime::ABI.fastly_http_req_body_downstream_get
    end
  end

  class Body
    attr_reader :handle
    def initialize(handle = nil)
      @handle = handle || ::ComputeRuntime::ABI.fastly_http_body_new
    end

    def write(data, body_write_end = 0)
      ::ComputeRuntime::ABI.fastly_http_body_write(@handle, data, body_write_end)
    end

    def read
      handle_read_chunk_size = 4 * 1024
      chunk_buffer = String.new(capacity: handle_read_chunk_size)
      buffer = ""
      loop do
        nwritten = ::ComputeRuntime::ABI.fastly_http_body_read(@handle, handle_read_chunk_size)
        break if nwritten == 0
        buffer << chunk_buffer[0, nwritten]
      end
      buffer
    end
  end

  class Event
    def self.client_ip
      ::ComputeRuntime::ABI.fastly_http_req_downstream_client_ip_addr
    end
    def self.client_geo
      ip = self.client_ip
      ::ComputeRuntime::Geo.lookup ip
    end
  end

  class Geo
    def self.lookup(ip)
      require "json/ext/parser"
      buffer_size = 1024 * 2
      buffer = String.new(capacity: buffer_size)
      ::ComputeRuntime::ABI.fastly_geo_lookup(ip, buffer, buffer_size)
      JSON::Ext::Parser.new(buffer).parse
    end
  end

  class Response
    def initialize
      @handle = ::ComputeRuntime::ABI.fastly_http_resp_new
    end

    def send_downstream(body, streaming = 0)
      ::ComputeRuntime::ABI.fastly_http_resp_send_downstream(@handle, body.handle, streaming)
    end

    def status=(code)
      ::ComputeRuntime::ABI.fastly_http_resp_status_set(@handle, code)
    end

    def set_header(name, value)
      ::ComputeRuntime::ABI.fastly_http_resp_header_append(@handle, name, value)
    end
  end

  class ObjectStore
    def initialize(name)
      @handle = ::ComputeRuntime::ABI.fastly_object_store_open(name)
    end

    def lookup(key)
      body_handle = ::ComputeRuntime::ABI.fastly_object_store_lookup(@handle, key)
      Body.new(body_handle)
    end

    def insert(key, value)
      if value.is_a?(String)
        body = Body.new
        body.write(value)
      end
      ::ComputeRuntime::ABI.fastly_object_store_insert(@handle, key, value)
    end
  end
end
