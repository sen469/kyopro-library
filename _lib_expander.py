#!/usr/bin/env python3

import argparse
import re
import sys
from logging import basicConfig
from os import environ, getenv, pathsep
from pathlib import Path
from typing import Optional


INCLUDE_RE = re.compile(r'^(\s*)#\s*include\s*"([^"]+)"\s*(?://.*)?$')


class Expander:
    def __init__(self, include_dirs: list[Path], ignore_paths: Optional[list[Path]] = None):
        self.include_dirs = []
        for include_dir in include_dirs:
            include_dir = include_dir.resolve()
            if include_dir not in self.include_dirs:
                self.include_dirs.append(include_dir)
        self.expanded = set()
        self.ignore_paths = {p.resolve() for p in ignore_paths} if ignore_paths else set()

    def is_ignored(self, path: Path) -> bool:
        if path in self.ignore_paths:
            return True
        for parent in path.parents:
            if parent in self.ignore_paths:
                return True
        return False

    def resolve_include(self, include_path: str, current_dir: Path) -> Optional[Path]:
        candidates = [current_dir / include_path]
        candidates += [include_dir / include_path for include_dir in self.include_dirs]

        for candidate in candidates:
            if candidate.is_file():
                return candidate.resolve()
        return None

    def expand_file(self, path: Path, origname: Optional[str] = None) -> list[str]:
        path = path.resolve()
        result = []
        source_name = origname if origname is not None else str(path)

        if origname is not None:
            result.append(f'#line 1 "{source_name}"\n')

        lines = path.read_text().splitlines(keepends=True)
        for lineno, line in enumerate(lines, 1):
            match = INCLUDE_RE.match(line.rstrip("\n"))
            if not match:
                result.append(line)
                continue

            include_path = match.group(2)
            resolved = self.resolve_include(include_path, path.parent)

            if resolved is None:
                result.append(line)
                continue

            if self.is_ignored(resolved):
                result.append(line)
                continue

            if resolved in self.expanded:
                continue

            self.expanded.add(resolved)
            result.append(f"// begin: {include_path}\n")
            result.extend(self.expand_file(resolved, str(resolved) if origname is not None else None))
            if result and not result[-1].endswith("\n"):
                result[-1] += "\n"
            result.append(f"// end: {include_path}\n")
            if origname is not None:
                result.append(f'#line {lineno + 1} "{source_name}"\n')

        return result


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description="Expand local kyopro-library includes for submission."
    )
    parser.add_argument("source", type=Path, help="source file")
    parser.add_argument(
        "-c",
        "--console",
        action="store_true",
        help="print expanded source to console",
    )
    parser.add_argument(
        "-o",
        "--output",
        type=Path,
        default=Path("combined.cpp"),
        help="output file when --console is not specified (default: combined.cpp)",
    )
    parser.add_argument(
        "--lib",
        action="append",
        type=Path,
        help="additional include path. This option can be specified multiple times",
    )
    parser.add_argument(
        "--ignore",
        action="append",
        type=Path,
        help="ignore specific file from expansion. This option can be specified multiple times",
    )
    parser.add_argument(
        "--origname",
        help="report line numbers from the original source file in GCC/Clang error messages",
    )
    return parser.parse_args()


def main() -> int:
    basicConfig(
        format="%(asctime)s [%(levelname)s] %(message)s",
        datefmt="%H:%M:%S",
        level=getenv("LOG_LEVEL", "INFO"),
    )

    args = parse_args()
    source = args.source.resolve()

    if not source.is_file():
        print(f"lib_expander.py: source file not found: {source}", file=sys.stderr)
        return 1

    repo_root = Path(__file__).resolve().parent
    include_dirs = []

    if args.lib:
        include_dirs.extend(args.lib)
    if "CPLUS_INCLUDE_PATH" in environ:
        include_dirs.extend(
            map(Path, filter(None, environ["CPLUS_INCLUDE_PATH"].split(pathsep)))
        )

    include_dirs.extend([source.parent, repo_root, repo_root / "lib", Path.cwd()])

    ignore_paths = args.ignore or []
    # デフォルトで lib/debug ディレクトリを除外対象にする
    default_debug_dir = repo_root / "lib/debug"
    if default_debug_dir.is_dir():
        ignore_paths.append(default_debug_dir)
    elif (repo_root / "lib/debug/debug.hpp").is_file():
        ignore_paths.append(repo_root / "lib/debug/debug.hpp")

    expander = Expander(include_dirs, ignore_paths)
    expanded = "".join(expander.expand_file(source, args.origname))

    if args.console:
        print(expanded, end="")
    else:
        args.output.write_text(expanded)

    return 0

if __name__ == "__main__":
    raise SystemExit(main())
