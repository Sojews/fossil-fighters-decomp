#!/usr/bin/env python3
"""Downloads the pinned build tools into tools/bin and tools/mwccarm.

Usage: python3 tools/download_tools.py
"""
import io
import platform
import stat
import sys
import urllib.request
import zipfile
from pathlib import Path

DSD_VERSION = "v0.12.1"
WIBO_VERSION = "1.2.0"
OBJDIFF_VERSION = "v3.8.2"
MWCCARM_URL = "http://decomp.aetias.com/files/mwccarm.zip"

root = Path(__file__).resolve().parent.parent
bin_dir = root / "tools" / "bin"


def host() -> tuple[str, str, str]:
    system = {"Darwin": "macos", "Linux": "linux", "Windows": "windows"}[platform.system()]
    machine = platform.machine().lower()
    machine = {"amd64": "x86_64", "aarch64": "arm64"}.get(machine, machine)
    exe = ".exe" if system == "windows" else ""
    return system, machine, exe


def fetch(url: str) -> bytes:
    print(f"Downloading {url}")
    with urllib.request.urlopen(url) as r:
        return r.read()


def save_exe(path: Path, data: bytes) -> None:
    path.write_bytes(data)
    path.chmod(path.stat().st_mode | stat.S_IEXEC)


def main() -> None:
    system, machine, exe = host()
    bin_dir.mkdir(parents=True, exist_ok=True)

    dsd_arch = "arm64" if machine == "arm64" and system == "macos" else "x86_64"
    save_exe(bin_dir / f"dsd{exe}", fetch(
        f"https://github.com/AetiasHax/ds-decomp/releases/download/{DSD_VERSION}/dsd-{system}-{dsd_arch}{exe}"))

    objdiff_arch = {"arm64": "arm64" if system != "linux" else "aarch64"}.get(machine, "x86_64")
    save_exe(bin_dir / f"objdiff-cli{exe}", fetch(
        f"https://github.com/encounter/objdiff/releases/download/{OBJDIFF_VERSION}/objdiff-cli-{system}-{objdiff_arch}{exe}"))

    # wibo runs the Windows-only Metrowerks tools on macOS and Linux.
    if system != "windows":
        wibo_asset = "wibo-macos" if system == "macos" else "wibo-x86_64"
        save_exe(bin_dir / "wibo", fetch(
            f"https://github.com/decompals/wibo/releases/download/{WIBO_VERSION}/{wibo_asset}"))

    if not (root / "tools" / "mwccarm").is_dir():
        zipfile.ZipFile(io.BytesIO(fetch(MWCCARM_URL))).extractall(root / "tools")

    print("Done.")


if __name__ == "__main__":
    sys.exit(main())
