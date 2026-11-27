"""
q1.py - Assignment 2, Question 1
Computes and visualises the probability distributions for Puzzle A and Puzzle B.

Puzzle A: 1 sub-puzzle, k=6  ->  N = 2^6 = 64 candidates, uniform distribution
Puzzle B: 8 sub-puzzles, k=3 ->  N = 2^3 = 8 candidates each, distribution of sum T
"""

import math
import matplotlib.pyplot as plt


# Puzzle A  (k=6, 1 sub-puzzle)

k_A = 6
N_A = 2 ** k_A          # 64 candidates
outcomes_A = list(range(1, N_A + 1))
prob_A = 1 / N_A

print("=" * 55)
print("PUZZLE A  (k=6, 1 sub-puzzle, N=64)")
print("=" * 55)
print(f"N = 2^{k_A} = {N_A}")
print(f"P(h = j) = 1/{N_A} = {prob_A:.6f}  for j = 1 .. {N_A}")

mean_A = (N_A + 1) / 2
var_A  = (N_A ** 2 - 1) / 12
sd_A   = math.sqrt(var_A)
print(f"\nE[h]  = (N+1)/2 = {mean_A}")
print(f"Var   = (N²-1)/12 = {var_A:.4f}")
print(f"SD    = {sd_A:.4f}")


# Puzzle B  (k=3, 8 sub-puzzles)
# Dynamic programming convolution

k_B       = 3
N_B       = 2 ** k_B      # 8 candidates per sub-puzzle
n_puzzles = 8
total_outcomes = N_B ** n_puzzles   # 8^8 = 16,777,216

# dp[s] = number of ways to achieve sum s across sub-puzzles seen so far
dp = {0: 1}
for _ in range(n_puzzles):
    new_dp = {}
    for s, cnt in dp.items():
        for face in range(1, N_B + 1):
            new_s = s + face
            new_dp[new_s] = new_dp.get(new_s, 0) + cnt
    dp = new_dp

T_values = sorted(dp.keys())
probs_B  = {t: dp[t] / total_outcomes for t in T_values}

print("\n" + "=" * 55)
print("PUZZLE B  (k=3, 8 sub-puzzles, N per puzzle=8)")
print("=" * 55)
print(f"Total outcomes = 8^8 = {total_outcomes:,}")
print(f"\n{'T':>5}  {'Count':>12}  {'P(T=t)':>12}")
print("-" * 35)
for t in T_values:
    print(f"{t:>5}  {dp[t]:>12,}  {probs_B[t]:>12.8f}")

mean_B = sum(t * probs_B[t] for t in T_values)
var_B  = sum(((t - mean_B) ** 2) * probs_B[t] for t in T_values)
sd_B   = math.sqrt(var_B)
print(f"\nE[T]  = {mean_B:.4f}  (expected: {n_puzzles * (N_B + 1) / 2})")
print(f"Var   = {var_B:.4f}  (expected: {n_puzzles * (N_B ** 2 - 1) / 12})")
print(f"SD    = {sd_B:.4f}")



fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(14, 5))

# Figure 1 – Puzzle A
ax1.bar(outcomes_A, [prob_A] * N_A, color="steelblue", edgecolor="none", width=0.8)
ax1.axvline(mean_A, color="red", linestyle="--", linewidth=1.5, label=f"Mean = {mean_A}")
ax1.set_title("Puzzle A – Uniform distribution (k=6, N=64)")
ax1.set_xlabel("Number of hashes (h)")
ax1.set_ylabel("Probability P(h)")
ax1.set_xlim(0, N_A + 1)
ax1.set_ylim(0, 0.025)
ax1.legend()
ax1.grid(axis="y", linestyle="--", alpha=0.4)

# Figure 2 – Puzzle B
ax2.bar(T_values, [probs_B[t] for t in T_values],
        color="darkorange", edgecolor="none", width=0.8)
ax2.axvline(mean_B, color="red", linestyle="--", linewidth=1.5, label=f"Mean = {mean_B:.1f}")
ax2.set_title("Puzzle B – Sum of 8 uniform{1..8} sub-puzzles (k=3)")
ax2.set_xlabel("Total hashes (T)")
ax2.set_ylabel("Probability P(T=t)")
ax2.legend()
ax2.grid(axis="y", linestyle="--", alpha=0.4)

plt.tight_layout()
plt.savefig("q1_distributions.png", dpi=150)
plt.show()
print("\nPlot saved as q1_distributions.png")
