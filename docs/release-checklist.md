# FifeGUI Release Steps

This is a checklist documenting the steps required to perform a FifeGUI release.

We need this document, because maintainers might change and we need to transfer
the knowledge of "how to do a release properly" to the next person having the maintainer role.
In the end, it should form a checklist guide for the maintainer.

We'll refine the checklist over time, so that it becomes more complete.

## 1. Prerequisites

- [ ] You need to be a maintainer (with push access to the repository).
- [ ] You need to have access to GitHub Actions
    - so that you can delete and re-trigger builds manually when things go wrong during automated builds and releases.
- [ ] Your local Git repository must be synced with the `main` branch.

## 2. Release Preparation & Versioning

- [ ] Pick a new version number (e.g., `1.2.3`).
- [ ] Update to the new version number.
    - [ ] Update `vcpgk.json`.
    - [ ] Then run `./build-tools/set-version.sh` to update the version in all relevant files.
- [ ] Update `AUTHORS`.
- [ ] Update `CHANGELOG`.
  - [ ] Roll changelog data from the **"Unreleased"** section to the new version section.
  - [ ] Link the new header (add a new reference at the end of the changelog).
  - [ ] Fix the link to **Unreleased** (update the old version number with the new version number).
- [ ] `git commit` + `git push` — add changes until everything is updated and ready.

### Pick Release Commit

- [ ] Pick the release commit.
  This is the **golden commit** — the base for the release tag.
- [ ] Check that the commit message **does not contain** `[skip ci]`.
- [ ] Check that the builds of this commit are all **green on CI services**.

## 3. Release Execution (Tagging)

The release is performed automatically by tagging the commit.
The build artifacts of the tag are then released to **GitHub Releases**.

- [ ] Identify the hash of the verified **Golden Commit**.

- [ ] Create the tag:

```bash
git tag -a <tagname=version> <git-hash-of-release-commit>
```

Example:

```bash
git tag -a 1.2.3 9fceb02
```

This tags the commit `9fceb02` as version `1.2.3`.

- [ ] Push the tag:

```bash
git push origin <tagname=version>
```

Example:

```bash
git push origin 1.2.3
```

This pushes the single tag `1.2.3` to `origin`.

- [ ] Wait for CI services to complete the **tag build**.
- [ ] Wait for CI services to deploy the build artifacts to **GitHub Releases**.

## 4. Post-Release

- [ ] Verify the release page: check that all build artifacts for the release are attached.
- [ ] Announce the release in the **IRC channel**.
- [ ] Announce the release in the **Discord channel**.
- [ ] Update the **Overview channel** in the Unknown Horizons Discord server.
- [ ] (Optional) Update the website: update download links.

## 5. Emergency Rollback (If Build Fails)

- [ ] Delete local tag:

```bash
git tag -d <version>
```

- [ ] Delete remote tag via CLI:

```bash
git push --delete origin <version>
```

or delete it via the **GitHub UI**.

- [ ] Fix issues on the branch.
- [ ] Create a new commit.
- [ ] Repeat the process starting from **Step 2**.
```

