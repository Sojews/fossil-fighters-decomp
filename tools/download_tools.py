#!/usr/bin/env python3
"""Downloads the pinned build tools into tools/bin and tools/mwccarm.

Every download is over HTTPS and checked against a pinned SHA-256 before it is written or run.
To bump a tool, update its version and hashes (GitHub shows them as each release asset's digest).

Usage: python3 tools/download_tools.py
"""
import hashlib
import io
import platform
import stat
import sys
import urllib.request
import zipfile
from pathlib import Path

DSD = ("v0.12.1", {
    "dsd-macos-arm64": "1bbb57fe66a37123a7e9a64459b174956ea69ea103ad3dfd2766f18ca63057c7",
    "dsd-macos-x86_64": "21adcef119d6e9b6bb853b0e5cc626bc4c086d28d45abc9a91b715c78631a044",
    "dsd-linux-x86_64": "256edde804dd8e5d7bc4c0e4187f85244ee4cd1f42bb427502e01fda6af43ceb",
    "dsd-windows-x86_64.exe": "5917df26bffc05d4752276a2e0b1edd21bd6e920aabe1992be8a50efc9d10932",
})
OBJDIFF = ("v3.8.2", {
    "objdiff-cli-macos-arm64": "767c67e13b7b5e6ca545e7d2235e47c7bceac07d47a52776f74a52914cb4085a",
    "objdiff-cli-macos-x86_64": "2cfe0f46fc84cef7d96813f100a0f01fbf99ac7e423b7fda1d314aada4d5ff72",
    "objdiff-cli-linux-x86_64": "e5445089a6f707e30cb5e02992451b604619cedfd2660ee2373a86bfefd9e718",
    "objdiff-cli-linux-aarch64": "087516e73e1b3223b39974f00bc2810fea5c3dc2c93a8e1f06442ec68acaacb4",
    "objdiff-cli-windows-x86_64.exe": "36d229ac6ce74a26b47f42cf86ea8e808d8cd834a54aa62d7ed3f47b765d971b",
    "objdiff-cli-windows-arm64.exe": "c4fed8851366859e69499bfd1fed7f026169602833d3032708329809f307f7d0",
})
WIBO = ("1.2.0", {
    "wibo-macos": "2b3000ef6a7a490c24ccd71967735ae0005e218922e51806cca1b8d77fd3cf7c",
    "wibo-x86_64": "13f86a2d618f0dbe67179d349625345eabf9b46450295cb4c904e49f6aff85af",
})
MWCCARM_URL = "https://decomp.aetias.com/files/mwccarm.zip"
MWCCARM_SHA256 = "6f7b16098d2b1bc9b45b278f6e6e24327c4da9b3ebf7924be1afd4a721d83976"

root = Path(__file__).resolve().parent.parent
bin_dir = root / "tools" / "bin"


def host() -> tuple[str, str, str]:
    system = {"Darwin": "macos", "Linux": "linux", "Windows": "windows"}[platform.system()]
    machine = platform.machine().lower()
    machine = {"amd64": "x86_64", "aarch64": "arm64"}.get(machine, machine)
    exe = ".exe" if system == "windows" else ""
    return system, machine, exe


def fetch(url: str, sha256: str) -> bytes:
    if not url.startswith("https://"):
        sys.exit(f"Refusing non-HTTPS download: {url}")
    print(f"Downloading {url}")
    with urllib.request.urlopen(url) as r:
        data = r.read()
    actual = hashlib.sha256(data).hexdigest()
    if actual != sha256:
        sys.exit(f"SHA-256 mismatch for {url}\n  expected {sha256}\n  got      {actual}\nNothing was written.")
    return data


def github_tool(repo: str, pinned: tuple[str, dict[str, str]], asset: str) -> bytes:
    tag, hashes = pinned
    if asset not in hashes:
        sys.exit(f"No pinned build of {asset} for this platform; install it manually into tools/bin.")
    return fetch(f"https://github.com/{repo}/releases/download/{tag}/{asset}", hashes[asset])


def save_exe(path: Path, data: bytes) -> None:
    path.write_bytes(data)
    path.chmod(path.stat().st_mode | stat.S_IEXEC)


def main() -> None:
    system, machine, exe = host()
    bin_dir.mkdir(parents=True, exist_ok=True)

    dsd_arch = "arm64" if machine == "arm64" and system == "macos" else "x86_64"
    save_exe(bin_dir / f"dsd{exe}", github_tool("AetiasHax/ds-decomp", DSD, f"dsd-{system}-{dsd_arch}{exe}"))

    objdiff_arch = ("aarch64" if system == "linux" else "arm64") if machine == "arm64" else "x86_64"
    save_exe(bin_dir / f"objdiff-cli{exe}",
             github_tool("encounter/objdiff", OBJDIFF, f"objdiff-cli-{system}-{objdiff_arch}{exe}"))

    # wibo runs the Windows-only Metrowerks tools on macOS and Linux.
    if system != "windows":
        wibo_asset = "wibo-macos" if system == "macos" else "wibo-x86_64"
        save_exe(bin_dir / "wibo", github_tool("decompals/wibo", WIBO, wibo_asset))

    if not (root / "tools" / "mwccarm").is_dir():
        zipfile.ZipFile(io.BytesIO(fetch(MWCCARM_URL, MWCCARM_SHA256))).extractall(root / "tools")

    print("Done.")


if __name__ == "__main__":
    sys.exit(main())
