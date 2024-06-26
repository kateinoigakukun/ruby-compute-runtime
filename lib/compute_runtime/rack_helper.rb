# frozen_string_literal: true

require "uri"
require "rack"

module ComputeRuntime
  module RackHelper
    def rack_env(method, uri)
      Rack::MockRequest.env_for(URI(uri).path,{
        "QUERY_STRING"=>URI(uri).query,
        "REQUEST_METHOD"=> method,
        "REQUEST_PATH"=> URI(uri).path,
        "REQUEST_URI"=> uri,
        "HTTP_HOST"=>uri.split("/")[2],
        "SERVER_NAME"=> URI(uri).hostname,
        "SERVER_PORT"=> URI(uri).port,
        "PATH_INFO"=> URI(uri).path
      })
    end

    def run(rack_response)
      resp = Response.new
      resp.status = rack_response[0]
      rack_response[1].each{|key,value|
        resp.set_header(key, value)
      }
      body = Body.new
      if rack_response[2].is_a?(Array)
        body.write rack_response[2].compact.join
      elsif rack_response[2].respond_to?(:each)
        rack_response[2].each do |part|
          body.write part.to_s
        end
      elsif rack_response[2].respond_to?(:call)
        # TBD: handling streaming body
      else
        raise Error.new("Unsupported response body generated by the application")
      end
      resp.send_downstream body
    end
  end
end