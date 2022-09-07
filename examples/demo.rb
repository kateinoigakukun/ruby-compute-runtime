# Example:
# $ ./exe/ruby-compute-runtime ./examples/hello.rb -o tmp/sample.wasm
# $ viceroy ./tmp/sample.wasm

require "compute_runtime"

body = ComputeRuntime::Body.new
resp = ComputeRuntime::Response.new

begin
    geo = ComputeRuntime::Event.client_geo

    body.write <<-EOS
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="utf-8">
    <style>
    body {
        font-family: monospace;
        font-size: 1rem;
    }
    </style>
</head>
<body>
<h1>Hello from #{geo["city"]} 👋</h1>
<p>Generated at: #{Time.now}</p>
<p>Engine: Compute@Edge with #{RUBY_DESCRIPTION}</p>
<iframe width="750" height="650" src="https://maps.google.co.jp/maps?ll=#{geo["latitude"]}, #{geo["longitude"]}&amp;output=embed&amp;t=m&amp;z=17" frameborder="0" marginwidth="0" marginheight="0" scrolling="no"></iframe>
</body>
</html>
EOS
    resp.send_downstream body
rescue Exception => e
    body.write "#{e}"
    resp.send_downstream body
end
