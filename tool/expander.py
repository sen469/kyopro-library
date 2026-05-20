#!/usr/bin/env python3

import argparse
import re
import sys
from logging import basicConfig
from os import environ, getenv, pathsep
from pathlib import Path
from typing import Optional


INCLUDE_RE = re.compile(r'^(\s*)#\s*include\s*([<"])([^>"]+)([>"])\s*(?://.*)?$')


class Expander:
    def __init__(
        self,
        include_dirs: list[Path],
        ignore_paths: Optional[list[Path]] = None,
        github_base_url: Optional[str] = None,
        github_roots: Optional[list[tuple[Path, str]]] = None,
    ):
        self.include_dirs = []
        for include_dir in include_dirs:
            include_dir = include_dir.resolve()
            if include_dir not in self.include_dirs:
                self.include_dirs.append(include_dir)
        self.expanded: set[Path] = set()
        self.ignore_paths = {p.resolve() for p in ignore_paths} if ignore_paths else set()
        self.github_base_url = github_base_url.rstrip("/") if github_base_url else None
        self.github_roots = []
        for root, prefix in github_roots or []:
            self.github_roots.append((root.resolve(), prefix.strip("/")))

    def is_ignored(self, path: Path) -> bool:
        if path in self.ignore_paths:
            return True
        return any(parent in self.ignore_paths for parent in path.parents)

    def resolve_include(self, include_path: str, current_dir: Path, quoted: bool) -> Optional[Path]:
        candidates = []
        if quoted:
            candidates.append(current_dir / include_path)
        candidates.extend(include_dir / include_path for include_dir in self.include_dirs)

        for candidate in candidates:
            if candidate.is_file():
                return candidate.resolve()
        return None

    def github_url(self, path: Path) -> Optional[str]:
        if not self.github_base_url:
            return None

        for root, prefix in self.github_roots:
            try:
                rel = path.resolve().relative_to(root)
            except ValueError:
                continue

            repo_path = rel.as_posix()
            if prefix:
                repo_path = f"{prefix}/{repo_path}"
            return f"{self.github_base_url}/{repo_path}"

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

            opener = match.group(2)
            closer = match.group(4)
            if (opener == "<" and closer != ">") or (opener == '"' and closer != '"'):
                result.append(line)
                continue

            include_path = match.group(3)
            resolved = self.resolve_include(include_path, path.parent, opener == '"')

            if resolved is None or self.is_ignored(resolved):
                result.append(line)
                continue

            if resolved in self.expanded:
                continue

            self.expanded.add(resolved)
            url = self.github_url(resolved)
            if url:
                result.append(f"// begin: {include_path} ({url})\n")
            else:
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
        description="Expand kyopro-library and bundled ACL includes for submission."
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
    parser.add_argument(
        "--github-base",
        default=getenv("KYOPRO_GITHUB_BASE", "https://github.com/sen469/kyopro-library/blob/main"),
        help="base URL used in expanded include comments. Set empty string to disable",
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
        print(f"expander.py: source file not found: {source}", file=sys.stderr)
        return 1

    script_dir = Path(__file__).resolve().parent
    repo_root = script_dir
    if not (repo_root / "lib").is_dir() and (repo_root.parent / "lib").is_dir():
        repo_root = repo_root.parent

    include_dirs = []

    if args.lib:
        include_dirs.extend(args.lib)
    if "CPLUS_INCLUDE_PATH" in environ:
        include_dirs.extend(
            map(Path, filter(None, environ["CPLUS_INCLUDE_PATH"].split(pathsep)))
        )

    include_dirs.extend([source.parent, repo_root, repo_root / "lib", Path.cwd()])

    ignore_paths = args.ignore or []
    default_debug_dir = repo_root / "lib/debug"
    if default_debug_dir.is_dir():
        ignore_paths.append(default_debug_dir)
    elif (repo_root / "lib/debug/debug.hpp").is_file():
        ignore_paths.append(repo_root / "lib/debug/debug.hpp")

    github_base_url = args.github_base or None
    github_roots = [
        (repo_root / "lib", "lib"),
        (repo_root, ""),
    ]
    expander = Expander(include_dirs, ignore_paths, github_base_url, github_roots)
    expanded = "".join(expander.expand_file(source, args.origname))

    if args.console:
        print(expanded, end="")
    else:
        args.output.write_text(expanded)

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
