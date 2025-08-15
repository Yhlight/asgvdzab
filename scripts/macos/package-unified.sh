#!/bin/bash

# 统一模块打包脚本 - macOS平台
# 由于macOS和Linux都是Unix-like系统，直接调用Linux版本

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
LINUX_SCRIPT="$SCRIPT_DIR/../linux/package-unified.sh"

if [ ! -x "$LINUX_SCRIPT" ]; then
    echo "[ERROR] 找不到Linux版本的package-unified.sh脚本"
    exit 1
fi

# 直接调用Linux版本，传递所有参数
exec "$LINUX_SCRIPT" "$@"