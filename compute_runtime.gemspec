# frozen_string_literal: true

require_relative "lib/compute_runtime/version"

Gem::Specification.new do |spec|
  spec.name = "compute_runtime"
  spec.version = ComputeRuntime::VERSION
  spec.authors = ["Yuta Saito"]
  spec.email = ["kateinoigakukun@gmail.com"]

  spec.summary = "Ruby runtime for Fastly Compute@Edge"
  spec.description = <<~DESC
    The Ruby runtime for Fastly's Compute@Edge platform provides the environment
    to run Ruby code on Fastly's edge servers.
  DESC

  spec.homepage = "https://github.com/kateinoigakukun/ruby-compute-runtime"
  spec.license = "MIT"
  spec.required_ruby_version = ">= 2.6.0"

  spec.metadata["homepage_uri"] = spec.homepage
  spec.metadata["source_code_uri"] = "https://github.com/kateinoigakukun/ruby-compute-runtime"

  # Specify which files should be added to the gem when it is released.
  # The `git ls-files -z` loads the files in the RubyGem that have been added into git.
  spec.files = Dir.chdir(__dir__) do
    `git ls-files -z`.split("\x0").reject do |f|
      (f == __FILE__) || f.match(%r{\A(?:(?:bin|test|spec|features)/|\.(?:git|travis|circleci)|appveyor)})
    end
  end
  spec.bindir = "exe"
  spec.executables = spec.files.grep(%r{\Aexe/}) { |f| File.basename(f) }
  spec.require_paths = ["lib"]
  spec.extensions = ["ext/compute_runtime/extconf.rb"]

  # TODO: Add dependency here after releasing a new version of ruby_wasm
  # spec.add_runtime_dependency "ruby_wasm", "2.5.0.pre.1"
end
