#!/usr/bin/env python3
"""Scan units: list functions with their instruction match % (objdiff-cli)."""
import json, subprocess, sys, os

UNITS = sys.argv[1:] or [
    "src/ov001/dino_field", "src/ov001/dino_field_camera",
    "src/ov001/dino_field_map", "src/ov001/dino_field_raster",
    "src/ov001/dino_field_town_map", "src/ov001/dino_field_chara_angle_anime",
    "src/ov001/dino_field_battle_start_effect", "src/ov001/dino_field_player_control",
    "src/ov001/dino_field_radar", "src/ov003/dino_ui_revive_status",
    "src/ov003/dino_ui_field_menu_fraction", "src/ov003/dino_ui_rv_information3d_model",
    "src/ov004/dino_ui_wireless_cancel_item", "src/ov004/disp_blower",
    "src/ov004/dino_ui_debug_reviver_menu", "src/ov004/dino_ui_island_map",
    "src/ov004/dino_ui_top_menu", "src/ov004/dino_ui_exchange",
    "src/ov004/dino_ui_debug_kaseki_rock_get", "src/ov004/dino_ui_debug_flag_menu",
    "src/ov005/dino_ui_cleaning_menu_item", "src/ov005/dino_ui_shop_sell",
]
MIN_PCT = float(os.environ.get("MIN_PCT", 99.999))

rows = []
for u in UNITS:
    try:
        d = json.loads(subprocess.run(
            ["tools/bin/objdiff-cli", "diff", "-p", ".", "-u", u, "-o", "-",
             "--format", "json"], capture_output=True, text=True, timeout=120).stdout)
    except Exception as e:
        print("FAIL", u, e, file=sys.stderr)
        continue
    for s in d.get("left", {}).get("symbols", []):
        if not s.get("instructions"):
            continue
        mp = s.get("match_percent", 0)
        if mp < MIN_PCT:
            rows.append((mp, int(s.get("size", 0) or 0), s.get("demangled_name") or s["name"], s["name"], u))
rows.sort()
for mp, size, dem, mangled, u in rows:
    print(f"{mp:7.2f}% {size:6d}B {dem}  [{u}]")
print(f"\n{len(rows)} functions below {MIN_PCT}%")
