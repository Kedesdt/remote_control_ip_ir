import tkinter as tk
import requests
from botoes import botoesSamsung as botoes

IP = "192.168.20.69"

#url = "http://seu_ip_do_esp32/send?code=0000%20006D%20..."  # Substitua pelo URL correto
# hexa 0000 006D 0000 0022 00AA 00AA 0014 003F 0014 003F 0014 003F 0014 0014 0014 0014
# 0014 0014 0014 0014 0014 0014 0014 003F 0014 003F 0014 003F 0014 0014 0014 0014
# 0014 0014 0014 0014 0014 0014 0014 0014 0014 0014 0014 003F 0014 0014 0014 0014
# 0014 0014 0014 0014 0014 0014 0014 0014 0014 0014 003F 0014 0014 0014 003F 0014
# 003F 0014 003F 0014 003F 0014 003F 0014 003F 0014 0706

def enviar_codigo(codigo):
    # Substitua a URL pelo seu endereço ESP32
    url = f"http://{IP}/send?code={codigo}"
    print (60* "#")
    print("Url: ", url)
    print(60 * "#")
    response = requests.get(url)
    if response.status_code == 200:
        print(f"Comando IR enviado com sucesso para o código {codigo}")
    else:
        print(f"Erro ao enviar o comando IR para o código {codigo}")

def enviarGet(url):
    response = requests.get(url)

    if response.status_code == 200:
        print("Comando IR enviado com sucesso!")
    else:
        print(f"Erro ao enviar o comando IR. Código de status: {response.status_code}")

def criaUrl(codigoHexa):
    lista = codigoHexa.split(' ')
    novo = ''
    for i in range(len(lista) - 1):
        novo += lista[i]
        novo += "%20"
    novo+= lista[-1]
    return novo


root = tk.Tk()
root.title("Controle Remoto IR")

# Crie vários botões com diferentes códigos

for texto, codigo in botoes:
    tk.Button(root, text=texto, command=lambda c=codigo: enviar_codigo(criaUrl(c))).pack()

root.mainloop()