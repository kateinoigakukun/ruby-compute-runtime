# ComputeRuntime

Ruby runtime for Fastly Compute@Edge

A Ruby Compute Runtime for Fastly's Compute@Edge platform provides the environment Ruby is executed.

Please note that This is still PoC stage for demo

## Usage

You need to install [wasi-preset-args](https://github.com/kateinoigakukun/wasi-preset-args/) before starting this project.

```console
$ cd examples
$ bundle install
$ bundle exec ruby-compute-runtime demo.rb -o sample.wasm

$ # Try on local
$ viceroy ./sample.wasm

$ # Deploy to Compute@Edge
$ fastly compute init
$ fastly compute pack -w ./sample.wasm
$ fastly compute deploy
```

## Development

If you update sources under `ext/compute_runtime`, please run `ruby-compute-runtime` command with `--remake` option as follows:

```console
$ bundle exec ruby-compute-runtime demo.rb -o sample.wasm --remake
```

Otherwise, it re-uses old version of extension library.

## Contributing

Bug reports and pull requests are welcome on GitHub at [https://github.com/kateinoigakukun/ruby-compute-runtime](https://github.com/kateinoigakukun/ruby-compute-runtime). This project is intended to be a safe, welcoming space for collaboration, and contributors are expected to adhere to the [code of conduct](https://github.com/kateinoigakukun/compute_runtime/blob/main/CODE_OF_CONDUCT.md).

## License

The gem is available as open source under the terms of the [MIT License](https://opensource.org/licenses/MIT).

## Code of Conduct

Everyone interacting in the ComputeRuntime project's codebases, issue trackers, chat rooms and mailing lists is expected to follow the [code of conduct](https://github.com/kateinoigakukun/compute_runtime/blob/main/CODE_OF_CONDUCT.md).
