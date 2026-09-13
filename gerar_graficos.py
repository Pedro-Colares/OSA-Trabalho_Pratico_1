import pandas as pd
import matplotlib.pyplot as plt


#Leitura:
metricas = pd.read_csv("metricas.csv")
resultados = pd.read_csv("resultados.csv")

metricas["Overhead"] = metricas["TamanhoDisco"] - metricas["BytesUteis"]

metricas["BytesUteisMB"] = metricas["BytesUteis"] / (1024 * 1024)
metricas["TamanhoDiscoMB"] = metricas["TamanhoDisco"] / (1024 * 1024)

metricas["BytesUteisPercentual"] = (
    metricas["BytesUteis"] / metricas["TamanhoDisco"] * 100
)

metricas["OverheadPercentual"] = (
    metricas["Overhead"] / metricas["TamanhoDisco"] * 100
)

#Tabela:
tabela = metricas[
    [
        "Formato",
        "N",
        "TamanhoDisco",
        "BytesUteis",
        "Overhead",
        "Eficiencia",
        "TempoLeitura"
    ]
    
].copy()

tabela["TamanhoDisco"] = tabela["TamanhoDisco"].apply(
    lambda x: f"{x / (1024 * 1024):.2f} MB"
    if x >= 1024 * 1024
    else f"{x / 1024:.2f} KB"
)

tabela["BytesUteis"] = tabela["BytesUteis"].apply(
    lambda x: f"{x / (1024 * 1024):.2f} MB"
    if x >= 1024 * 1024
    else f"{x / 1024:.2f} KB"
)

tabela["Overhead"] = tabela["Overhead"].apply(
    lambda x: f"{x / (1024 * 1024):.2f} MB"
    if x >= 1024 * 1024
    else f"{x / 1024:.2f} KB"
)

tabela["Eficiencia"] = tabela["Eficiencia"].apply(
    lambda x: f"{x:.2f}%"
)

tabela["TempoLeitura"] = tabela["TempoLeitura"].apply(
    lambda x: f"{x:.2f} ms"
)

print("\n===== TABELA =====\n")
print(tabela.to_string(index=False))

#Gráfico 1:
plt.figure();

for formato in ["Fixo", "Delimitado", "Indicador"]:
    dados = metricas[metricas["Formato"] == formato]

    plt.plot(
        dados["N"],
        dados["TamanhoDiscoMB"],
        marker="o",
        label=formato
    )

dados_uteis = metricas.drop_duplicates("N").sort_values("N")
plt.plot(
        dados_uteis["N"],
    dados_uteis["BytesUteisMB"],
    marker="o",
    label="Bytes Úteis"
    )

plt.xlabel("Quantidade de Registros")
plt.ylabel("Tamanho Físico (MB)")
plt.title("Ocupação em Disco vs. Volume de Dados:")
plt.legend()
plt.grid(True)
plt.xticks([1000, 10000, 50000, 100000], ["1k", "10k", "50k", "100k"])
plt.tight_layout()
plt.savefig("grafico1_ocupacaoVSvolume.png", dpi=300)
plt.show()

#Gráfico 2:

formatos = ["Fixo", "Delimitado", "Indicador"]
dados2 = (
    metricas.groupby("Formato").agg({
        "BytesUteis": "mean",
        "BytesUteisPercentual": "mean",
        "OverheadPercentual": "mean"
    })
    .reindex(formatos)
)

plt.figure()
plt.bar(
    formatos,
    dados2["BytesUteisPercentual"],
    label = "Bytes Úteis"
)

plt.bar(
    formatos,
    dados2["OverheadPercentual"],
    bottom = dados2["BytesUteisPercentual"],
    label = "Overhead"
)

plt.xlabel("Formato")
plt.ylabel("Composição do Arquivo (%)")
plt.title("Eficiência de Armazenamento e Fragmentação Interna (%):")
plt.ylim(0, 100)
plt.legend()
plt.grid(axis="y")
plt.tight_layout()
plt.savefig("grafico2_eficiencia.png", dpi=300)
plt.show()

#Gráfico 3:
plt.figure()
for formato in ["Fixo", "Delimitado", "Indicador"]:
    dados = metricas[metricas["Formato"] == formato]

    plt.plot(
        dados["N"],
        dados["TempoLeitura"],
        marker="o",
        label=formato
    )

plt.xlabel("Quantidade de Registros")
plt.ylabel("Tempo de Leitura (ms)")
plt.title("Desempenho de Leitura Sequencial Total (ms):")
plt.legend()
plt.grid(True)
plt.xticks([1000, 10000, 50000, 100000], ["1k", "10k", "50k", "100k"])
plt.tight_layout()
plt.savefig("grafico3_desempenho.png", dpi=300)
plt.show()

#Gráfico 4:
plt.figure()
plt.plot(
    resultados["N"],
    resultados["TempoDireto"],
    marker="o",
    label="Acesso Direto - Fixo"
)
plt.plot(
    resultados["N"],
    resultados["TempoSequencial"],
    marker="o",
    label="Busca Sequencial - Delimitado"
)

plt.xlabel("Quantidade de Registros")
plt.ylabel("Tempo Total (ms)")
plt.title("Acesso Direto (RRN em O(1)) vs. Varredura Sequencial (O(N)):")
plt.yscale("log")
plt.xticks([1000, 10000, 50000, 100000], ["1k", "10k", "50k", "100k"])
plt.legend()
plt.grid(True, which="both")
plt.tight_layout()
plt.savefig("grafico4_acesso.png", dpi=300)
plt.show()



