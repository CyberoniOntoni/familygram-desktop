# FamilyGram — Windows Desktop Client Build

Build [familygram-desktop](https://github.com/CyberoniOntoni/familygram-desktop) for self-hosted [Testgram](https://github.com/CyberoniOntoni/testgram) servers.

FamilyGram prompts for your server address at first launch — no hardcoded IP or domain in the client.

## Example server values

Replace with your own deployment (see [Testgram deployment guide](https://github.com/CyberoniOntoni/testgram/blob/dev/deploy/DEPLOYMENT-example.md)):

| Setting | Example |
|---------|---------|
| Server host | `203.0.113.50` or `tg.acmechat.example` |
| MTProto ports | `20443`, `20543`, `20643` (media DC e.g. `20644`) |
| STUN/TURN | `5348` + relay range as configured on your server |
| Login codes | Verification bot on Telegram, or `App__FixedVerifyCode` for testing |

Use the **public** IP or domain reachable by clients — not a LAN-only address, unless every client is on that network.

---

## Prerequisites (Windows)

1. **Visual Studio 2022 or 2026** with Desktop development + Windows 10/11 SDK.
2. **Python 3.10** on `PATH`.
3. **Git** on `PATH`.
4. **Telegram API credentials** — `api_id` and `api_hash` from [my.telegram.org](https://my.telegram.org/apps).

Create a build folder, e.g. `D:\TBuild`, with subfolders `ThirdParty` and `Libraries`.

---

## Build steps

### 1. Open the VS environment

```bat
%comspec% /k "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
```

### 2. Clone and prepare dependencies

```bat
git clone --recursive https://github.com/CyberoniOntoni/familygram-desktop.git
cd familygram-desktop
git submodule update --init --recursive
Telegram\build\prepare\win.bat
```

### 3. Configure with your API credentials

```bat
cd Telegram
configure.bat x64 -D TDESKTOP_API_ID=YOUR_API_ID -D TDESKTOP_API_HASH=YOUR_API_HASH
```

### 4. Build in Visual Studio

1. Open `out\Telegram.slnx` in Visual Studio.
2. Configuration: **Release**.
3. Build project **Telegram**.

Output: `out\Release\Familygram.exe`

---

## First login

1. Confirm your Testgram gateway listens on MTProto ports (`docker compose logs gateway-server`).
2. Run `Familygram.exe` and enter your server host (e.g. `tg.acmechat.example`).
3. Sign in with your phone number and verification code.

---

## Related

- Server deploy: [testgram/deploy/DEPLOYMENT-example.md](https://github.com/CyberoniOntoni/testgram/blob/dev/deploy/DEPLOYMENT-example.md)
- Upstream Windows build details: [building-win.md](building-win.md)