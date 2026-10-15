#!/usr/bin/env python3
"""Rewrites an mwcc .d file (Windows backslash paths) with forward slashes for ninja.

Usage: fix_dep.py <in.d> <out.d>
"""
import sys

text = open(sys.argv[1]).read()
open(sys.argv[2], "w").write(text.replace("\\\n", "\0").replace("\\", "/").replace("\0", "\\\n"))
