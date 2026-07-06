# FamilyGram

Universal Windows desktop client for self-hosted [Testgram](https://github.com/CyberoniOntoni/testgram) servers. FamilyGram is based on [Telegram Desktop](https://github.com/telegramdesktop/tdesktop) and prompts for your server address instead of hardcoding endpoints.

## Features

- **First-run server setup** — enter your server's IP address or domain on launch
- **Dynamic DC discovery** — connects via bootstrap ports, calls `help.getConfig`, and stores the full DC list from your server
- **Change server** — switch servers anytime under **Settings → Advanced → FamilyGram → Change server**
- **Persistent host** — server address is saved in `tdata/familygram_server`

## How it works

On first launch (or after changing the server), FamilyGram:

1. Connects to your host on bootstrap MTProto ports `20443`, `20543`, and `20643`
2. Fetches DC options via `help.getConfig`
3. Saves the host and applies the returned DC configuration
4. Continues to the normal sign-in flow (QR or phone)

Changing the server signs you out on the device and requires signing in again.

## Sign in

1. Deploy a [Testgram](https://github.com/CyberoniOntoni/testgram) server and configure its login bot
2. Link your phone number with the bot (typically `/start` in the bot chat)
3. Launch FamilyGram and enter your server address
4. Sign in with the linked phone number and enter the code from the bot chat

Use the **public** IP or domain that clients can reach over the internet — not a LAN-only address, unless every client is on that network.

## Server requirements

FamilyGram works with any Testgram deployment that exposes standard MTProto ports. Typical setup:

| Setting | Example |
|---------|---------|
| MTProto ports | `20443`, `20543`, `20643` (and media DC, e.g. `20644`) |
| STUN/TURN | `5348` + relay range as configured on your server |
| Login bot | Server-specific Telegram bot for phone linking and codes |

No server-side changes are required beyond a normal Testgram deployment — the client uses the existing `help.getConfig` response for DC options.

## Build (Windows)

FamilyGram targets **Windows 64-bit**. Full upstream instructions are in [docs/building-win.md](docs/building-win.md).

### Prerequisites

- Visual Studio 2022 or 2026 with Desktop development and Windows 10/11 SDK
- Python 3.10 and Git on `PATH`
- **api_id** and **api_hash** from [my.telegram.org](https://my.telegram.org/apps) — see [docs/api_credentials.md](docs/api_credentials.md)

Create a build folder (e.g. `D:\TBuild`) with `ThirdParty` and `Libraries` subfolders.

### Quick build

```bat
%comspec% /k "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"

git clone --recursive https://github.com/CyberoniOntoni/familygram-desktop.git
cd familygram-desktop
Telegram\build\prepare\win.bat

cd Telegram
configure.bat x64 -D TDESKTOP_API_ID=YOUR_API_ID -D TDESKTOP_API_HASH=YOUR_API_HASH

cmake --build ..\out --config Release --parallel
```

The built executable is `out\Release\Telegram.exe` (product name: **FamilyGram**).

`prepare\win.bat` downloads Qt and other dependencies — the first run takes a long time and requires network access.

## Project layout

| Path | Purpose |
|------|---------|
| `Telegram/SourceFiles/core/familygram_server.*` | Host validation, bootstrap ports, DC option application |
| `Telegram/SourceFiles/intro/intro_server.*` | First-run server connection step |
| `Telegram/SourceFiles/settings/sections/settings_advanced.cpp` | Settings UI for changing server |
| `Telegram/SourceFiles/storage/localstorage.*` | Read/write `familygram_server` host file |
| `Telegram/SourceFiles/core/version.h` | App name and version |

## License

Source code is published under GPLv3 with OpenSSL exception. See [LICENSE](LICENSE) and [LEGAL](LEGAL).

## Credits

- [Telegram Desktop](https://github.com/telegramdesktop/tdesktop) — upstream client
- [Testgram](https://github.com/CyberoniOntoni/testgram) — self-hosted Telegram server