#!/usr/bin/env python3

import csv
import os
from collections import defaultdict
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
import numpy as np

DATA_DIR = "data"
PLOTS_DIR = "plots"
CSV_FILES = {
    "random": "results_random.csv",
    "reverse_sorted": "results_reverse.csv",
    "nearly_sorted": "results_nearly_sorted.csv",
    "shared_prefix": "results_shared_prefix.csv",
}

ALGO_LABELS = {
    "std_quick": "Standard QuickSort",
    "std_merge": "Standard MergeSort",
    "string_quick": "String QuickSort (ternary)",
    "string_merge": "String MergeSort (LCP)",
    "msd_radix": "MSD Radix Sort",
    "msd_radix_quick": "MSD Radix+Quick Sort",
}

COLORS = {
    "std_quick": "#e74c3c",
    "std_merge": "#e67e22",
    "string_quick": "#2ecc71",
    "string_merge": "#3498db",
    "msd_radix": "#9b59b6",
    "msd_radix_quick": "#1abc9c",
}

LINE_STYLES = {
    "std_quick": "-",
    "std_merge": "-",
    "string_quick": "--",
    "string_merge": "--",
    "msd_radix": ":",
    "msd_radix_quick": "-.",
}

MARKERS = {
    "std_quick": "o",
    "std_merge": "s",
    "string_quick": "D",
    "string_merge": "^",
    "msd_radix": "v",
    "msd_radix_quick": "p",
}

TITLE_NAMES = {
    "random": "Random arrays",
    "reverse_sorted": "Reverse-sorted arrays",
    "nearly_sorted": "Nearly-sorted arrays",
    "shared_prefix": "Shared-prefix arrays",
}


def load_csv(filename: str) -> dict[str, dict[int, tuple[float, int]]]:
    data: dict[str, dict[int, tuple[float, int]]] = defaultdict(dict)
    with open(filename, "r") as f:
        reader = csv.DictReader(f)
        for row in reader:
            algo = row["algorithm"]
            size = int(row["size"])
            time_us = float(row["time_us"])
            comps = int(row["comparisons"])
            data[algo][size] = (time_us, comps)
    return data


def plot_time(data: dict, dataset_name: str, output_path: str):
    fig, ax = plt.subplots(figsize=(10, 6))

    for algo in ["std_quick", "std_merge", "string_quick", "string_merge",
                  "msd_radix", "msd_radix_quick"]:
        if algo not in data:
            continue
        sizes = sorted(data[algo].keys())
        times = [data[algo][s][0] for s in sizes]
        ax.plot(sizes, times,
                label=ALGO_LABELS[algo],
                color=COLORS[algo],
                linestyle=LINE_STYLES[algo],
                marker=MARKERS[algo],
                markersize=4,
                linewidth=1.5,
                markevery=max(1, len(sizes) // 10))

    ax.set_xlabel("Array size (n)", fontsize=12)
    ax.set_ylabel("Time (μs)", fontsize=12)
    ax.set_title(f"Sorting time — {TITLE_NAMES[dataset_name]}", fontsize=14)
    ax.legend(fontsize=9, framealpha=0.9)
    ax.grid(True, alpha=0.3)
    fig.tight_layout()
    fig.savefig(output_path, dpi=150)
    plt.close(fig)


def plot_comparisons(data: dict, dataset_name: str, output_path: str):
    fig, ax = plt.subplots(figsize=(10, 6))

    for algo in ["std_quick", "std_merge", "string_quick", "string_merge",
                  "msd_radix", "msd_radix_quick"]:
        if algo not in data:
            continue
        sizes = sorted(data[algo].keys())
        comps = [data[algo][s][1] for s in sizes]
        ax.plot(sizes, comps,
                label=ALGO_LABELS[algo],
                color=COLORS[algo],
                linestyle=LINE_STYLES[algo],
                marker=MARKERS[algo],
                markersize=4,
                linewidth=1.5,
                markevery=max(1, len(sizes) // 10))

    ax.set_xlabel("Array size (n)", fontsize=12)
    ax.set_ylabel("Character comparisons", fontsize=12)
    ax.set_title(f"Character comparisons — {TITLE_NAMES[dataset_name]}", fontsize=14)
    ax.legend(fontsize=9, framealpha=0.9)
    ax.grid(True, alpha=0.3)
    fig.tight_layout()
    fig.savefig(output_path, dpi=150)
    plt.close(fig)


def plot_summary(data: dict, output_path: str):
    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(16, 6))

    datasets = list(data.keys())
    algos = ["std_quick", "std_merge", "string_quick", "string_merge",
             "msd_radix", "msd_radix_quick"]

    x = np.arange(len(datasets))
    width = 0.12
    n3000 = 3000

    for i, algo in enumerate(algos):
        times = []
        comps = []
        for ds in datasets:
            if algo in data[ds] and n3000 in data[ds][algo]:
                t, c = data[ds][algo][n3000]
                times.append(t)
                comps.append(c)
            else:
                times.append(0)
                comps.append(0)

        offset = (i - len(algos) / 2 + 0.5) * width
        bars1 = ax1.bar(x + offset, times, width, label=ALGO_LABELS[algo],
                        color=COLORS[algo], alpha=0.85)
        ax2.bar(x + offset, comps, width, label=ALGO_LABELS[algo],
                color=COLORS[algo], alpha=0.85)

    ax1.set_xticks(x)
    ax1.set_xticklabels([TITLE_NAMES[d] for d in datasets], fontsize=10)
    ax1.set_ylabel("Time (μs)", fontsize=12)
    ax1.set_title("Sorting time at n=3000", fontsize=14)
    ax1.legend(fontsize=7, framealpha=0.9)
    ax1.grid(True, alpha=0.3, axis='y')

    ax2.set_xticks(x)
    ax2.set_xticklabels([TITLE_NAMES[d] for d in datasets], fontsize=10)
    ax2.set_ylabel("Character comparisons", fontsize=12)
    ax2.set_title("Character comparisons at n=3000", fontsize=14)
    ax2.legend(fontsize=7, framealpha=0.9)
    ax2.grid(True, alpha=0.3, axis='y')

    fig.suptitle("String Sorting Algorithms — Summary (n=3000)", fontsize=16, y=1.01)
    fig.tight_layout()
    fig.savefig(output_path, dpi=150, bbox_inches="tight")
    plt.close(fig)


def main():
    os.makedirs(PLOTS_DIR, exist_ok=True)

    all_data: dict[str, dict] = {}

    for ds_name, csv_file in CSV_FILES.items():
        path = os.path.join(DATA_DIR, csv_file)
        if not os.path.exists(path):
            print(f"WARNING: {path} not found, skipping")
            continue
        data = load_csv(path)
        all_data[ds_name] = data

        plot_time(data, ds_name, os.path.join(PLOTS_DIR, f"time_{ds_name}.png"))
        plot_comparisons(data, ds_name, os.path.join(PLOTS_DIR, f"comps_{ds_name}.png"))
        print(f"[plot] {ds_name}: time + comps graphs saved")

    if all_data:
        plot_summary(all_data, os.path.join(PLOTS_DIR, "summary.png"))
        print("[plot] summary graph saved")

    print(f"\nAll graphs saved to {PLOTS_DIR}/")


if __name__ == "__main__":
    main()
