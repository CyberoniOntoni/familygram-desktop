# Intentionally a no-op.
#
# Earlier versions of this script rewrote desktop-app/patches/build_libvpx_win.sh
# into a custom msbuild flow and left broken remnants of `make -j... install`.
# Upstream's script is already the correct path for GH Actions:
#   configure --target=$TOOLCHAIN && make && make install
# which places vpxmt.lib under local/lib/<arch>/.

def main() -> int:
    return 0


if __name__ == '__main__':
    raise SystemExit(main())
