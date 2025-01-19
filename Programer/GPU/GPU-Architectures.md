---
title: GPU Architectures
mathjax: true
date: 2019-09-05 15:05:02
categories:
- GPU
---
# Graphics pipeline

{% asset_img 1.jpg %}
- "Logical" pipeline described in OGL/DX specification
 - It's an abstraction
- At physical level things are very different
 - As long as specs are met there's no problem
- Today we will look at things from a slightly closer POV.

# Anatomy of a GPU

- Extremely Parallel machine 
 - Thousands of "threads" in flight
 - But
  - Limited flow control
  - Some threads shares program counter
  - No Inter process communication
- Memory bandwidth is very high（内存带宽很高）
 - Hundreds of GB/s
 - But
  - Very high latency（延迟很高）
   - Thousand of cycles
  - Latency hidind mechanism necessary
