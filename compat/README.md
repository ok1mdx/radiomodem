# VARA compatibility adapter (planned)

The adapter will speak the public host-side TCP command/data interface of a separately running VARA modem. It will not implement or claim to implement the VARA RF waveform.

`NativeRadioModem` will implement the project's own open waveform/protocol. `ExternalVaraTnc` will proxy data to a local or remote VARA host interface. The exact command set and port behavior must be validated against current official VARA documentation before implementation.
