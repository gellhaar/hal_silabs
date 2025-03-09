#!/usr/bin/env python

"""
Copyright (c) 2024 Silicon Laboratories Inc.

SPDX-License-Identifier: Apache-2.0
"""
import argparse
import re
from pathlib import Path

devices = {
	"xg21": {
		"bits": "platform/Device/SiliconLabs/EFR32MG21/Include/efr32mg21_cmu.h",
  },
	"xg22": {
		"bits": "platform/Device/SiliconLabs/EFR32BG22/Include/efr32bg22_cmu.h",
		"nodes": "platform/service/device_manager/clocks/sl_device_clock_efr32xg22.c"
  },
	"xg23": {
		"bits": "platform/Device/SiliconLabs/EFR32FG23/Include/efr32fg23_cmu.h",
		"nodes": "platform/service/device_manager/clocks/sl_device_clock_efr32xg23.c"
  },
	"xg24": {
		"bits": "platform/Device/SiliconLabs/EFR32MG24/Include/efr32mg24_cmu.h",
		"nodes": "platform/service/device_manager/clocks/sl_device_clock_efr32xg24.c"
  },
	"bgm24": {
		"bits": "platform/Device/SiliconLabs/BGM24/Include/bgm24_cmu.h",
		"nodes": "platform/service/device_manager/clocks/sl_device_clock_efr32xg24.c"
  },
	"xg27": {
		"bits": "platform/Device/SiliconLabs/EFR32BG27/Include/efr32bg27_cmu.h",
		"nodes": "platform/service/device_manager/clocks/sl_device_clock_efr32xg27.c"
  },
	"xg29": {
		"bits": "platform/Device/SiliconLabs/EFR32BG29/Include/efr32bg29_cmu.h",
		"nodes": "platform/service/device_manager/clocks/sl_device_clock_efr32xg29.c"
  },
}

clocks = {
  "BUS_CLOCK_CLKEN0": 0,
  "BUS_CLOCK_CLKEN1": 1,
  "BUS_CLOCK_CLKEN2": 2,
  "BUS_CLOCK_CLKENHV": 3,
}

if __name__ == "__main__":
  parser = argparse.ArgumentParser(description="Generate headers for Clock Control for Series 2 "
                                   "devices. The headers are used from DeviceTree, and represent "
                                   "every clock enable bit as a DT compatible macro.")
  parser.add_argument("--out", "-o", type=Path, default=Path(__file__).parent / "out",
                      help="Output directory. Defaults to the directory ./out/ relative to the "
                      "script. Set to $ZEPHYR_BASE/include/zephyr/dt-bindings/clock/silabs/ "
                      "to directly generate output into the expected location within the Zephyr "
                      "main tree.")
  parser.add_argument("--sdk", "-s", type=Path, default=Path(__file__).parent.parent / "simplicity_sdk",
                      help="Path to Simplicity SDK to extract data from. Defaults to the directory "
                       "../simplicity_sdk relative to the script.")
  args = parser.parse_args()

  args.out.mkdir(exist_ok=True)

  for device, data_sources in devices.items():
    print(f"Generate clock control binding for {device}")
    bits_file = (args.sdk / data_sources["bits"]).resolve()
    bits = {}
    with bits_file.open() as f:
      for line in f:
        if m := re.match(r"#define (_CMU_CLKEN.*_SHIFT)\s+(\d+)", line):
          bits[m.group(1)] = m.group(2)

    nodes = []
    if "nodes" in data_sources:
      node_file = (args.sdk / data_sources["nodes"]).resolve(strict=True)
      with node_file.open() as f:
        for line in f:
          if m := re.match(r".*uint32_t SL_BUS_(.*)_VALUE = \(([^\s]+).*(_CMU[^\s]+SHIFT)", line):
            try:
              nodes.append(f"#define {m.group(1)}"
                          f"{' ' * (20 - len(m.group(1)))}"
                          f"(FIELD_PREP(CLOCK_REG_MASK, {clocks[m.group(2)]}) | "
                          f"FIELD_PREP(CLOCK_BIT_MASK, {bits[m.group(3)]}))"
              )
            except KeyError as e:
              print(f"WARN: Failed to emit clock node: {e}")
    else:
      # xg21 has on-demand automatic clock requests, there are no enable bits
      nodes.append("#define CLOCK_AUTO 0xFFFFFFFFUL")

    file = [
      "/*",
      " * Copyright (c) 2024 Silicon Laboratories Inc.",
      " *",
      " * SPDX-License-Identifier: Apache-2.0",
      " *",
      f" * This file was generated by the script {Path(__file__).name} in the hal_silabs module.",
      " * Do not manually edit.",
      " */",
      "",
      f"#ifndef ZEPHYR_INCLUDE_DT_BINDINGS_CLOCK_SILABS_{device.upper()}_CLOCK_H_",
      f"#define ZEPHYR_INCLUDE_DT_BINDINGS_CLOCK_SILABS_{device.upper()}_CLOCK_H_",
      "",
      "#include <zephyr/dt-bindings/dt-util.h>",
      "#include \"common-clock.h\"",
      "",
      "/*",
      " * DT macros for clock tree nodes.",
      " * Defined as:",
      " *  0..5 - Bit within CLKEN register",
      " *  6..8 - CLKEN register number",
      " * Must stay in sync with equivalent SL_BUS_*_VALUE constants in the Silicon Labs HAL to be",
      " * interpreted correctly by the clock control driver.",
      " */",
    ] + nodes + [
      "",
      f"#endif /* ZEPHYR_INCLUDE_DT_BINDINGS_CLOCK_SILABS_{device.upper()}_CLOCK_H_ */",
      "",
    ]

    outfile = args.out / f"{device}-clock.h"
    outfile.write_text("\n".join(file))
