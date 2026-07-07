import pathlib
import re
import sys

def main() -> int:
    path = pathlib.Path('build_libvpx_win.sh')
    if not path.is_file():
        print(f'[ERROR] {path} not found', file=sys.stderr)
        return 1

    text = path.read_text(encoding='utf-8')
    updated = text

    if "find . -name '*.vcxproj'" not in updated:
        updated = re.sub(
            r'--size-limit=4096x4096\r?\n\r?\nmake',
            (
                '--size-limit=4096x4096\n\n'
                "find . -name '*.vcxproj' -exec sed -i 's/<PlatformToolset>v143<\\/PlatformToolset>/<PlatformToolset>v145<\\/PlatformToolset>/g' {} +\n"
                "sed -i 's/-p:Platform=\"\\$platform\"/-p:Platform=\"\\$platform\" -p:PlatformToolset=v145/g' build/make/gen_msvs_sln.sh\n"
                "find . -name '*.vcxproj' -exec sed -i 's/<MultiProcessorCompilation>true<\\/MultiProcessorCompilation>/<MultiProcessorCompilation>false<\\/MultiProcessorCompilation>/g' {} +\n\n"
                'make --no-print-directory target=solution vpx.sln\n'
                'cmd.exe //c "msbuild.exe vpx.sln /m:1 /t:Build /p:Configuration=Release /p:Platform=x64 /p:PlatformToolset=v145"\n'
                'make --no-print-directory target=libs .install-libs\n'
                'make --no-print-directory target=solution .install-projects'
            ),
            updated,
            count=1,
        )
    else:
        if 'MultiProcessorCompilation>false' not in updated:
            needle = "sed -i 's/-p:Platform=\"\\$platform\"/-p:Platform=\"\\$platform\" -p:PlatformToolset=v145/g' build/make/gen_msvs_sln.sh\n"
            if needle in updated:
                updated = updated.replace(
                    needle,
                    needle
                    + "find . -name '*.vcxproj' -exec sed -i 's/<MultiProcessorCompilation>true<\\/MultiProcessorCompilation>/<MultiProcessorCompilation>false<\\/MultiProcessorCompilation>/g' {} +\n",
                )

        release_build = (
            'make --no-print-directory target=solution vpx.sln\n'
            'cmd.exe //c "msbuild.exe vpx.sln /m:1 /t:Build /p:Configuration=Release /p:Platform=x64 /p:PlatformToolset=v145"\n'
            'make --no-print-directory target=libs .install-libs\n'
            'make --no-print-directory target=solution .install-projects'
        )
        updated = re.sub(
            r'make -j\$NUMBER_OF_PROCESSORS\r?\nmake -j\$NUMBER_OF_PROCESSORS install',
            release_build,
            updated,
        )
        updated = updated.replace('make -j1\nmake -j1 install', release_build)
        updated = re.sub(
            r'make --no-print-directory target=solution Release_x64\r?\n',
            'cmd.exe //c "msbuild.exe vpx.sln /m:1 /t:Build /p:Configuration=Release /p:Platform=x64 /p:PlatformToolset=v145"\n',
            updated,
        )
        updated = re.sub(
            r"sed -i 's/-m -t:Build/-m:1 -t:Build/g' build/make/gen_msvs_sln.sh\n\n",
            '',
            updated,
        )

    if updated != text:
        path.write_text(updated, encoding='utf-8')
    return 0

if __name__ == '__main__':
    raise SystemExit(main())