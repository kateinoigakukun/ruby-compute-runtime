# frozen_string_literal: true

require "bundler/gem_tasks"
$LOAD_PATH << File.join(File.dirname(__FILE__), "vendor", "deps", "ruby.wasm", "lib")

require "rake/tasklib"
require "ruby_wasm/build_system"
require "ruby_wasm/rake_task"

Dir.glob("tasks/**.rake").each { |f| import f }

FULL_EXTS = "bigdecimal,cgi/escape,continuation,coverage,date,dbm,digest/bubblebabble,digest,digest/md5,digest/rmd160,digest/sha1,digest/sha2,etc,fcntl,fiber,gdbm,json,json/generator,json/parser,nkf,objspace,pathname,psych,racc/cparse,rbconfig/sizeof,ripper,stringio,strscan,monitor,zlib"

LIB_ROOT = File.expand_path("../vendor/deps/ruby.wasm", __FILE__)
BUILD_DIR = File.join(Dir.pwd, "tmp", "build")

options = {
  target: "wasm32-unknown-wasi",
  src: { name: "head", type: "github", repo: "ruby/ruby", rev: "v3_2_0_preview2 ", patches: [] },
  default_exts: FULL_EXTS,
  build_dir: BUILD_DIR,
}

channel = "head-wasm32-unknown-wasi-full-c@e"

build_task = RubyWasm::BuildTask.new(channel, **options) do |t|
  t.crossruby.user_exts = [
    RubyWasm::CrossRubyExtProduct.new(File.expand_path("../ext/compute_runtime", __FILE__), t.toolchain),
  ]
  t.crossruby.wasmoptflags = %w["-O2"]
  t.crossruby.cppflags = %w["-DOPT_THREADED_CODE=2"]
end

task :cache_key do
  puts build_task.hexdigest
end

wasi_vfs = build_task.wasi_vfs

RUBY_ROOT = File.join("rubies", channel)
