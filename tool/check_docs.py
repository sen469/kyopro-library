"""Check generated documentation links and unexpanded ACL placeholders."""

from html.parser import HTMLParser
import argparse
from pathlib import Path
from urllib.parse import unquote, urlsplit
import sys


class Page(HTMLParser):
    def __init__(self, source):
        super().__init__()
        self.ids = set()
        self.links = []
        self.assets = []
        self.feed(source)

    def handle_starttag(self, tag, attrs):
        attrs = dict(attrs)
        if "id" in attrs:
            self.ids.add(attrs["id"])
        for attr in ("href", "src"):
            if attr in attrs:
                self.links.append(attrs[attr])
        if tag in ("script", "img", "iframe") and "src" in attrs:
            self.assets.append(attrs["src"])
        if tag == "link" and "stylesheet" in attrs.get("rel", "").split():
            self.assets.append(attrs.get("href", ""))


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("site_dir", nargs="?", default="site")
    parser.add_argument("--offline", action="store_true")
    args = parser.parse_args()
    site_prefix = "/" if args.offline else "/kyopro-library/"
    repo = Path.cwd().resolve()
    root = Path(args.site_dir).resolve()
    pages = {}
    errors = []
    for path in root.rglob("*.html"):
        source = path.read_text(encoding="utf-8")
        pages[path] = Page(source)
        if "@{keyword." in source or "@{example." in source:
            errors.append(f"{path.relative_to(root)}: unexpanded ACL placeholder")
    if not pages:
        sys.exit("No built pages found. Run mkdocs build --strict first.")

    for path, page in pages.items():
        if args.offline:
            for asset in page.assets:
                if urlsplit(asset).scheme in ("http", "https") or asset.startswith("//"):
                    errors.append(f"{path.relative_to(root)}: external asset: {asset}")
        for link in page.links:
            url = urlsplit(link)
            source_prefix = "/sen469/kyopro-library/blob/main/"
            if url.netloc == "github.com" and url.path.startswith(source_prefix):
                if args.offline:
                    errors.append(f"{path.relative_to(root)}: external source link: {link}")
                source = (repo / unquote(url.path[len(source_prefix):])).resolve()
                if not source.is_relative_to(repo) or not source.is_file():
                    errors.append(f"{path.relative_to(root)}: missing source: {link}")
            if url.scheme or url.netloc:
                continue
            target_path = unquote(url.path)
            if target_path.startswith("/"):
                if not target_path.startswith(site_prefix):
                    errors.append(f"{path.relative_to(root)}: outside site prefix: {link}")
                    continue
                target = root / target_path[len(site_prefix):]
            else:
                target = path.parent / target_path if target_path else path
            target = target.resolve()
            if target.is_dir():
                target /= "index.html"
            if not target.is_relative_to(root) or not target.is_file():
                errors.append(f"{path.relative_to(root)}: missing target: {link}")
            elif url.fragment and target in pages:
                if unquote(url.fragment) not in pages[target].ids:
                    errors.append(f"{path.relative_to(root)}: missing anchor: {link}")

    if errors:
        sys.exit("\n".join(errors))
    print(f"Checked {len(pages)} HTML pages: links and ACL placeholders OK")


if __name__ == "__main__":
    main()
