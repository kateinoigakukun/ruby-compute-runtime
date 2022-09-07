# Example:
# $ ./exe/ruby-compute-runtime ./examples/hello.rb -o tmp/sample.wasm
# $ viceroy ./tmp/sample.wasm

require "compute_runtime"

body = ComputeRuntime::Body.new
resp = ComputeRuntime::Response.new

begin
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
<h1>Hello RubyKaigi 👋</h1>
<p>Generated at: #{Time.now}</p>
<p>Engine: Compute@Edge with #{RUBY_DESCRIPTION}</p>
</body>
</html>
EOS
    resp.send_downstream body
rescue Exception => e
    body.write "#{e}"
    resp.send_downstream body
end
