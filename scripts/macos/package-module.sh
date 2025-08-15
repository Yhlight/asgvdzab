#!/bin/bash

# CJMOD模块打包脚本 - macOS平台
# 由于macOS和Linux相似，直接调用Linux脚本

# 获取脚本所在目录
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

# 调用Linux脚本
exec "$SCRIPT_DIR/../linux/package-module.sh" "$@"