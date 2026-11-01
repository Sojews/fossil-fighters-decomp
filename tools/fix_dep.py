#!/usr/bin/env python3
"""Rewrites an mwcc .d file (Windows backslash paths) with forward slashes for ninja.

Usage: fix_dep.py <in.d> <out.d>
"""
import re
import sys

text = open(sys.argv[1]).read()
text = text.replace("\\\n", "\0").replace("\\", "/").replace("\0", "\\\n")
# wibo maps the host root to drive Z:; strip it so ninja finds the headers (else every build recompiles everything)
open(sys.argv[2], "w").write(re.sub(r"(?<![^\s])Z:/", "/", text))
