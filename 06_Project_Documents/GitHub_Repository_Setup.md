# GitHub Repository Setup Notes

This project is initialized as a local Git repository so it can be opened in GitHub Desktop and published to GitHub.

## Important

This project uses Git LFS for large files:

- PDFs.
- Images.
- Videos.
- Axure `.rp` files.
- Unreal `.uasset` and `.umap` files.
- Packaged binaries/archives.

Git LFS is necessary because the project already contains a very large project-board PDF and will likely contain Unreal assets later.

## GitHub Desktop Workflow

1. Open GitHub Desktop.
2. Choose `File > Add Local Repository`.
3. Select:

```text
G:\Workspace\University\Mumosa
```

4. Confirm the repository opens.
5. Use `Publish repository` when ready.

## Recommended Repository Settings

Suggested repository name:

```text
mumosa-spatial-review-prototype
```

Suggested visibility:

- Private while the project is active.
- Public only if the team confirms media/source permissions and removes anything sensitive.

## Unreal Notes

The `.gitignore` ignores Unreal-generated folders such as:

- `Binaries/`
- `DerivedDataCache/`
- `Intermediate/`
- `Saved/`
- `.vs/`

These should not be committed because they are regenerated locally and can become huge.

Commit these Unreal folders/files when the Unreal project is created:

- `Source/`
- `Content/`
- `Config/`
- `Plugins/` if needed.
- `.uproject`

## Large Media Notes

Keep substitute prototype media in:

```text
07_Prototype_Media/
```

Track source and attribution in:

```text
07_Prototype_Media/media_manifest.md
```

Do not add random internet media without a source URL and usage note.

