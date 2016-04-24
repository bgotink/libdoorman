# Doorman

This project contains C++ code to handle portable chimes branded "doorman" on
Raspberry Pi devices.
We can only test this project on our doorman WFDC841 receiver and doorman 46011SL
transmitter.

## NPM

This project is published as NPM package.

### Requirements

The NPM package requires at least Node 4 LTS. It also requires [wiringPi][wpi]
to be installed on your Raspberry Pi.

### Installation

```
npm install -g doorman-chime
```

### Usage

```
doorman-chime --help
```

## C++

Running `make` will build three binaries:

### `bin/parser-test`

This is merely a test binary where you can input bits of a signal yourself and
test the behaviour of the parser. You'll probably not need this.

### `bin/doorman`

This binary allows for reading and writing doorman signals. You'll need sudo
when running this.

```
bin/doorman <pin> write <channel: 1-16> <sound: 1-3>
```

Writes the signal to play the given sound on the given channel over the given
GPIO pin.

```
bin/doorman <pin> read [file]
bin/doorman <pin> read-human [file]
```

Reads the signal over the given GPIO pin. If a file is given, it writes the data
to that file, otherwise it outputs to standard out. Command `read` will output
the raw data, `read-human` will try to find the doorman signals.  
The program will listen for a couple of seconds before outputting the signals.

### `bin/listen`

This binary allows for continuous listening for signals on a pin. You'll need
sudo when running this.

```
bin/listen <pin>
```

Listens for signals on the given GPIO pin, outputs a single line with the
significant bits of the signal (see PROTOCOL.md) per signal received. Note that
doorman transmitters tend to send their signal more than once.

## License

```
Copyright (c) 2015 Bram Vercruysse <https://github.com/BramV> and Bram Gotink <https://github.com/bgotink>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
```

[wpi]: http://wiringpi.com/
