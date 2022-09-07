# frozen_string_literal: true

require "bundler/gem_tasks"
require "rake/testtask"

Rake::TestTask.new(:test) do |t|
  t.libs << "test"
  t.libs << "lib"
  t.test_files = FileList["test/**/test_*.rb"]
end

task :setup do
  `mkdir -p tmp`
  `curl -L -o tmp/ruby.tar.gz https://github.com/kateinoigakukun/ruby.wasm/releases/download/2022-09-07-c%40e-b/ruby-head-wasm32-unknown-wasi-full-c@e.tar.gz`
  `tar xfz tmp/ruby.tar.gz -C tmp`
end

require "standard/rake"

require "rake/extensiontask"

task build: :compile

Rake::ExtensionTask.new("compute_runtime") do |ext|
  ext.lib_dir = "lib/compute_runtime"
end

task default: %i[clobber compile test standard]
