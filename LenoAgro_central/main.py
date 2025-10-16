import time
import requests
import datetime
import paho.mqtt.client as mqtt
import os
import oracledb  
import pandas as pd

    
# Configurações
BROKER = "mqtt.flespi.io"
PORT = 1883
TOKEN = "FLESPI_TOKEN"
CLIENT_ID = "teste"
TOPIC = "central"

API_URL = "https://api.open-meteo.com/v1/forecast?latitude=23.33&longitude=46.38&daily=precipitation_probability_max,temperature_2m_min,temperature_2m_max&timezone=America%2FSao_Paulo"


def gravar_banco(mqtt_data: dict):
    try:
        # Efetua a conexão com o Usuário no servidor
        conn = oracledb.connect(user='rm567944', password="260299", dsn='oracle.fiap.com.br:1521/ORCL')
        
        # Cria as instruções para cada módulo
        inst_cadastro = conn.cursor()

        # Nomes precisam ser identicos
        sql = f"""INSERT INTO MONITORAMENTO_SOLO (id_dispositivo, cultura, temp_solo, umidade_solo, ph_solo, npk) VALUES('{mqtt_data["id"]}', '{mqtt_data["cultura"]}', '{mqtt_data["temperatura"]}', '{mqtt_data["umidade"]}',' {mqtt_data["ph"]}', '{mqtt_data["tipo_irrigacao"]}')"""
        
        inst_cadastro.execute(sql)
        conn.commit()

    except Exception as e:
        # Informa o erro
        print("Erro: ", e)
        conexao = False

        inst_cadastro.close()
        conn.close()

        return conexao
    else:
        conexao = True

        inst_cadastro.close()
        conn.close()

        return conexao
    # Callback quando conecta

def ler_banco() -> pd.DataFrame:
    os.system('cls')
    # Aqui ta se conectando
    try:
        lista_dados = []
        # Efetua a conexão com o Usuáriono servidor
        conn = oracledb.connect(user='rm567944', password="260299", dsn='oracle.fiap.com.br:1521/ORCL')
        
        inst_exibe = conn.cursor()
        sql = "SELECT * FROM MONITORAMENTO_SOLO ORDER BY DATA_HORA_GRAVACAO DESC"
        inst_exibe.execute(sql)
        
        data = inst_exibe.fetchall()


        # Insere os valores da tabela na Lista
        for dt in data:
            lista_dados.append(dt)

        
                
        # ordena a lista
        # lista_dados = sorted(lista_dados)
        # Gera um DataFrame com os dados da lista utilizando o Pandas
        dados_df = pd.DataFrame.from_records(lista_dados, columns=['data_hora_gravacao','id_dispositivo', 'cultura', 'temp_solo', 'umidade_solo', 'ph_solo', 'npk'])
        

        return dados_df
    
        

    except Exception as e:
        # Informa o erro
        print("Erro: ", e)# Flag para não executar a Aplicação
        conexao = False

        inst_exibe.close()
        conn.close()

        return conexao
    else:# Flag para executar a Aplicação
        conexao = True

        inst_exibe.close()
        conn.close()

        return conexao
    # Callback quando conecta


def on_connect(client, userdata, flags, rc):
    if rc == 0:
        client.subscribe(TOPIC)
    else:
        print(f"Falha na conexão. Código: {rc}")

# Callback quando recebe mensagem
def on_message(client, userdata, msg):
    try:
        payload = msg.payload.decode()  # converte bytes -> string

        valores = payload.split(";")

        # Converte para dicionário
        # INPUT: 2602;cana-de-açucar;-5.80;43.00;3.00;K
        dados = {
            "id": valores[0],
            "cultura": valores[1],
            "temperatura": float(valores[2]),
            "umidade": float(valores[3]),
            "ph": float(valores[4]),
            "tipo_irrigacao": valores[5]
        }

        # Aqui chama função para gravação no banco
        gravar_banco(dados)
        

    except Exception as e:
        print("Erro ao processar mensagem:", e)

# Retorna dados em JSON
def consultar_open_meteo() -> dict:
    response = requests.get(API_URL)
        
    data = response.json()

    return data

def ultimo_registro(dados: pd.DataFrame) -> None:
    print("[    ULTIMO REGISTRO     ]")
    dados.columns = ['Data Hora', 'ID Device', 'Cultura', 'Temp Solo', 'Umidade Solo', 'pH Solo', 'NPK']
    print(dados.iloc[[0]])
    print('\n')

def tabela(dados: pd.DataFrame) -> None:
    print("[    ULTIMOS REGISTROS     ]")
    print(dados.head())

def previsao_semana() -> None:
    dados_api = consultar_open_meteo()
    # message = f'{dados_api['daily']['time'][0]};{dados_api['daily']['temperature_2m_min'][0]};{dados_api['daily']['temperature_2m_max'][0]};{dados_api['daily']['precipitation_probability_max'][0]}'
    
    msg = (f"[    PREVISÃO    ] --> \tProb. Chuva: {dados_api['daily']['precipitation_probability_max'][0]}% \tTemp. Máx.: {dados_api['daily']['temperature_2m_max'][0]}°C\tTemp. Min.: {dados_api['daily']['temperature_2m_min'][0]}°C")

    data = (f"[    DATA    ] ------> {dados_api['daily']['time'][0]}")

    return {
        "data": data,
        "msg": msg
    }


def alertas(alerta_temperatura: float, alerta_umidade: float, alerta_ph: float, alerta_npk: str) -> None:
    print("\n")
    #'data_hora_gravacao','id_dispositivo', 'cultura', 'temp_solo', 'umidade_solo', 'ph_solo', 'npk'

    if(alerta_temperatura > 35): print("[    ATENÇÃO    ] --> Temperatura do solo Alta!")
    if(alerta_umidade < 50): print("[    ATENÇÃO    ] --> Solo em escassez hidrica. Umidade abaixo de 50%!")
    if(alerta_ph < 5): print("[    ATENÇÃO    ] --> pH do solo está muito baixo")
    
    match alerta_npk:
        case 'NPK':
            print("[    ATENÇÃO    ] --> Repor macronutrientes NPK")
            return
        case 'NP':
            print("[    ATENÇÃO    ] --> Repor macronutrientes NP")
            return
        case 'NK':
            print("[    ATENÇÃO    ] --> Repor macronutrientes NK")
            return
        case 'PK':
            print("[    ATENÇÃO    ] --> Repor macronutrientes PK")
            return
        case 'N':
            print("[    ATENÇÃO    ] --> Repor macronutriente N")
            return
        case 'P':
            print("[    ATENÇÃO    ] --> Repor macronutriente P")
            return
        case 'K':
            print("[    ATENÇÃO    ] --> Repor macronutriente K")
            return
        
# Criar e configurar cliente
client = mqtt.Client(client_id=CLIENT_ID)
client.username_pw_set(TOKEN)
client.on_connect = on_connect
client.on_message = on_message

client.connect(BROKER, PORT, 60)


previsao = {}
while True:
    client.loop_start() 

    dados = ler_banco()

    alerta_temperatura = dados.at[0, "temp_solo"]
    alerta_umidade = dados.at[0, "umidade_solo"]
    alerta_ph = dados.at[0, "ph_solo"]
    alerta_npk = dados.at[0, "npk"]
 
    # #printa primeira linha
    ultimo_registro(dados)

    #printa tabela
    tabela(dados)

    hoje = datetime.datetime.now()
    # printa previsao do tempo
    if hoje.hour == 7 and hoje.minute == 26:
        previsao = previsao_semana()
        print(f"\n{previsao["data"]}")
        print(previsao["msg"])

        #printa alertas
        alertas(alerta_temperatura, alerta_umidade, alerta_ph, alerta_npk)

        time.sleep(61)

    else:
        if len(previsao) == 0:
            #printa alertas
            alertas(alerta_temperatura, alerta_umidade, alerta_ph, alerta_npk)

            time.sleep(30)

        else:
            # printa previsao
            print(f"\n{previsao["data"]}")
            print(previsao["msg"])

            #printa alertas
            alertas(alerta_temperatura, alerta_umidade, alerta_ph, alerta_npk)

            time.sleep(30)

        


    


    # previsao = datetime.datetime.now()

    # # Desconectar
    # client.loop_stop()
    # client.disconnect()

    # # Todo dia, as 06:01H, é realizada uma consulta a OpenMeteo API para termos previsão do tempo, chuva e temperaturas maxima e minima
    # if previsao.hour == 6 and previsao.minutes == 0:
    #     data = consultar_open_meteo()
    #     # Para evitar rodar varias vezes no mesmo minuto;
    #     time.sleep(2 * 60)
    # else:
    #     # Codigo do menu e sistema AQUI
    #     ler_banco()
    #     # Exibir ultimo registro
    #     print("Ultimo registro: ")
    #     # Gerar alerta quando em situações criticas
    #     if (umidade < 50 or temperatura > 35):
    #         print("Alertas importantes")
    #     # Indicar reposição de NPK
    #     if (npk != "OK"):
    #         print("Repor", npk)
    #     # Indicar Calagem

    #     if(ph_solo < 5):
    #         print("Adicionar Cal")
    #     # Visualizar ultimos 5 registros
