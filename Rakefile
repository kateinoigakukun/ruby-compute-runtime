# frozen_string_literal: true

require "bundler/gem_tasks"
require "bundler/setup"
require "rake/tasklib"
require "ruby_wasm/rake_task"

# Un-bundle the environment so that we don't use the system bundler while
# building rubies.
ENV.replace(Bundler.original_env)

Dir.glob("tasks/**.rake").each { |f| import f }

def is_perf_profiling?
  ENV["PERF"] == "1"
end

FULL_EXTS = "bigdecimal,cgi/escape,continuation,coverage,date,dbm,digest/bubblebabble,digest,digest/md5,digest/rmd160,digest/sha1,digest/sha2,etc,fcntl,fiber,gdbm,json,json/generator,json/parser,nkf,objspace,pathname,psych,racc/cparse,rbconfig/sizeof,ripper,stringio,strscan,monitor,zlib"

BUILD_DIR = File.join(Dir.pwd, "tmp", "build")

options = {
  target: "wasm32-unknown-wasi",
  src: { name: "head", type: "github", repo: "ruby/ruby", rev: "9cd086ba4b559153864ab924723a665a4ddfb5d8", patches: [] },
  # Disable all extensions when profiling to profile only the core runtime for now.
  default_exts: is_perf_profiling? ? "" : FULL_EXTS,
  build_dir: BUILD_DIR,
}

channel = "head-wasm32-unknown-wasi-full-c@e"

build_task = RubyWasm::BuildTask.new(channel, **options) do |t|
  t.crossruby.user_exts = [
    RubyWasm::CrossRubyExtProduct.new(File.expand_path("../ext/compute_runtime", __FILE__), t.toolchain),
  ]
  t.crossruby.wasmoptflags = %w[-O3 -g]
  t.crossruby.debugflags = %w[-ggdb3]
  t.crossruby.ldflags = %w[-Xlinker -zstack-size=16777216]
end

task :cache_key do
  puts build_task.hexdigest
end

file "compile_commands.json" do
  ext_build_dir = File.join(BUILD_DIR, "wasm32-unknown-wasi", "#{channel}-ext", "compute_runtime")
  args = %W(bear make -C "#{ext_build_dir}" compute_runtime.a)
  sh(*args)
  ln_s File.join(ext_build_dir, "compile_commands.json"), "compile_commands.json"
end

wasi_vfs = build_task.wasi_vfs

RUBY_ROOT = File.join("rubies", channel)
