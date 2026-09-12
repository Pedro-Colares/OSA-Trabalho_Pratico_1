import pandas as pd
import matplotlib.pyplot as plt


#Leitura:
metricas = pd.read_csv("metricas.csv")
resultados = pd.read_csv("resultados.csv")

metricas["Overhead"] = metricas["TamahoDisco"] - metricas["BytesUteis"]

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