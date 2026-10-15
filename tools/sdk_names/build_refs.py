#!/usr/bin/env python3
"""Clones the reference library decomps at pinned commits and compiles them the way Fossil Fighters' copies were built.

Usage: python3 tools/sdk_names/build_refs.py      (then match.py and apply.py, see README.md)

Outputs objects to tools/sdk_names/obj/<variant>/.  Nothing here is linked into the game.
"""
import os
import re
import subprocess
import sys
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path

HERE = Path(__file__).resolve().parent
ROOT = HERE.parent.parent
REF = HERE / "ref"
OBJ = HERE / "obj"

REPOS = {
    "NitroSDK": ("https://github.com/ntrtwl/NitroSDK", "9eec6c92c9d7bba9350732b11296ed16aab3a8bc"),
    "NitroSystem": ("https://github.com/ntrtwl/NitroSystem", "d636fb5d2b212e8736f09859fff447be2bd0ac85"),
    "st": ("https://github.com/zeldaret/st", "048ffeaaaae4137aa258e2ecb3e9bf996c98d435"),
}

SDK_FLAGS = ("-O4,p -proc arm946e -enum int -lang c99 -Cpp_exceptions off -gccext,on -msgstyle gcc -ipa file "
             "-interworking -inline on,noauto -char signed -nosyspath -stdinc -sym on -cwd include "
             "-DSDK_CW_FORCE_EXPORT_SUPPORT -DSDK_TS -DSDK_4M -DSDK_ARM9 -DSDK_CW -DSDK_FINALROM -DNNS_FINALROM").split()
MSL_FLAGS = ("-O4,p -proc arm946e -enum int -char signed -str reuse -gccext,on -fp soft -inline noauto -lang c "
             "-Cpp_exceptions off -interworking -w off -sym on -gccinc -nolink -msgstyle gcc -ipa file").split()
THUMB = ["-thumb", "-DSDK_CODE_THUMB"]   # the game links the Thumb builds of the libraries
ARM = ["-DSDK_CODE_ARM"]                 # hand-written ARM functions are matched from these

# (name, repo, compiler, extra flags)
VARIANTS = [
    ("sdk_thumb", "NitroSDK", "2.0/sp1p2", THUMB),
    ("sdk_arm", "NitroSDK", "2.0/sp1p2", ARM),
    ("nns_thumb", "NitroSystem", "2.0/sp2", THUMB),
    ("msl_thumb", "st", "2.0/sp1p5", ["-thumb"]),
]


def clone(name: str) -> Path:
    url, rev = REPOS[name]
    path = REF / name
    if not path.is_dir():
        subprocess.run(["git", "init", "-q", str(path)], check=True)
        subprocess.run(["git", "-C", str(path), "fetch", "-q", "--depth", "1", url, rev], check=True)
        subprocess.run(["git", "-C", str(path), "checkout", "-q", "FETCH_HEAD"], check=True)
    return path


def sources(repo: Path, name: str) -> list[Path]:
    if name == "st":
        return sorted((repo / "libs/c/src").rglob("*.c"))
    srcs = set()
    for mb in repo.glob("libraries/**/meson.build"):
        for var, body in re.findall(r"(\w+)\s*=\s*files\((.*?)\)", mb.read_text(), re.S):
            if "arm7" not in var:
                srcs |= {mb.parent / f for f in re.findall(r"'([^']+\.c)'", body) if (mb.parent / f).exists()}
    return sorted(srcs)


def main() -> None:
    sdk = clone("NitroSDK")
    clone("NitroSystem")
    st = clone("st")
    fx_const = sdk / "gen/nitro/fx/fx_const.h"
    if not fx_const.exists():
        subprocess.run([sys.executable, "-I", str(sdk / "gen/nitro/fx/gen_fx_const.py"),
                        str(sdk / "gen/nitro/fx/fx_const.csv"), str(fx_const)], check=True)

    jobs = []
    for variant, name, ver, extra in VARIANTS:
        repo = REF / name
        cc = [str(ROOT / "tools/bin/wibo"), str(ROOT / f"tools/mwccarm/{ver}/mwccarm.exe")]
        if name == "st":
            flags = MSL_FLAGS + extra + ["-i", str(st / "libs/c/include")]
            base = repo / "libs/c/src"
        else:
            srcs = sources(repo, name)
            # Libraries include each other's private headers, so every library's src/ and include/ is searched.
            private = sorted({s.parent for s in srcs} | {s.parent.parent / "include" for s in srcs
                                                         if (s.parent.parent / "include").is_dir()})
            incs = [sdk / "gen", sdk / "include", repo / "include", st / "libs/c/include", *private]
            flags = SDK_FLAGS + extra + [a for i in incs for a in ("-i", str(i))] + ["-prefix", "nitro.h"]
            base = repo / "libraries"
        for src in sources(repo, name):
            out = OBJ / variant / src.relative_to(base).with_suffix(".o")
            jobs.append((cc + flags + ["-i", str(src.parent), "-c", str(src), "-o", str(out)], out, repo))

    def build(job):
        cmd, out, cwd = job
        out.parent.mkdir(parents=True, exist_ok=True)
        subprocess.run(cmd, capture_output=True, cwd=cwd)
        return out.exists()   # wibo exits 0 even when mwcc fails, so check for the object

    with ThreadPoolExecutor(os.cpu_count()) as ex:
        ok = sum(ex.map(build, jobs))
    print(f"Compiled {ok}/{len(jobs)} reference files into {OBJ.relative_to(ROOT)}/ (a few ARM7-only files are expected to fail)")


if __name__ == "__main__":
    main()
