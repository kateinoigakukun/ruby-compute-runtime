require "optparse"

module ComputeRuntime
  class Toolchain
    def initialize
      @wasm = "ruby.wasm"
      raise "ruby.wasm not found" unless File.exist?(@wasm)
      raise "wasi-vfs not found" unless system("which", "wasi-vfs")
    end

    def pack_directory(infile, outfile, dirmaps)
      mapdir_args = dirmaps.collect_concat { |k, v| ["--mapdir", "#{k}::#{v}"] }
      Kernel.system("wasi-vfs", *(["pack", infile, "-o", outfile] + mapdir_args))
    end

    def preset_args(infile, outfile, *args)
      Kernel.system("wasi-preset-args", "-o", outfile, infile, "--", "--disable=gems", "/examples/demo.rb")
    end

    def compile(input_file, output_file)
      pack_directory(@wasm, output_file, {"/examples" => "./examples", "/usr" => "./tmp/head-wasm32-unknown-wasi-minimal-c@e/usr"})
      preset_args(output_file, output_file, "--disable=gems", "/examples/demo.rb")
    end
  end

  class CLI
    def parse_args(args)
      args = args.dup
      opts = OptionParser.new
      opts.on("-o", "--output FILE", "Output file") { |v| @output = v }
      opts.parse!
      @input = args.shift
      raise "No input file" unless @input
    end

    def run(args)
      parse_args(args)
      toolchain = Toolchain.new
      toolchain.compile(@input, @output)
    end
  end
end
