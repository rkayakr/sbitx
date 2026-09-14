# Deploying the sBitx MkDocs Site

The sBitx documentation is built with **MkDocs Material** from `docs/mkdocs/`.

## Prerequisites

Install tooling from the repository root:

```bash
pip install -r docs/requirements.txt
```

## Local Development

### Run local docs server

```bash
mkdocs serve
```

### Build static site

```bash
mkdocs build
```

The generated site output is in `site/`.

## Manual Deployment via SCP

```bash
mkdocs build
ssh -i ~/.ssh/deploy_key -o UserKnownHostsFile=~/.ssh/known_hosts -o StrictHostKeyChecking=yes -o IdentitiesOnly=yes ${DEPLOY_USER}@${DEPLOY_HOST} "mkdir -p '${DEPLOY_PATH}'"
scp -o UserKnownHostsFile=~/.ssh/known_hosts -o StrictHostKeyChecking=yes -o IdentitiesOnly=yes -r site/. ${DEPLOY_USER}@${DEPLOY_HOST}:${DEPLOY_PATH}/
```

Suggested deployment variables:

- `DEPLOY_HOST` — target host
- `DEPLOY_USER` — SSH user
- `DEPLOY_PATH` — destination path on server

## Versioned Deployment via mike

Use `mike` when you want versioned docs and a `latest` alias.

> Note: This scaffold does not yet add a Material version selector configuration in `mkdocs.yml`.
> The commands below still publish versioned directories and aliases, but selector UI setup can be added later if needed.

```bash
# Example: deploy version 4.301 and update latest alias
mike deploy 4.301 latest --update-aliases

# Set default version presented by mike
mike set-default latest
```

## GitHub Actions Example (`.github/workflows/docs.yml`)

The workflow below builds and deploys docs on pushes to `mkdoc` and `main` when docs sources change.

```yaml
name: Deploy Docs

on:
  push:
    branches:
      - mkdoc
      - main
    paths:
      - 'docs/mkdocs/**'
      - 'mkdocs.yml'
      - 'docs/requirements.txt'
      - 'docs/RxEQ.odt'
      - 'docs/apfdoc.pdf'
      - 'docs/Using the QSSTV-sBitx Edition.pdf'
      - 'docs/Wiki-Resources/**'
      - '.github/workflows/docs.yml'
      - 'DEPLOY_DOCS.md'
  workflow_dispatch:

jobs:
  deploy-docs:
    runs-on: ubuntu-latest
    steps:
      - name: Checkout
        uses: actions/checkout@v4

      - name: Setup Python
        uses: actions/setup-python@v5
        with:
          python-version: '3.11'

      - name: Install dependencies
        run: pip install -r docs/requirements.txt

      - name: Build docs
        run: mkdocs build

      - name: Configure SSH key
        env:
          DEPLOY_KEY: ${{ secrets.DEPLOY_KEY }}
          DEPLOY_KNOWN_HOSTS: ${{ secrets.DEPLOY_KNOWN_HOSTS }}
        run: |
          mkdir -p ~/.ssh
          echo "$DEPLOY_KEY" > ~/.ssh/deploy_key
          chmod 600 ~/.ssh/deploy_key
          echo "$DEPLOY_KNOWN_HOSTS" > ~/.ssh/known_hosts
          chmod 644 ~/.ssh/known_hosts

      - name: Deploy via SCP
        env:
          DEPLOY_HOST: ${{ secrets.DEPLOY_HOST }}
          DEPLOY_USER: ${{ secrets.DEPLOY_USER }}
          DEPLOY_PATH: ${{ secrets.DEPLOY_PATH }}
        run: |
          ssh -i ~/.ssh/deploy_key \
            -o UserKnownHostsFile=~/.ssh/known_hosts \
            -o StrictHostKeyChecking=yes \
            -o IdentitiesOnly=yes \
            "${DEPLOY_USER}@${DEPLOY_HOST}" "mkdir -p '${DEPLOY_PATH}'"
          scp -i ~/.ssh/deploy_key \
            -o UserKnownHostsFile=~/.ssh/known_hosts \
            -o StrictHostKeyChecking=yes \
            -o IdentitiesOnly=yes \
            -r site/. "${DEPLOY_USER}@${DEPLOY_HOST}:${DEPLOY_PATH}/"
```

Required repository secrets:

- `DEPLOY_HOST`
- `DEPLOY_USER`
- `DEPLOY_KEY`
- `DEPLOY_PATH`
- `DEPLOY_KNOWN_HOSTS` (pinned known_hosts entry for your deployment host)

You can adjust the `branches:` list to match your preferred publishing flow.

## Notes

- Keep `docs/Wiki-Resources/` and legacy PDF/ODT files in place for backward compatibility.
- The docs in `docs/mkdocs/` are now the MkDocs source of truth for site navigation.
