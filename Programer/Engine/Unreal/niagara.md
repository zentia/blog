---
sidebar_position: 1
---

# Niagara

## Emitter Properties

### Local Space

是否使用本地空间

## Particles.Position

|                                     | UE4                                                                                                                    |
| ----------------------------------- | ---------------------------------------------------------------------------------------------------------------------- |
| Particles.Position 类型             | 位置                                                                                                                   |
| 本地空间发射器(Local space emitter) | `Particles.Position`将随 Niagara 系统在游戏中的原点发生变化（无变化）                                                  |
| 世界空间发射器(World space emitter) | 激活后，`Particles.Position`将随系统的位置发生变化。对于小型坐标，这仍是游戏的原点。对于较大型坐标，这可能是任意数字。 |
