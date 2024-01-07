require "optparse"
require "ruby_wasm"
require "ruby_wasm/cli"

module ComputeRuntime
  class Toolchain
    def initialize(opts)
      @opts = opts
    end

    def run_rbwasm(*args)
      RubyWasm::CLI.new(stdout: $stdout, stderr: $stderr).run(args)
    end

    def pack_directory(infile, outfile, dirmaps)
      mapdir_args = dirmaps.collect_concat { |k, v| ["--mapdir", "#{k}::#{v}"] }
      pack_args = ["pack", infile, "-o", outfile] + mapdir_args
      run_rbwasm(*pack_args)
    end

    def preset_args(infile, outfile, *args)
      Kernel.system("wasi-preset-args", "-o", outfile, infile, "--", *args)
    end

    def compile(input_file, output_file)
      # Build ruby.wasm
      build_args = ["build", "-o", output_file]
      case @opts[:stdlib]
      when true
        build_args << "--stdlib"
      when false, nil
        build_args << "--no-stdlib"
      end
      build_args << "--remake" if @opts[:remake]
      run_rbwasm(*build_args)
      @wasm = output_file

      mapping = {
        # TODO: support per file mapping in wasi-vfs
        "/exe" => File.dirname(input_file),
      }

      pack_directory(@wasm, output_file, mapping)
      preset_args(output_file, output_file, "--disable=gems", "/exe/#{File.basename(input_file)}")
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
      opts.on("--remake", "Remake ruby.wasm") { |v| @remake = v }
      opts.parse!
      @input = args.shift
      raise "No input file" unless @input
    end

    def run(args)
      parse_args(args)
      toolchain = Toolchain.new(stdlib: @stdlib, remake: @remake)
      toolchain.compile(@input, @output)
    end
  end
end
