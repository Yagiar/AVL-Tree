#!/usr/bin/env python3
import argparse
import os
import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt


def main():
    parser = argparse.ArgumentParser(description="Визуализация распределений задержек из CSV")
    parser.add_argument("csv", help="Путь к CSV с колонками: op,scenario,latency_ns")
    parser.add_argument("--out-prefix", default="plots", help="Префикс имён файлов вывода")
    args = parser.parse_args()

    df = pd.read_csv(args.csv)
    required = {"op", "scenario", "latency_ns"}
    if not required.issubset(df.columns):
        raise SystemExit(f"CSV должен содержать колонки: {required}")

    # Создаём директорию для вывода, если указана
    out_dir = os.path.dirname(args.out_prefix)
    if out_dir:
        os.makedirs(out_dir, exist_ok=True)

    # В лог-шкале удобнее смотреть хвосты распределений
    df["latency_us"] = df["latency_ns"] / 1000.0

    sns.set(style="whitegrid")

    # Гистограммы по сценариям/операциям
    g = sns.displot(
        data=df,
        x="latency_us",
        col="scenario",
        row="op",
        kind="hist",
        bins=80,
        log_scale=(True, False),
        facet_kws={"sharex": False},
    )
    g.set_axis_labels("latency (us)", "count")
    plt.tight_layout()
    g.savefig(f"{args.out_prefix}_hist.png", dpi=150)

    # ECDF (кумулятивное распределение)
    plt.figure(figsize=(10, 6))
    sns.ecdfplot(data=df, x="latency_us", hue="scenario", stat="proportion")
    plt.xscale("log")
    plt.xlabel("latency (us)")
    plt.ylabel("ECDF")
    plt.tight_layout()
    plt.savefig(f"{args.out_prefix}_ecdf.png", dpi=150)

    # Boxplot по операциям и сценариям
    plt.figure(figsize=(10, 6))
    sns.boxplot(data=df, x="op", y="latency_us", hue="scenario")
    plt.yscale("log")
    plt.xlabel("operation")
    plt.ylabel("latency (us)")
    plt.tight_layout()
    plt.savefig(f"{args.out_prefix}_box.png", dpi=150)


if __name__ == "__main__":
    main()


