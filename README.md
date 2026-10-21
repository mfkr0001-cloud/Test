# GD Tools — GitHub Actions build (Geometry Dash 2.208)

This repository is configured so GitHub builds Android packages for you. You do **not** need to install CMake, Ninja, the Android NDK, Geode CLI, or the Geode SDK on your own computer.

## Easiest setup

1. Create a repository on GitHub (a public repository is simplest).
2. Upload **all files and folders inside this project**. Make sure `.github/workflows/build.yml` is included. On GitHub's web uploader, hidden folders can be easy to miss.
3. Open the repository's **Actions** tab and enable Actions if GitHub asks.
4. Select **Build Android Geode mod** and click **Run workflow**. A push to the repository also starts a build automatically.
5. Open the finished workflow run. Under **Artifacts**, download:
   - `GDTools-Android64` for most modern Android phones (arm64)
   - `GDTools-Android32` only for a 32-bit Android/Geode installation
6. Extract the downloaded artifact ZIP to get the `.geode` file, then install it using Geode's supported mod installation flow.

Official build action: https://github.com/geode-sdk/build-geode-mod
Geode Android build docs: https://github.com/geode-sdk/docs/blob/main/getting-started/create-mod.md

## Target game version

The mod metadata targets Geometry Dash `2.208`. The Geode loader version is a separate thing from the GD game version. If the installed loader rejects the package due to Geode-version compatibility, check the installed Geode version and adjust the `geode` field in `mod.json` to a compatible SDK/loader version, then push the change and rerun the workflow.

## What is currently implemented

This source is still an early mod prototype. It includes a basic in-level toggle intended to skip the standard player-death handler. It is **not** a full collision bypass and does not yet implement replay recording/playback, speedhack, or hitbox visualization. The GitHub workflow automates compilation; it cannot make unimplemented features work or guarantee a successful build before the workflow is run.

## If the build fails

Open the failed Actions run and inspect the first error in the **Build Geode mod** step. Common causes include an SDK/loader version mismatch or a C++ hook/API mismatch. Download the full build log and share it to troubleshoot.
