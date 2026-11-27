"""
CSCI262 Assignment 3 - Email System Event Modeler & Intrusion Detection System
Usage: python IDS.py Events.txt Stats.txt Days
"""

import sys
import os
import random
import statistics
import csv

# ------ DATA STRUCTURES ------

class Event:
    """Stores one event descriptor from Events.txt."""
    def __init__(self, name, kind, minimum, maximum, weight):
        self.name    = name     # str
        self.kind    = kind     # 'C' or 'D'
        self.minimum = minimum  # float
        self.maximum = maximum  # float or None
        self.weight  = weight   # int

    def __repr__(self):
        return (f"Event({self.name!r}, kind={self.kind}, "
                f"min={self.minimum}, max={self.maximum}, weight={self.weight})")


class EventStat:
    """Stores mean/std for one event (from Stats.txt or computed baseline)."""
    def __init__(self, name, mean, std):
        self.name = name
        self.mean = mean  # float
        self.std  = std   # float

    def __repr__(self):
        return f"EventStat({self.name!r}, mean={self.mean}, std={self.std})"


# ------ PHASE 1 INITIAL INPUT ------

def parse_events(path):
    """Parse Events.txt -> (list[Event], list[str] warnings)."""
    events   = []
    warnings = []
    try:
        with open(path) as f:
            lines = [l.strip() for l in f if l.strip()]
    except FileNotFoundError:
        print(f"Error: cannot open '{path}'.")
        sys.exit(1)

    if not lines:
        print(f"Error: '{path}' is empty.")
        sys.exit(1)

    try:
        n = int(lines[0])
    except ValueError:
        print(f"Error: first line of '{path}' must be an integer count.")
        sys.exit(1)

    actual = len(lines) - 1
    if actual != n:
        warnings.append(
            f"Events.txt header says {n} event(s) but {actual} line(s) follow.")

    for line in lines[1:]:
        parts = line.split(":")
        # Strip trailing empty field from trailing colon
        if parts and parts[-1] == "":
            parts = parts[:-1]

        if len(parts) < 5:
            warnings.append(f"Skipping malformed event line: {line!r}")
            continue

        name    = parts[0].strip()
        kind    = parts[1].strip().upper()
        raw_min = parts[2].strip()
        raw_max = parts[3].strip()

        try:
            weight = int(parts[4].strip())
        except ValueError:
            warnings.append(f"Event {name!r}: weight is not an integer; skipping.")
            continue

        try:
            minimum = float(raw_min) if raw_min else 0.0
        except ValueError:
            warnings.append(f"Event {name!r}: bad minimum value '{raw_min}'; using 0.")
            minimum = 0.0

        try:
            maximum = float(raw_max) if raw_max else None
        except ValueError:
            warnings.append(f"Event {name!r}: bad maximum value '{raw_max}'; ignoring.")
            maximum = None

        if kind not in ("C", "D"):
            warnings.append(f"Event {name!r}: unknown kind {kind!r} (expected C or D).")

        if weight <= 0:
            warnings.append(f"Event {name!r}: weight {weight} is not a positive integer.")

        events.append(Event(name, kind, minimum, maximum, weight))

    return events, warnings


def parse_stats(path, fatal=True):
    """Parse a Stats-format file -> (list[EventStat], list[str] warnings).
    If fatal=True (default, for initial startup), errors call sys.exit.
    If fatal=False (alert engine interactive loop), errors raise ValueError
    so the caller can catch and continue the loop.
    """
    stats    = []
    warnings = []
    try:
        with open(path) as f:
            lines = [l.strip() for l in f if l.strip()]
    except FileNotFoundError:
        msg = f"Error: cannot open '{path}'."
        if fatal: print(msg); sys.exit(1)
        raise ValueError(msg)

    if not lines:
        msg = f"Error: '{path}' is empty."
        if fatal: print(msg); sys.exit(1)
        raise ValueError(msg)

    try:
        n = int(lines[0])
    except ValueError:
        msg = f"Error: first line of '{path}' must be an integer count."
        if fatal: print(msg); sys.exit(1)
        raise ValueError(msg)

    actual = len(lines) - 1
    if actual != n:
        warnings.append(
            f"Stats file header says {n} event(s) but {actual} line(s) follow.")

    for line in lines[1:]:
        parts = line.split(":")
        if parts and parts[-1] == "":
            parts = parts[:-1]

        if len(parts) < 3:
            warnings.append(f"Skipping malformed stats line: {line!r}")
            continue

        name = parts[0].strip()
        try:
            mean = float(parts[1].strip())
            std  = float(parts[2].strip())
        except ValueError:
            warnings.append(f"Stats for {name!r}: non-numeric mean/std; skipping.")
            continue

        if std <= 0:
            warnings.append(
                f"Stats for {name!r}: std={std} is non-positive (must be > 0).")

        stats.append(EventStat(name, mean, std))

    return stats, warnings


def check_consistency(events, stats):
    """Cross-check Events list vs Stats list; return list of warning strings."""
    warnings = []
    e_names  = [e.name for e in events]
    s_names  = [s.name for s in stats]

    if len(events) != len(stats):
        warnings.append(
            f"Event count mismatch: Events.txt has {len(events)}, "
            f"Stats.txt has {len(stats)}.")

    for n in e_names:
        if n not in s_names:
            warnings.append(
                f"Event '{n}' is in Events.txt but has no entry in Stats.txt.")
    for n in s_names:
        if n not in e_names:
            warnings.append(
                f"Event '{n}' is in Stats.txt but has no entry in Events.txt.")

    s_map = {s.name: s for s in stats}
    for ev in events:
        if ev.name not in s_map:
            continue
        st = s_map[ev.name]

        # Mean vs stated range
        if st.mean < ev.minimum:
            warnings.append(
                f"Event '{ev.name}': mean ({st.mean}) is below "
                f"declared minimum ({ev.minimum}).")
        if ev.maximum is not None and st.mean > ev.maximum:
            warnings.append(
                f"Event '{ev.name}': mean ({st.mean}) exceeds "
                f"declared maximum ({ev.maximum}).")

        # 3-sigma practical range vs bounds
        low3 = st.mean - 3 * st.std
        if low3 < ev.minimum:
            warnings.append(
                f"Event '{ev.name}': mean-3*std = {low3:.2f} < minimum {ev.minimum} "
                f"(values will be clamped at minimum).")
        if ev.maximum is not None:
            high3 = st.mean + 3 * st.std
            if high3 > ev.maximum:
                warnings.append(
                    f"Event '{ev.name}': mean+3*std = {high3:.2f} > maximum "
                    f"{ev.maximum} (values will be clamped at maximum).")

    return warnings


def print_loaded(events, stats):
    """Pretty-print loaded events and statistics to console."""
    print("\n=== Events loaded ===")
    print(f"  {'Name':<22} {'Type':<6} {'Min':<8} {'Max':<10} {'Weight'}")
    print("  " + "-" * 58)
    for e in events:
        mx = f"{e.maximum}" if e.maximum is not None else "(none)"
        print(f"  {e.name:<22} {e.kind:<6} {e.minimum:<8} {mx:<10} {e.weight}")

    print("\n=== Statistics loaded ===")
    print(f"  {'Name':<22} {'Mean':<12} {'Std Dev'}")
    print("  " + "-" * 44)
    for s in stats:
        print(f"  {s.name:<22} {s.mean:<12} {s.std}")


# ------ PHASE 2 ACTIVITY ENGINE ------

LOG_FILE = "activity_log.csv"


def sample_event_value(ev, st):
    """
    Draw one day's total for event ev from N(mean, std).

    Discrete ('D'):
      - Round Gaussian sample to nearest integer (models a whole-number count).
      - Clamp to [minimum, maximum]; maximum defaults to unbounded if not set.

    Continuous ('C'):
      - Use raw float sample, clamped to [minimum, maximum].
      - Round to 2 decimal places as required by the spec.
    """
    value = random.gauss(st.mean, st.std)

    if ev.kind == "D":
        value = round(value)                         # integer count
        value = max(int(ev.minimum), int(value))     # floor clamp
        if ev.maximum is not None:
            value = min(int(ev.maximum), value)      # ceiling clamp
    else:
        value = max(ev.minimum, value)
        if ev.maximum is not None:
            value = min(ev.maximum, value)
        value = round(value, 2)                      # 2 decimal places

    return value


def run_activity_engine(events, stats, days, log_path=LOG_FILE):
    """
    Simulate 'days' days of activity.

    Writes a human-readable CSV log (log_path).
    Returns list-of-dicts [{event_name: value, 'day': int}, ...].
    """
    s_map      = {s.name: s for s in stats}
    daily_data = []

    # Write CSV header
    with open(log_path, "w", newline="") as f:
        writer = csv.writer(f, lineterminator="\n")
        writer.writerow(["day"] + [e.name for e in events])

    print(f"\n[Activity Engine] Generating {days} day(s) of events "
          f"(log: '{log_path}')...")

    update_every = max(1, days // 5)

    for day in range(1, days + 1):
        row = {"day": day}
        for ev in events:
            if ev.name in s_map:
                row[ev.name] = sample_event_value(ev, s_map[ev.name])
            else:
                row[ev.name] = 0  # event has no stats; use 0

        daily_data.append(row)

        # Append one CSV row
        with open(log_path, "a", newline="") as f:
            writer = csv.writer(f, lineterminator="\n")
            writer.writerow([day] + [row[e.name] for e in events])

        if day % update_every == 0 or day == days:
            print(f"  ... day {day}/{days} complete.", flush=True)

    print(f"[Activity Engine] Done. {days} day(s) written to '{log_path}'.")
    return daily_data


def read_log(log_path, events):
    """
    Read activity log CSV back from disk -> list-of-dicts.
    Supports the spec requirement that the analysis engine can read from the log file.
    """
    daily_data = []
    with open(log_path, newline="") as f:
        reader = csv.DictReader(f)
        for row in reader:
            entry = {"day": int(row["day"])}
            for ev in events:
                raw = row.get(ev.name, "0")
                entry[ev.name] = int(raw) if ev.kind == "D" else float(raw)
            daily_data.append(entry)
    return daily_data

# ------ PHASE 3 ANALYSIS ENGINE ------

BASELINE_FILE    = "baseline_stats.csv"
DAILY_TOTALS_FILE = "daily_totals.csv"

def run_analysis_engine(events, daily_data, stats_path=BASELINE_FILE,
                        totals_path=DAILY_TOTALS_FILE):
    """
    Compute per-event mean and sample standard deviation from daily_data.
    Writes:
      - totals_path : daily totals CSV (one row per day, same columns as activity log)
      - stats_path  : per-event mean and std (same format as Stats.txt)
    Returns dict {event_name: (mean, std)}.
    """
    print(f"\n[Analysis Engine] Computing statistics from "
          f"{len(daily_data)} day(s) of data...")

    # 1. Save daily totals to a dedicated file
    with open(totals_path, "w", newline="") as f:
        writer = csv.writer(f, lineterminator="\n")
        writer.writerow(["day"] + [ev.name for ev in events])
        for row in daily_data:
            writer.writerow([row["day"]] + [row.get(ev.name, 0) for ev in events])
    print(f"[Analysis Engine] Daily totals written to '{totals_path}'.")

    # 2. Compute mean and std for each event
    computed = {}
    for ev in events:
        vals = [row[ev.name] for row in daily_data if ev.name in row]

        if len(vals) == 0:
            mean_v, std_v = 0.0, 0.0
        elif len(vals) == 1:
            mean_v, std_v = float(vals[0]), 0.0
        else:
            mean_v = statistics.mean(vals)
            std_v  = statistics.stdev(vals)   # sample std (N-1)

        computed[ev.name] = (round(mean_v, 4), round(std_v, 4))

    # 3. Save per-event stats in Stats.txt format (can be re-used as input)
    with open(stats_path, "w") as f:
        f.write(f"{len(events)}\n")
        for ev in events:
            m, s = computed[ev.name]
            f.write(f"{ev.name}:{m}:{s}:\n")

    print(f"[Analysis Engine] Baseline statistics written to '{stats_path}'.")
    print(f"\n  {'Name':<22} {'Mean':<14} {'Std Dev'}")
    print("  " + "-" * 50)
    for ev in events:
        m, s = computed[ev.name]
        print(f"  {ev.name:<22} {m:<14} {s}")

    return computed


# ------ PHASE 4 ALERT ENGINE ------

ALERT_LOG_FILE    = "alert_activity_log.csv"
ALERT_STATS_FILE  = "alert_baseline_stats.csv"
ALERT_TOTALS_FILE = "alert_daily_totals.csv"


def compute_anomaly_counter(row, events, baseline):
    """
    For one day's data (row dict), compute the anomaly counter:
      counter = sum over all events of:
                  (|daily_value - baseline_mean| / baseline_std) * weight

    If baseline_std == 0 for an event:
      - If the daily value equals the baseline mean: contribution = 0
        (no deviation, no anomaly from this event).
      - Otherwise: the deviation is theoretically infinite; we cap it at
        a large sentinel (100 * weight) to prevent division-by-zero while
        still ensuring the day is flagged.
    """
    counter = 0.0
    for ev in events:
        bl_mean, bl_std = baseline.get(ev.name, (0.0, 0.0))
        val = row.get(ev.name, 0)
        diff = abs(val - bl_mean)

        if bl_std == 0.0:
            # Zero std: any deviation is anomalous; treat as 100 std devs away
            deviation = 0.0 if diff == 0.0 else 100.0
        else:
            deviation = diff / bl_std

        counter += deviation * ev.weight

    return round(counter, 2)


def run_alert_engine(events, baseline):
    """
    Interactive alert loop.
    baseline: dict {event_name: (mean, std)} from the baseline analysis engine.
    """
    total_weight = sum(e.weight for e in events)
    threshold    = 2 * total_weight

    print(f"\n[Alert Engine] Ready.")
    print(f"  Baseline weights: "
          + ", ".join(f"{e.name}={e.weight}" for e in events))
    print(f"  Sum of weights = {total_weight}  ->  threshold = 2 x {total_weight} = {threshold}")

    while True:
        print("\n" + "=" * 62)
        user_in = input(
            "Alert Engine: enter path to new stats file, or 'quit' to exit: "
        ).strip()

        if user_in.lower() in ("quit", "q", "exit"):
            print("[Alert Engine] Exiting.")
            break

        stats_path = user_in
        if not os.path.isfile(stats_path):
            print(f"  Error: file '{stats_path}' not found. Please try again.")
            continue

        try:
            new_stats, sw = parse_stats(stats_path, fatal=False)
        except ValueError as e:
            print(f"  {e}")
            continue
        if sw:
            print("  Warnings from stats file:")
            for w in sw:
                print(f"    [!]  {w}")
        if not new_stats:
            print("  Error: no valid stats loaded from that file.")
            continue

        # Validate days input
        while True:
            try:
                raw = input("  Number of days to simulate: ").strip()
                alert_days = int(raw)
                if alert_days <= 0:
                    print("  Days must be a positive integer.")
                    continue
                break
            except ValueError:
                print("  Invalid input; please enter a positive integer.")

        # Step 3 of spec: generate activity data with the NEW stats
        alert_data = run_activity_engine(
            events, new_stats, alert_days, log_path=ALERT_LOG_FILE
        )

        # Step 3 (cont.): run analysis engine on alert data (spec requires this;
        #                 produces daily totals file and per-event stats for the period)
        print("\n[Analysis Engine] Event generation complete. "
              "Starting analysis phase.")
        run_analysis_engine(events, alert_data, stats_path=ALERT_STATS_FILE,
                            totals_path=ALERT_TOTALS_FILE)

        # Step 4: anomaly detection against ORIGINAL baseline
        print(f"\n[Alert Engine] Anomaly detection results ({alert_days} day(s)):")
        print(f"  (Threshold = 2 x sum_of_weights = 2 x {total_weight} = {threshold})")
        print(f"\n  {'Day':<6} {'Threshold':>10} {'Anomaly Counter':>16}  Status")
        print("  " + "-" * 52)

        flagged = 0
        for row in alert_data:
            day     = row["day"]
            counter = compute_anomaly_counter(row, events, baseline)
            if counter >= threshold:
                status = "FLAGGED  *** ALERT ***"
                flagged += 1
            else:
                status = "okay"
            print(f"  {day:<6} {threshold:>10} {counter:>16.2f}  {status}")

        print(f"\n  Summary: {flagged}/{alert_days} day(s) flagged.")

# MAIN

def main():
    if len(sys.argv) != 4:
        print("Usage: python IDS.py Events.txt Stats.txt Days")
        sys.exit(1)

    events_path = sys.argv[1]
    stats_path  = sys.argv[2]

    try:
        days = int(sys.argv[3])
        if days <= 0:
            raise ValueError
    except ValueError:
        print("Error: Days must be a positive integer.")
        sys.exit(1)

    # ------ Phase 1: Initial Input ------
    print("=" * 62)
    print("CSCI262 IDS - Email System Event Modeler")
    print("=" * 62)
    print(f"  Events file : {events_path}")
    print(f"  Stats file  : {stats_path}")
    print(f"  Baseline days: {days}")

    events, ew = parse_events(events_path)
    stats,  sw = parse_stats(stats_path)

    any_warning = False
    if ew:
        print("\n[Input] Warnings from Events.txt:")
        for w in ew:
            print(f"  [!]  {w}")
        any_warning = True
    if sw:
        print("\n[Input] Warnings from Stats.txt:")
        for w in sw:
            print(f"  [!]  {w}")
        any_warning = True

    cw = check_consistency(events, stats)
    if cw:
        print("\n[Input] Consistency warnings:")
        for w in cw:
            print(f"  [!]  {w}")
        any_warning = True

    if not any_warning:
        print("\n[Input] Files loaded successfully - no issues detected.")

    if not events:
        print("Error: no valid events loaded. Cannot continue.")
        sys.exit(1)

    print_loaded(events, stats)

    # -- Phase 2: Activity Engine (baseline) ------------------------------
    print(f"\n[Activity Engine] Starting baseline generation ({days} day(s)).")
    daily_data = run_activity_engine(events, stats, days, log_path=LOG_FILE)

    # -- Phase 3: Analysis Engine ------------------------------------------
    print("\n[Analysis Engine] Event generation complete. Starting analysis phase.")
    # Also demonstrate log-file read-back (spec: engine "can read from the log file")
    disk_data = read_log(LOG_FILE, events)
    baseline  = run_analysis_engine(events, disk_data, stats_path=BASELINE_FILE)

    # -- Phase 4: Alert Engine ---------------------------------------------
    run_alert_engine(events, baseline)


if __name__ == "__main__":
    main()
