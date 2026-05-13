# Agent Notes

This repo contains an active Unreal Editor workflow where command-line C++ builds and the running editor session can drift apart.

## Unreal C++ Hot Reload Rule

After changing C++ in `DerelictCorridorMegascans`, do not assume a successful build means the open Unreal Editor session is using the new code.

Required workflow:

1. Make the code change.
2. Trigger Live Coding / hot reload for the already-open Unreal Editor session.
   - Preferred repo helper: `powershell -ExecutionPolicy Bypass -File .\Scripts\TriggerUnrealLiveCoding.ps1`
   - Reliable manual fallback: `Ctrl+Alt+F11` in Unreal Editor.
3. Confirm the patch was applied before testing.
   - Preferred signal: `LogLiveCoding: Display: Live coding succeeded`
4. Only then trust PIE or VR test results.

## Important Distinction

These are separate states:

- build succeeded
- editor session loaded the patch

Treat "nothing changed" as a possible stale-editor problem before assuming the feature logic failed.
