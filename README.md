# ComputeRuntime

Ruby runtime for Fastly Compute@Edge

A Ruby Compute Runtime for Fastly's Compute@Edge platform provides the environment Ruby is executed.

Please note that This is still PoC stage for demo

## Usage

You need to install [wasi-vfs](https://github.com/kateinoigakukun/wasi-vfs/releases/tag/v0.2.0) before starting this project.

```console
$ bundle install
$ rake setup
$ ./exe/ruby-compute-runtime ./examples/demo.rb -o tmp/sample.wasm

$ # Try on local
$ viceroy ./tmp/sample.wasm

$ # Deploy to Compute@Edge
$ cd example
$ fastly compute init
$ fastly compute pack -w ./../tmp/sample.wasm
$ fastly compute deploy
```

## Contributing

Bug reports and pull requests are welcome on GitHub at [https://github.com/kateinoigakukun/ruby-compute-runtime](https://github.com/kateinoigakukun/ruby-compute-runtime). This project is intended to be a safe, welcoming space for collaboration, and contributors are expected to adhere to the [code of conduct](https://github.com/kateinoigakukun/compute_runtime/blob/main/CODE_OF_CONDUCT.md).

## License

The gem is available as open source under the terms of the [MIT License](https://opensource.org/licenses/MIT).

## Code of Conduct

Everyone interacting in the ComputeRuntime project's codebases, issue trackers, chat rooms and mailing lists is expected to follow the [code of conduct](https://github.com/kateinoigakukun/compute_runtime/blob/main/CODE_OF_CONDUCT.md).
