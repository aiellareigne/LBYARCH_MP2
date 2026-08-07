# Grayscale Image Representation Converter (Float to uint8)

## Project Overview
This project converts grayscale image matrices represented as single-precision floating-point values (`[0.0, 1.0]`) into 8-bit unsigned integer values (`[0, 255]`).

The performance-critical pixel conversion routine `imgCvtGrayFloatToInt()` is implemented in **x86-64 Assembly** using **scalar SIMD floating-point registers and instructions** (`XMM`, `movss`, `mulss`, `cvtss2si`).

---

## Benchmark & Performance Analysis

Execution time was measured across **30 runs** for each matrix size:

| Matrix Dimensions | Total Pixels | Average Execution Time (ms) |
|-------------------|--------------|-----------------------------|
| $10 \times 10$     | 100          |    0.0006 ms               |
| $100 \times 100$   | 10,000       |         0.0105 ms          |
| $1000 \times 1000$ | 1,000,000    |      0.6608 ms             |

### Performance Analysis
- **Scalability:** Execution time scales linearly $O(N)$ relative to pixel count $N = H \times W$.
- **Scalar Bottleneck:** The implementation processes one scalar float per iteration (`movss`/`mulss`). Execution time could be further accelerated by roughly $4\times$ by adopting packed vector SIMD instructions (`movups`, `mulps`, `cvtps2dq`).

---

## Output & Correctness Check (Screenshot)
<img width="647" height="467" alt="att qkZB3WDEKmvICp-xw-nTgDTPHgvN2eIq0tObuUy4ckY" src="https://github.com/user-attachments/assets/e3608ac3-31ac-4175-964d-563422a5cbdf" />

---

## Demonstration Video (GDrive)
https://drive.google.com/file/d/17eXABF3OgzEoKczr9avuGJVKLSTcfnLV/view?usp=sharing
