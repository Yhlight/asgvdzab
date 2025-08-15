#!/bin/bash

# CJMOD Module Packaging Script - macOS Platform
# Since macOS and Linux are both Unix-like systems, directly call the Linux version

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
LINUX_SCRIPT="$SCRIPT_DIR/../linux/package-cjmod.sh"

if [ ! -x "$LINUX_SCRIPT" ]; then
    echo "[ERROR] Cannot find Linux version of package-cjmod.sh script"
    exit 1
fi

# Directly call Linux version, passing all arguments
exec "$LINUX_SCRIPT" "$@"