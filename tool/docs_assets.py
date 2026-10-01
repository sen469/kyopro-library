"""Bundle math assets and expose source files in the local documentation."""

from pathlib import Path
import re

from mkdocs.structure.files import File
from mkdocs.utils import get_relative_url


ROOT = Path(__file__).resolve().parent.parent
SOURCE_URL = "https://github.com/sen469/kyopro-library/blob/main/"


def on_files(files, config):
    vendor = ROOT / "docs" / "atcoder" / "lib"
    for name in ("katex.min.js", "auto-render.min.js", "katex.min.css"):
        files.append(File.generated(
            config, f"assets/katex/{name}", content=(vendor / name).read_bytes()
        ))
    for font in sorted((vendor / "fonts").glob("*.woff2")):
        files.append(File.generated(
            config, f"assets/katex/fonts/{font.name}", content=font.read_bytes()
        ))
    files.append(File.generated(
        config, "assets/katex/LICENSE.txt", content=(vendor / "LICENSE.md").read_bytes()
    ))

    if config.extra.get("local_docs"):
        for source in sorted((ROOT / "lib").rglob("*")):
            if source.is_file():
                relative = source.relative_to(ROOT).as_posix()
                files.append(File.generated(
                    config, f"sources/{relative}.txt", content=source.read_bytes()
                ))
    return files


def on_page_markdown(markdown, page, config, files):
    if not config.extra.get("local_docs"):
        return markdown

    # Rewrite only this repository's source links; examples on other sites stay external.
    pattern = re.escape(SOURCE_URL) + r"(lib/[^\s)]+)"
    return re.sub(
        pattern,
        lambda match: get_relative_url(
            f"sources/{match[1]}.txt", page.file.src_uri
        ),
        markdown,
    )
