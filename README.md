# libidk

## Remote messages

Link `libidk_remote` for `idk::RemoteRxTxer` (SDL3 and SDL3_net). Configure
`IDK_BUILD_REMOTE=OFF` to build libidk without remote messaging dependencies.
The remote target is separate from the main library; existing remote users
must add it to their link dependencies.

`RemoteRxTxer(nullptr, port)` listens on a server port. A hostname constructor
uses an ephemeral local port and sends to the specified remote port. Check
`isOpen()` and `error()` after construction, or use the default constructor and
the checked `open(host, remotePort, localPort, bindHost)` method. Name resolution
can block up to one second per address. Polling is nonblocking.

`receive(message)` distinguishes `Empty`, `Message`, `Invalid` and `Error`.
`recvMsg()` is the simpler compatibility wrapper, returning null for all three
non-message outcomes. Receive pointers are borrowed until the next receive or
close; sending does not overwrite them. A valid packet supplies its actual
sender address and port through `sender()`. Copy that `RemoteEndpoint` to retain
an address reference and pass it to `sendTo()` to address a particular peer.
`sendMsg()` uses the configured remote, or the latest valid sender for a listener.
Unsolicited messages never change the configured remote. These classes are not
thread-safe; operate each socket from one thread or synchronize externally.

Packets have a 32-byte explicit big-endian envelope and at most 1024 payload
bytes. The marker includes a wire version; it is not authentication. Invalid
lengths, oversized packets and unknown versions are rejected before copying.
This replaces the old disabled native-struct wire format. Payloads remain
application-defined; `message/Wire.hpp` provides checked integer/float codecs.
Session management, authentication and delivery guarantees belong above this
UDP transport. The engine's `idkengine_network` target supplies bounded sessions
and server-authoritative entity snapshots.
