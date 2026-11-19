# RadioModem — cross-platform amateur-radio modem

**Development status: experimental prototype. Not ready for on-air transmission.**

License: GNU GPL version 3 or later (`GPL-3.0-or-later`; see [LICENSE](LICENSE)).

This is the initial development snapshot, version **0.1.0**.

Goal: a C++ codebase for Linux, Windows and iOS, beginning with HF SSB. The project is designed around a shared portable DSP/protocol core and platform-specific audio/PTT adapters.

## Important compatibility distinction

There are two different meanings of “VARA compatibility”:

1. **Application-level compatibility:** connect to a running VARA modem through its documented TCP host interface (command/data ports). This lets a client application use VARA as an external modem; it does not reproduce VARA's waveform.
2. **Native waveform compatibility:** independently generate and decode VARA's proprietary on-air waveform. VARA is proprietary software. This starter does not claim that compatibility and does not copy its implementation. A true compatible implementation would require a lawful, technically verified specification and extensive over-the-air/interoperability testing.

The project will therefore have two backends: `NativeRadioModem` (our open protocol/waveform) and `ExternalVaraTnc` (optional adapter to a separately installed VARA instance where that software is available). On iOS, the external VARA backend is not expected to run locally; it could only be used via a reachable remote host, subject to network and licensing constraints.

## First implementation milestone

This starter implements the binary link-frame envelope:

- magic `RDM1`, version, frame type, flags, sequence number
- payload length (up to 1024 bytes)
- CRC-32C integrity check
- strict decoder validation and unit tests

This is **not** the modulation layer. The next milestones are:

1. audio test harness (WAV input/output; 48 kHz)
2. HF waveform prototype for 500 Hz and 2300 Hz channels
3. synchronization, carrier/frequency-offset estimation and soft decisions
4. FEC/interleaving and ARQ state machine
5. live audio I/O and PTT/CAT adapters
6. Qt-based UI and iOS audio integration
7. VARA TCP host-interface adapter and interoperability tests

## Build (Linux / macOS / Windows)

Requires CMake 3.20+ and a C++20 compiler.

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
ctest --test-dir build --output-on-failure
./build/radiomodem_demo
```

On a multi-configuration generator (Visual Studio), run the demo from `build/Release/`.

## Design decisions

- C++20 core with no third-party dependencies in this first milestone.
- CMake build system.
- Qt 6 is the likely UI choice because Qt supports Linux, Windows and iOS; iOS builds require Apple's Xcode toolchain and a Mac.
- Keep audio, UI and radio-control APIs outside the protocol/DSP core.
- HF-first; initial target profiles are 500 Hz and 2300 Hz SSB audio passbands. Exact symbol rates and tone/carrier plans will be selected after simulation rather than guessed.

## Regulatory note

Before transmitting, check the applicable amateur-radio rules, band plan, identification requirements and restrictions on message content. The software deliberately has no encryption feature. Start with offline WAV loopback and dummy loads; do not key a transmitter until the waveform and levels have been tested safely.

## Experimental OFDM transmitter (development snapshot)

The core now includes a first **transmit-only** BPSK-OFDM waveform generator:

- 12 kHz internal audio sample rate.
- `HF-500`: 1024-point FFT, 128-sample cyclic prefix, 40 active carriers (about 469 Hz occupied bandwidth), centered at 1500 Hz.
- `HF-2300`: 256-point FFT, 32-sample cyclic prefix, 48 active carriers (about 2250 Hz occupied bandwidth), centered at 1450 Hz.
- No external DSP dependencies; radix-2 inverse FFT.

This is a laboratory waveform prototype only. It does **not** yet include a receiver, preamble/synchronization, channel estimation, pulse shaping, FEC, ARQ, or a validated HF channel model. Do not transmit it over the air yet. The parameters are starting points for simulation and listening tests, not a finalized air interface.
