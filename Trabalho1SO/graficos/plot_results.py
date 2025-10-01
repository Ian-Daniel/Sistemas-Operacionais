import pandas as pd
import matplotlib.pyplot as plt

# ===============================
# Gráfico E1 - Tamanho da matriz
# ===============================
def plot_e1(csv_file="result_e1.csv"):
    df = pd.read_csv(csv_file)

    plt.figure(figsize=(8,5))
    plt.plot(df["matrix_size"], df["sequencial"], marker="o", label="Sequencial")
    plt.plot(df["matrix_size"], df["threads"], marker="o", label="Threads")
    plt.plot(df["matrix_size"], df["processos"], marker="o", label="Processos")

    plt.xlabel("Tamanho da matriz (n x n)")
    plt.ylabel("Tempo médio (s)")
    plt.title("E1 - Tempo médio vs Tamanho da matriz")
    plt.legend()
    plt.grid(True)
    plt.tight_layout()
    plt.savefig("grafico_e1.png")
    plt.show()


# ===============================
# Gráfico E2 - Número de threads/processos
# ===============================
def plot_e2(csv_file="result_e2.csv"):
    df = pd.read_csv(csv_file)

    plt.figure(figsize=(8,5))
    plt.plot(df["P"], df["threads"], marker="o", label="Threads")
    plt.plot(df["P"], df["processos"], marker="o", label="Processos")

    plt.xlabel("Número de Threads/Processos (P)")
    plt.ylabel("Tempo médio (s)")
    plt.title("E2 - Tempo médio vs P")
    plt.legend()
    plt.grid(True)
    plt.tight_layout()
    plt.savefig("grafico_e2.png")
    plt.show()


if __name__ == "__main__":
    print("Gerando gráficos...")
    plot_e1()
    plot_e2()
    print("Gráficos salvos como grafico_e1.png e grafico_e2.png")