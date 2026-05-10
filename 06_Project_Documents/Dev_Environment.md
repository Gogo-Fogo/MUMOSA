# Development Environment

## Machine Specs

| Component | Detail |
|---|---|
| **Hostname** | GogosPC |
| **OS** | Windows 11 Pro 24H2 (build 26100) |
| **CPU** | AMD Ryzen 5 7600X3D (6 cores, 4.10 GHz) |
| **RAM** | 64 GB (63.1 GB usable) |
| **Motherboard** | ASUS System Product Name, BIOS 3057 |

## GPU

| Component | Detail |
|---|---|
| **Model** | NVIDIA GeForce RTX 5070 Ti |
| **Driver** | Game Ready Driver 596.36 (Apr 28, 2026) |
| **CUDA Cores** | 8960 |
| **Core Clock** | 2452 MHz |
| **VRAM** | 16 GB GDDR7 (16303 MB dedicated) |
| **Shared Memory** | 32329 MB |
| **Total Available** | 48632 MB |
| **Memory Bandwidth** | 896.064 GB/s |
| **Bus** | PCIe x16 Gen4 |
| **Resizable BAR** | Yes |
| **Direct3D** | 12_1 |

## Storage

| Drive | Type | Size | Label |
|---|---|---|---|
| C: | Samsung SSD 990 Evo Plus 1TB (NVMe) | 1 TB | Local Disk |
| G: | Samsung SSD 990 PRO with Heatsink 2TB (NVMe) | 2 TB | Giga |
| E: | G-TECH ArmorATD SCSI (External HDD) | Large | TheBigOne |

## Display

| Detail | Value |
|---|---|
| **Monitor** | LG UltraGear (3440×1440 @ 160 Hz) |
| **Technology** | G-SYNC Compatible |

## Implications For Unreal Development

### Strengths
- **RTX 5070 Ti (16 GB VRAM)** — Can run Lumen/Nanite, high-quality lighting, VR preview. 16 GB VRAM is enough for moderate-sized scenes with high-quality assets.
- **64 GB RAM** — Plenty for large scenes, editor stability, and running multiple tools (VS + UE + browser).
- **2 TB NVMe (G: drive)** — Fast drive ideal for the Unreal project, DDC (Derived Data Cache), and large asset storage. Put the project here.
- **6-core X3D CPU** — Good for game logic, physics, Blueprint compilation. Not the fastest for C++ compilation, but workable.

### Limits To Keep In Mind
- **6 cores / 12 threads** — C++ compilation in UE will be slower than on a 16-core chip. Expect longer iteration times on heavy code changes. Batch edits, avoid frequent full rebuilds.
- **16 GB VRAM** — Enough for Nanite + Lumen at 3440×1440, but pushing extreme photogrammetry or massive open worlds could hit limits. Keep the demo scene focused and contained.
- **C: is 1 TB** — Windows + tools may fill up. Keep the Unreal project and asset caches on G:.
- **E: is external HDD** — Slow. Only use for cold storage / backup / media. Never put the project or DDC here.

## Software Available

| Tool | Detail |
|---|---|
| **Visual Studio 2022 Community** | Installed at `C:\Program Files\Microsoft Visual Studio\2022\Community` |
| **VS Code** | Installed |
| **dotnet** | Available |
| **Epic Games Launcher** | Installed |
| **Unreal Engine** | 5.7.4 at `G:\Workspace\UnrealEngine\UE_5.7` |
| **Ollama** | v0.23.0, running locally at `localhost:11434` |

### Local AI Models

| Model | Size | Purpose | VRAM |
|---|---|---|---|
| `deepseek-r1:7b` | 4.7 GB | Investigative reasoning, chain-of-thought dot-connecting | ~5-6 GB GPU |
| `qwen2.5:7b` | 4.7 GB | General reasoning, structured JSON output | ~5-6 GB GPU |
| `moondream:latest` | ~900 MB | Vision analysis of evidence photos (CPU-runnable) | ~0-1 GB GPU |

**VRAM budget with Unreal + VR (target ~8 GB):** DeepSeek-R1 (~5-6 GB) + Unreal (~6-8 GB) needs careful balance. Use Moondream for vision (CPU) to save GPU VRAM. If VRAM is tight, drop to Qwen2.5 or run DeepSeek on CPU via Ollama.

## Project Location

- **Unreal project**: `G:\Workspace\University\Mumosa\Unreal\` (or similar on G: drive)
- **Derived Data Cache**: Keep on G: drive to avoid filling C:
