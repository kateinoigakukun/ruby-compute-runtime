require "optparse"

module ComputeRuntime
  class Toolchain
    def initialize(opts)
      @opts = opts
      @ruby_root = "rubies/head-wasm32-unknown-wasi-full-c@e"
      @wasm = "#{@ruby_root}/usr/local/bin/ruby"
      raise "ruby.wasm not found" unless File.exist?(@wasm)
      check_executable("wasi-vfs")
    end

    def pack_directory(infile, outfile, dirmaps)
      mapdir_args = dirmaps.collect_concat { |k, v| ["--mapdir", "#{k}::#{v}"] }
      Kernel.system("wasi-vfs", *(["pack", infile, "-o", outfile] + mapdir_args))
    end

    def preset_args(infile, outfile, *args)
      Kernel.system("wasi-preset-args", "-o", outfile, infile, "--", *args)
    end

    def compile(input_file, output_file)
      mapping = {
        # TODO: support per file mapping in wasi-vfs
        "/exe" => File.dirname(input_file),
        "/lib" => "./lib",
      }
      if @opts[:stdlib]
        mapping["/usr"] = "rubies/head-wasm32-unknown-wasi-full-c@e/usr"
      end

      pack_directory(@wasm, output_file, mapping)
      preset_args(output_file, output_file, "--disable=gems", "-I/lib", "/exe/#{File.basename(input_file)}")
    end

    def check_executable(command)
      (ENV["PATH"] || "").split(File::PATH_SEPARATOR).each do |path_dir|
        bin_path = File.join(path_dir, command)
        return bin_path if File.executable?(bin_path)
      end
      raise "missing executable: #{command}"
    end
  end

  class CLI
    def parse_args(args)
      args = args.dup
      opts = OptionParser.new
      opts.on("-o", "--output FILE", "Output file") { |v| @output = v }
      opts.on("--[no-]stdlib", "Include stdlib in the output or not") { |v| @stdlib = v }
      opts.parse!
      @input = args.shift
      raise "No input file" unless @input
    end

    def run(args)
      parse_args(args)
      toolchain = Toolchain.new(stdlib: @stdlib)
      toolchain.compile(@input, @output)
    end
  end
end
