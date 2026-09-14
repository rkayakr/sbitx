# sBitx Documentation (MkDocs)

This directory contains the source for the sBitx MkDocs Material documentation site.

## Quick Start (Local Development)

### Install dependencies

From the repository root:

```bash
pip install -r docs/requirements.txt
```

### Run the development server

```bash
mkdocs serve
```

Open `http://localhost:8000` to preview docs with live reload.

### Build static site

```bash
mkdocs build
```

Generated output is written to `site/`.

## File Structure

```text
docs/mkdocs/
  index.md                  Home page
  quick-start.md            Installation and upgrade summary
  getting-help.md           Support and contributor guidance
  guide/
    controls.md             Main controls image guide
    rx-eq.md                Placeholder for RxEQ ODT migration
    qsstv.md                Placeholder for QSSTV PDF migration
    apf.md                  Placeholder for APF PDF migration
  reference/
    hardware.md             Hardware pinout/circuit references
    disassembly.md          Placeholder for disassembly migration
  files/                    Legacy ODT/PDF copies for direct download links
  img/                      Image assets copied from Wiki-Resources
  releases.md               Release notes copy for docs site
```

## Adding Pages

1. Create a new Markdown file under `docs/mkdocs/`.
2. Add it to the `nav:` section in `/mkdocs.yml`.
3. Run `mkdocs serve` and verify navigation, links, and formatting.

If a file is not listed in `nav:`, it will not appear in the main docs menu.

## Deployment

See `/DEPLOY_DOCS.md` for local, manual SCP, `mike`, and GitHub Actions deployment options.
