#!/usr/bin/env bash
set -euo pipefail

# Edit this path to your usual competitive programming directory.
COMPETITIVE_PROGRAMMING_DIR="${COMPETITIVE_PROGRAMMING_DIR:-$HOME/competitive-programming}"

REPO_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
TARGET_DIR="${1:-$COMPETITIVE_PROGRAMMING_DIR}"
FORCE=0

usage() {
    cat <<'USAGE'
Usage:
  ./setup.sh [target_dir]
  COMPETITIVE_PROGRAMMING_DIR=/path/to/procon ./setup.sh
  ./setup.sh --force [target_dir]

Installs kyopro-library into the target directory:
  - creates/updates lib -> this repository's lib symlink
  - copies tool/expander.py
  - copies tool/rantes.sh
  - creates generate.py and ans.cpp templates if missing

Edit COMPETITIVE_PROGRAMMING_DIR near the top of this file to make
plain ./setup.sh install into your usual directory.
USAGE
}

if [ "${1:-}" = "-h" ] || [ "${1:-}" = "--help" ]; then
    usage
    exit 0
fi

if [ "${1:-}" = "--force" ]; then
    FORCE=1
    TARGET_DIR="${2:-$COMPETITIVE_PROGRAMMING_DIR}"
fi

require_file() {
    local file="$1"
    if [ ! -f "$file" ]; then
        echo "setup.sh: required file not found: $file" >&2
        exit 1
    fi
}

install_copy() {
    local src="$1"
    local dst="$2"

    if [ -d "$dst" ] && [ ! -L "$dst" ]; then
        echo "setup.sh: $dst already exists and is a directory." >&2
        exit 1
    fi

    if [ -L "$dst" ]; then
        rm -f "$dst"
    fi

    cp "$src" "$dst"
    chmod +x "$dst"
}

install_template() {
    local dst="$1"
    local kind="$2"

    if [ -e "$dst" ] || [ -L "$dst" ]; then
        echo "kept existing $(basename "$dst")"
        return
    fi

    case "$kind" in
        generate.py)
            cat > "$dst" <<EOF
#!/usr/bin/env python3

def main():
    # Write random test input here.
    pass

if __name__ == "__main__":
    main()
EOF
            chmod +x "$dst"
            ;;
        ans.cpp)
            cat > "$dst" <<EOF
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    return 0;
}
EOF
            ;;
        *)
            echo "setup.sh: unknown template: $kind" >&2
            exit 1
            ;;
    esac

    echo "created $(basename "$dst")"
}

install_lib_symlink() {
    local src="$REPO_ROOT/lib"
    local dst="$TARGET_DIR/lib"

    if [ -L "$dst" ]; then
        local current
        current="$(readlink "$dst")"
        if [ "$current" = "$src" ]; then
            echo "lib symlink already points to $src"
            return
        fi

        if [ "$FORCE" -ne 1 ]; then
            echo "setup.sh: $dst already points to $current. Use --force to replace it." >&2
            exit 1
        fi
        rm -f "$dst"
    elif [ -e "$dst" ]; then
        echo "setup.sh: $dst already exists and is not a symlink." >&2
        echo "Remove it manually or choose another target directory." >&2
        exit 1
    fi

    ln -s "$src" "$dst"
    echo "linked lib -> $src"
}

require_file "$REPO_ROOT/tool/expander.py"
require_file "$REPO_ROOT/tool/rantes.sh"

mkdir -p "$TARGET_DIR"

install_lib_symlink
install_copy "$REPO_ROOT/tool/expander.py" "$TARGET_DIR/expander.py"
install_copy "$REPO_ROOT/tool/rantes.sh" "$TARGET_DIR/rantes.sh"
install_template "$TARGET_DIR/generate.py" generate.py
install_template "$TARGET_DIR/ans.cpp" ans.cpp

echo "installed kyopro-library tools into $TARGET_DIR"
