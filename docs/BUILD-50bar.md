# 50bar — Windows Desktop Client Build

Build [testgram-tdesktop](https://github.com/CyberoniOntoni/testgram-tdesktop) against the **50bar** Testgram server.

## Server values (already patched on `dev`)

| Setting | Value |
|---------|-------|
| Public WAN IP | `154.211.187.131` |
| MTProto ports | `20443(TCP)`, `20543(TCP)`, `20643(TCP)`, `20644(TCP)` media |
| STUN/TURN | `5348(TCP&UDP)` + relay `49152-49172(UDP)` |
| Login codes | `@fiftybar_login_bot` on Telegram (link phone via `/start`) |
| Brand | `50bar` |

**Do not** use the LAN IP (`192.168.11.79`) or domain in the client — MTProto must hit the public IP.

### Source file to verify

`Telegram/SourceFiles/mtproto/mtproto_dc_options.cpp` — built-in DC bootstrap endpoints:

```cpp
#define IpV4 "154.211.187.131"
#define IpV4Port 20443   // DC1
#define IpV4Port2 20543  // DC2
#define IpV4Port3 20643  // DC3
```

After the first connection the server also pushes full `DcOptions` (including media DC on `20644`).

---

## Prerequisites (Windows)

1. **Visual Studio 2022 or 2026** with Desktop development + Windows 10/11 SDK (`10.0.26100.0` per upstream docs).
2. **Python 3.10** on `PATH`.
3. **Git** on `PATH`.
4. **Telegram API credentials** — `api_id` and `api_hash` from [my.telegram.org](https://my.telegram.org/apps).  
   Required at configure time; test-only defaults in `config.h` will not work against a private server.

Create a build folder, e.g. `D:\TBuild`, with subfolders `ThirdParty` and `Libraries`.

---

## Build steps

### 1. Open the correct VS environment

For **64-bit** (recommended):

```bat
%comspec% /k "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
```

VS 2026 path uses `...\18\Community\...` and may need `-vcvars_ver=14.44` (see [building-win.md](building-win.md)).

### 2. Clone and prepare dependencies

In `D:\TBuild` (or your *BuildPath*):

```bat
git clone --recursive --branch dev https://github.com/CyberoniOntoni/testgram-tdesktop.git tdesktop
cd tdesktop
git submodule update --init --recursive
Telegram\build\prepare\win.bat
```

`prepare\win.bat` downloads Qt and other libraries — first run takes a long time and needs network.

### 3. Configure with your API credentials

```bat
cd Telegram
configure.bat x64 -D TDESKTOP_API_ID=YOUR_API_ID -D TDESKTOP_API_HASH=YOUR_API_HASH
```

Replace `YOUR_API_ID` / `YOUR_API_HASH` with values from my.telegram.org.

### 4. Build in Visual Studio

1. Open `D:\TBuild\tdesktop\out\Telegram.slnx` in Visual Studio.
2. Set configuration to **Release** (or Debug for testing).
3. Build project **Telegram**.

Output:

- Debug: `out\Debug\Telegram.exe`
- Release: `out\Release\Telegram.exe`

---

## After install — first login

1. On the server, confirm gateway listens: `docker compose logs gateway-server | grep 20443`
2. Router forwards: `20443-20644(TCP)`, `5348(TCP&UDP)`, `49152-49172(UDP)` → `192.168.11.79`
3. Open **@fiftybar_login_bot** → `/start` → link your phone number
4. Run your built `Telegram.exe` → enter that phone → code arrives in the bot chat

---

## Customizing for another server

Edit only `mtproto_dc_options.cpp` (or revert to `YOUR_SERVER_IP` placeholders), then re-run `configure.bat` and rebuild.

Updater / about URLs still point at `testgramdesktop.xie.su` in a few files — they do not affect MTProto login. Change separately if you host your own update channel.

---

## Related

- Server deploy: [testgram/deploy/DEPLOYMENT-50bar.md](https://github.com/CyberoniOntoni/testgram/blob/dev/deploy/DEPLOYMENT-50bar.md)
- Upstream Windows build details: [building-win.md](building-win.md)