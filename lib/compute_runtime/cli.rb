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

    def pack_directory(infile, outfile, dirmaps, dirmap_strings)
      mapdir_args = dirmaps.collect_concat { |k, v| ["--dir", "#{v}::#{k}"] }
      mapdir_args += dirmap_strings.collect_concat { |str| ["--dir", str] }
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

      # TODO: support per file mapping in wasi-vfs
      Dir.mktmpdir do |dir|
        # Copy input file to the directory
        FileUtils.cp(input_file, dir)

        mapping = {
          "/exe" => dir,
        }

        pack_directory(@wasm, output_file, mapping, @opts[:mapping])
      end
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
      @mapping = []
      opts = OptionParser.new
      opts.on("-o", "--output FILE", "Output file") { |v| @output = v }
      opts.on("--[no-]stdlib", "Include stdlib in the output or not") { |v| @stdlib = v }
      opts.on("--remake", "Remake ruby.wasm") { |v| @remake = v }
      opts.on("--dir <HOST_DIR::GUEST_DIR>...", "Package a host directory into Wasm module at a guest directory") { |v| @mapping.push(v) }
      opts.parse!
      @input = args.shift
      raise "No input file" unless @input
    end

    def run(args)
      parse_args(args)
      toolchain = Toolchain.new(stdlib: @stdlib, remake: @remake, mapping: @mapping)
      toolchain.compile(@input, @output)
    end
  end
end
