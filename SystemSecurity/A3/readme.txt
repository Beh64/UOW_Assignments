CSCI262 Assignment 3 - Email IDS

Prerequisites
-------------
Python 3.8 or later (tested on Python 3.12).
No third-party packages required; only standard-library modules are used
(sys, os, random, statistics, csv).

Files
-----
IDS.py            – Main program (all four phases)
Events.txt        – Event descriptor file (provided)
Stats.txt         – Baseline statistics file (provided)
readme.txt        – This file
Report.pdf        – Assignment report

Running the program
-------------------
python IDS.py Events.txt Stats.txt <Days>

    Events.txt  – path to the events descriptor file
    Stats.txt   – path to the baseline statistics file
    Days        – positive integer; number of days to generate for baseline

Example:
    python IDS.py Events.txt Stats.txt 30

The program will:
  1. Load and validate Events.txt and Stats.txt, reporting any issues.
  2. Print the loaded events and statistics to the console.
  3. Generate <Days> days of simulated activity; write activity_log.csv.
  4. Analyse the log (reads from disk); write daily_totals.csv and
     baseline_stats.csv.
  5. Enter the interactive Alert Engine loop:
       - Enter a path to a new stats file (same format as Stats.txt), OR
       - Type 'quit' to exit.
     Enter the number of days to simulate for that run.
     Each day is printed with its threshold, anomaly counter, and status.

Output files produced
---------------------
activity_log.csv         – Raw daily event values logged by the activity engine
daily_totals.csv         – Daily totals per event (written by the analysis engine)
baseline_stats.csv       – Computed mean/std per event (Stats.txt format)
alert_activity_log.csv   – Alert-phase daily event values
alert_daily_totals.csv   – Alert-phase daily totals (written by analysis engine)
alert_baseline_stats.csv – Alert-phase computed mean/std per event
