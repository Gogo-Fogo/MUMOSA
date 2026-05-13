# Codex Working Notes

## Unreal C++ + Live Coding

When editing C++ for the Unreal project in `DerelictCorridorMegascans`, do **not** assume that a successful command-line build means the open Unreal Editor session is running the new code.

Required workflow after C++ edits:

1. Make the code change.
2. Trigger Unreal Live Coding for the open editor session.
   - Preferred repo helper: `powershell -ExecutionPolicy Bypass -File .\Scripts\TriggerUnrealLiveCoding.ps1`
   - Preferred: use the bridge/editor hot-reload capability if available.
   - Reliable manual fallback: `Ctrl+Alt+F11` inside Unreal Editor.
3. Confirm the editor actually applied the patch before testing.
   - Look for `LogLiveCoding: Display: Live coding succeeded` in the Unreal log.
4. Only then run PIE / VR tests and judge the result.

## Important Rule

Treat these as separate states:

- `build succeeded`
- `editor session has loaded the patch`

They are not the same thing in this repo.

## Debugging Guidance

If a runtime/UI change appears to have "done nothing," first suspect stale code in the editor session before assuming the feature logic is broken.
