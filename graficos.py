import csv
import matplotlib.pyplot as plt

ARQUIVO_CSV = "resultados.csv"

with open(ARQUIVO_CSV, newline="", encoding="utf-8") as arquivo:
    dados = list(csv.DictReader(arquivo))

if not dados:
    raise ValueError(f"O arquivo {ARQUIVO_CSV} está vazio ou sem dados.")

# Mantém um resultado por quantidade de threads.
# Se houver uma coluna 'versao', prefere a linha serial como referência.
dados_por_threads = {}

for linha in dados:
    quantidade = int(linha["threads"].strip())
    versao = linha.get("versao", "").strip().lower()

    if quantidade not in dados_por_threads or versao == "serial":
        dados_por_threads[quantidade] = linha

dados = [dados_por_threads[t] for t in sorted(dados_por_threads)]

threads = [int(linha["threads"].strip()) for linha in dados]
tempos = [
    float(linha["tempo_s"].strip().replace(",", "."))
    for linha in dados
]

if 1 not in threads:
    raise ValueError("O CSV precisa conter o tempo sequencial, identificado com 1 thread.")

if any(tempo <= 0 for tempo in tempos):
    raise ValueError("Todos os tempos do CSV precisam ser maiores que zero.")

indice_sequencial = threads.index(1)
tempo_sequencial = tempos[indice_sequencial]

speedups = [tempo_sequencial / tempo for tempo in tempos]
eficiencias = [
    speedup / quantidade
    for speedup, quantidade in zip(speedups, threads)
]

print("Threads | Tempo (s) | Speed-up | Eficiência")
print("--------|-----------|----------|-----------")

for quantidade, tempo, speedup, eficiencia in zip(
    threads, tempos, speedups, eficiencias
):
    print(f"{quantidade:7} | {tempo:9.4f} | {speedup:8.3f} | {eficiencia:9.3f}")

# Deixa os pontos igualmente espaçados, como no gráfico de referência.
posicoes = list(range(len(threads)))

fig, eixo_eficiencia = plt.subplots(figsize=(10, 6))
eixo_speedup = eixo_eficiencia.twinx()

barras = eixo_eficiencia.bar(
    posicoes,
    eficiencias,
    width=0.55,
    color="#E89B35",
    label="Eficiência",
)

linha_medida, = eixo_speedup.plot(
    posicoes,
    speedups,
    marker="o",
    linewidth=2.5,
    color="#315F98",
    label="Speed-up medido",
)

linha_ideal, = eixo_speedup.plot(
    posicoes,
    threads,
    marker="o",
    linewidth=2.5,
    color="#5B9848",
    label="Speed-up ideal",
)

eixo_eficiencia.set_title("Contagem de números primos")
eixo_eficiencia.set_xlabel("Número de threads")
eixo_eficiencia.set_ylabel("Eficiência")
eixo_speedup.set_ylabel("Speed-up")

eixo_eficiencia.set_xticks(posicoes, [str(t) for t in threads])
eixo_eficiencia.set_ylim(0, max(1.0, max(eficiencias) * 1.1))
eixo_speedup.set_ylim(0, max(threads) * 1.05)
eixo_eficiencia.grid(axis="y", alpha=0.35)

legendas = [barras, linha_medida, linha_ideal]
eixo_eficiencia.legend(
    legendas,
    [item.get_label() for item in legendas],
    loc="upper left",
)

fig.tight_layout()
fig.savefig("speedup_primos.png", dpi=150)
plt.show()