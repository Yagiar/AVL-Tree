#!/usr/bin/env bash
set -euo pipefail

# Профилирование через xctrace (Instruments) на macOS ARM.
# Требуются Xcode Command Line Tools или Xcode.

BUILD_DIR=${BUILD_DIR:-build}
CONFIG=${CONFIG:-Release}
SECONDS_TO_RECORD=${SECONDS_TO_RECORD:-10}
PROFILE_TEMPLATE=${PROFILE_TEMPLATE:-"Time Profiler"}

cmake -S . -B "$BUILD_DIR" -DCMAKE_BUILD_TYPE=$CONFIG -DBUILD_BENCHMARKS=ON -DBUILD_TESTING=OFF
cmake --build "$BUILD_DIR" --config $CONFIG -j

TARGET="$(pwd)/$BUILD_DIR/benchmarks"
if [[ ! -x "$TARGET" ]]; then
  echo "Не найден исполняемый файл: $TARGET" >&2
  exit 1
fi

OUT_TRACE=${OUT_TRACE:-profile.trace}

# Запускаем xctrace запись. Можно передать параметры бенчмарка после '--'.
xctrace record \
  --template "$PROFILE_TEMPLATE" \
  --output "$OUT_TRACE" \
  --time-limit $SECONDS_TO_RECORD \
  --launch -- "$TARGET" "$@"

echo "Готово. Откройте $OUT_TRACE в Instruments для просмотра Flame Graph."


