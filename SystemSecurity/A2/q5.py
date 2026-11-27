"""
q5.py - Assignment 2, Question 5
Simulates the spread of Worm X and Counter-worm W, with and without mutation.

Rules:
  Worm X  : spreads to 1 new uninfected host per infected host, every hour
             (i.e. X doubles each hour on integer time steps)
  Worm W  : deployed at t=6.5 on one X-infected host; spreads to 2 X-infected
             hosts every half-hour (W triples every half-hour: W -> 3W)
  Mutation: at t=9 Worm X mutates to spread to 3 new hosts per hour
             (X quadruples each hour on integer time steps)
"""

import matplotlib.pyplot as plt

# Q5a  Pure Worm X spread (no counter-worm), 24 hours

print("=" * 55)
print("Q5a – Worm X growth (no counter-worm), t = 0..24")
print("=" * 55)
print(f"{'Hour':>5}  {'Infected':>14}")
print("-" * 25)
x_only = {}
for t in range(25):
    x_only[t] = 2 ** t
    print(f"{t:>5}  {x_only[t]:>14,}")

# Q5b  Worm X vs Counter-worm W (half-hour steps)

print()
print("=" * 65)
print("Q5b – Worm X vs Counter-worm W (half-hour steps, no mutation)")
print("=" * 65)

# Simulate at 0.5-hour resolution from t=0 to t=20
# Before t=6.5: only X exists (doubles at integer hours)
# At t=6.5:     W=1 deployed, X reduced by 1
# Each integer hour (t >= 7):  X doubles (X -> 2X)
# Each half-hour  (t >= 7):    W spreads: X -= 2W, W += 2W  (W triples)

time_steps = [t / 2 for t in range(0, 41)]  # 0.0, 0.5, …, 20.0

X = 1   # initial infections
W = 0

records = []

for t in time_steps:
    event = ""

    if t < 6.5:
        # X doubles on integer hours
        if t == int(t) and t > 0:
            X *= 2
            event = "X spreads (×2)"
    elif t == 6.5:
        W = 1
        X -= 1   # W infects one X host
        event = "W deployed"
    else:
        # Integer hour: X doubles first
        if t == int(t):
            X *= 2
            event = "X spreads (×2)"
        # Half-hour (and t==6.5 already handled): W spreads
        if t != int(t):
            spread = 2 * W
            actual_spread = min(spread, X)   # can't convert more than exist
            X -= actual_spread
            W += actual_spread
            event = f"W spreads (×3)" if actual_spread == spread else f"W spreads (limited, -{actual_spread})"

    records.append((t, X, W, event))

    if X == 0 and W > 0:
        print(f"{'Time':>6}  {'X-infected':>12}  {'W-infected':>12}  Event")
        print("-" * 65)
        for r in records:
            print(f"{r[0]:>6.1f}  {r[1]:>12,}  {r[2]:>12,}  {r[3]}")
        print(f"\n  -> X eradicated at t = {t}")
        break


# Q5d  Mutation at t=9: X spreads to 3 new hosts/hour (quadruples)

print()
print("=" * 65)
print("Q5d – Worm X (mutated at t=9) vs Counter-worm W")
print("=" * 65)

X_m = 1
W_m = 0
records_mut = []
MUTATION_TIME = 9.0

for t in time_steps:
    event = ""

    if t < 6.5:
        if t == int(t) and t > 0:
            X_m *= 2
            event = "X spreads (×2)"
    elif t == 6.5:
        W_m = 1
        X_m -= 1
        event = "W deployed"
    else:
        if t == int(t):
            multiplier = 4 if t >= MUTATION_TIME else 2
            X_m *= multiplier
            label = "×4 (mutated)" if t >= MUTATION_TIME else "×2"
            event = f"X spreads ({label})"
        if t != int(t):
            spread = 2 * W_m
            actual_spread = min(spread, X_m)
            X_m -= actual_spread
            W_m += actual_spread
            event = "W spreads (×3)"

    records_mut.append((t, X_m, W_m, event))

    if X_m == 0 and W_m > 0:
        break   # eradicated

print(f"{'Time':>6}  {'X-infected':>14}  {'W-infected':>12}  Event")
print("-" * 70)
# Print only from t=6.5 onwards for readability
for r in records_mut:
    if r[0] >= 6.5:
        print(f"{r[0]:>6.1f}  {r[1]:>14,}  {r[2]:>12,}  {r[3]}")
    if r[0] >= 17:   # stop printing after t=17 to keep output manageable
        print("  ... (X continues unbounded growth)")
        break

x_eradicated = any(r[1] == 0 for r in records_mut)
print(f"\n  -> X {'eradicated' if x_eradicated else 'NOT eradicated (unbounded growth)'}")


# Plots

fig, axes = plt.subplots(1, 2, figsize=(15, 6))

# --- Plot 1: No mutation ---
ts_b  = [r[0] for r in records if r[0] >= 6.0]
xs_b  = [r[1] for r in records if r[0] >= 6.0]
ws_b  = [r[2] for r in records if r[0] >= 6.0]
ts_a  = list(range(7))   # t=0..6 for pure X
xs_a  = [x_only[t] for t in ts_a]

ax = axes[0]
ax.plot(ts_a, xs_a, "b-o", markersize=4, label="Worm X (no W yet)")
ax.plot(ts_b, xs_b, "r-o", markersize=4, label="Worm X (with W)")
ax.plot(ts_b, ws_b, "g-s", markersize=4, label="Counter-worm W")
ax.axvline(6.5, color="gray", linestyle=":", label="W deployed (t=6.5)")

# Mark eradication
for r in records:
    if r[1] == 0:
        ax.axvline(r[0], color="black", linestyle="--", label=f"X=0 at t={r[0]}")
        break

ax.set_title("Q5b – Worm X vs Counter-worm W (no mutation)")
ax.set_xlabel("Time (hours)")
ax.set_ylabel("Number of infected hosts")
ax.legend(fontsize=8)
ax.grid(True, linestyle="--", alpha=0.4)
ax.set_xlim(0, 17)

# --- Plot 2: With mutation ---
ts_m = [r[0] for r in records_mut if r[0] >= 6.0 and r[0] <= 17]
xs_m = [r[1] for r in records_mut if r[0] >= 6.0 and r[0] <= 17]
ws_m = [r[2] for r in records_mut if r[0] >= 6.0 and r[0] <= 17]

ax2 = axes[1]
ax2.plot(ts_m, xs_m, "r-o", markersize=4, label="Worm X (mutated at t=9)")
ax2.plot(ts_m, ws_m, "g-s", markersize=4, label="Counter-worm W")
ax2.axvline(6.5, color="gray", linestyle=":", label="W deployed (t=6.5)")
ax2.axvline(9.0, color="purple", linestyle=":", label="X mutates (t=9)")

ax2.set_title("Q5d – Worm X (mutated) vs Counter-worm W")
ax2.set_xlabel("Time (hours)")
ax2.set_ylabel("Number of infected hosts")
ax2.legend(fontsize=8)
ax2.grid(True, linestyle="--", alpha=0.4)
ax2.set_xlim(6, 17)

plt.tight_layout()
plt.savefig("q5_worm_spread.png", dpi=150)
plt.show()
print("\nPlot saved as q5_worm_spread.png")
